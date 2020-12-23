/*
 * Copyright (c) 2016-2019 Devin Smith <devin@devinsmith.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "dnsresolver.h"

#include <string>

#ifndef WIN32
#include <resolv.h>
#endif

const static unsigned short DNS_UDP_PORT = 53;

// Some common DNS resolvers:
const static std::string DNS_HOST_IP_114  = "114.114.114.114";
const static std::string DNS_HOST_IP_8  = "8.8.8.8";
const static std::string DNS_HOST_IP_4  = "4.2.2.1";

typedef struct tagDnsQuestionSection {
  FXString host;
  unsigned short query_type;
  unsigned short query_class;
} DnsQuestionSection;

FXDEFMAP(DnsResolver) DnsResolverMap[] = {
  FXMAPFUNC(SEL_IO_READ, DnsResolver::ID_READ, DnsResolver::OnIORead)
};

FXIMPLEMENT(DnsResolver, FXObject, DnsResolverMap, ARRAYNUMBER(DnsResolverMap))

DnsResolver::DnsResolver(FXApp *app, FXObject *tgt, FXSelector sel) :
  application(app), target(tgt), selector(sel)
{
}

DnsResolver::~DnsResolver()
{
}

FXString DnsResolver::GetDNS()
{
#ifdef WIN32
  // XXX: Figure out how to get Windows DNS servers.
  return "8.8.8.8";
#else

  res_init();
  printf("Number of DNS: %d\n", _res.nscount);

  struct sockaddr_in dns = _res.nsaddr_list[0];
  char *addr = inet_ntoa(dns.sin_addr);
  printf("Addr: %s\n", addr);
  return addr;
#endif
}

int DnsResolver::Resolve(const FXString& server)
{
  char buf[520];

  FXString dnsServer = GetDNS();

  SendEvent(DNS_RESOLVING, FXStringFormat("Resolving %s using %s...",
        server.text(), dnsServer.text()));

  if ((socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    SendEvent(DNS_ERROR, "Unable to create DNS socket");
    return -1;
  }

  dnsServerAddr.sin_family = AF_INET;
  dnsServerAddr.sin_port = htons(DNS_UDP_PORT);
  dnsServerAddr.sin_addr.s_addr = inet_addr(dnsServer.text());

  int len = BuildDnsQueryPacket(server.text(), buf, 0, sizeof(buf));
  if (len < 0) {
    printf("build dns query packet fail.");
    return -1;
  }

  sendto(socketid, buf, len, 0, (struct sockaddr *)&dnsServerAddr,
      sizeof(dnsServerAddr));

#ifdef WIN32
  event = WSACreateEvent();
  WSAEventSelect(socketid, event, FD_CONNECT|FD_READ|FD_CLOSE); // sets non-blocking!!
  application->addInput((FXInputHandle)event, INPUT_READ, this, ID_READ);
#else
  application->addInput((FXInputHandle)socketid, INPUT_READ, this, ID_READ);
#endif
  return 0;
}

int DnsResolver::BuildDnsQueryPacket(const char* host, char* buf, int pos,
    int end)
{
  if (buf == NULL || host == NULL) {
    return 0;
  }
  //==========header section===========
  // query transaction id
  unsigned short query_id = 0x1234;
  buf[pos++] = 0xff & (query_id>>8);
  buf[pos++] = 0xff & query_id;

  bool req_recursive = true;
  // |qr| opcode |aa|tc|rd|rd|
  buf[pos++] = req_recursive ? 0x01 : 0x00;
  // |ra|reseverd|rcode|
  buf[pos++] = 0x00;

  // query count
  unsigned short query_cnt = 0x0001;
  buf[pos++] = 0xff & (query_cnt>>8);
  buf[pos++] = 0xff & query_cnt;

  // ans rr = 0
  buf[pos++] = 0;
  buf[pos++] = 0;

  buf[pos++] = 0;
  buf[pos++] = 0;

  buf[pos++] = 0;
  buf[pos++] = 0;

  //==========query section========
  int   cp    = 0;
  char  ch    = 0;
  char  last  = 0;
  int   lp    = pos++;
  while ((ch = host[cp++]) != '\0' && pos < end) {
    if (ch != '.') {
      buf[pos++] = ch;
      last = ch;
      continue;
    }
    int len = pos - lp -1;
    if (len <= 0 || len > 63) {
      printf("host name format invalid.\n");
      return -1;
    }
    buf[lp] = len;
    lp = pos++;
  }
  if (last == '.') {
    buf[lp] = 0;
  } else {
    buf[lp]     = pos - lp - 1;
    buf[pos++]  = 0;
  }

  //==========query type==========
  unsigned short query_type = 0x0001;
  buf[pos++] = 0xff & (query_type >> 8);
  buf[pos++] = 0xff & query_type;

  //==========query class=========
  unsigned short query_class = 0x0001;
  buf[pos++] = 0xff & (query_class >> 8);
  buf[pos++] = 0xff & query_class;

  return pos;
}

void DnsResolver::SendEvent(DnsEventType type, const FXString& msg)
{
  DNSEvent event;

  event.type = type;
  event.msg = msg;

  target->handle(this, selector, &event);
}

void DnsResolver::SendEvent(DnsEventType type, const FXString& msg,
    const std::vector<FXString>& addrs)
{
  DNSEvent event;

  event.type = type;
  event.msg = msg;
  event.addrs = addrs;

  target->handle(this, selector, &event);
}

long DnsResolver::OnIORead(FXObject *, FXSelector, void *)
{
  char buf[520];
  socklen_t addr_len	= sizeof(dnsServerAddr);
  printf("Got response\n");
  int len = recvfrom(socketid, buf, sizeof(buf), 0,
      (struct sockaddr *)&dnsServerAddr, &addr_len);

  // Parse DNS Response
  std::vector<DnsResource> answers = ParseDnsResponsePacket(buf, len);
  std::vector<FXString> addrs;

  for (size_t i = 0; i < answers.size(); i++) {
    const DnsResource& answer = answers[i];

    if (answer.domain_type == 1) {
      addrs.push_back(answer.data);
    }
  }

#ifdef WIN32
  WSAEventSelect(socketid, event, 0); // sets non-blocking!!
  application->removeInput((FXInputHandle)event, INPUT_READ);
#else
  application->removeInput((FXInputHandle)socketid, INPUT_READ);
#endif

  if (addrs.size() == 0) {
    SendEvent(DNS_ERROR, "\nError: unable to resolve\n");
  } else {
    SendEvent(DNS_RESOLVED, "done!\n", addrs);
  }
  return 1;
}

FXString IPBin2Dec(const FXString& data) {
  if (data.length() < 4) {
    return "";
  }
  char buf[32] = {0};
  snprintf(buf, sizeof(buf), "%d.%d.%d.%d",
    (unsigned char)data[0], (unsigned char)data[1],
    (unsigned char)data[2], (unsigned char)data[3]);
  return buf;
}

int ParseUnsignedInt(const char* buf, int pos, int end, unsigned int& value) {
  value = 0;
  value = (unsigned char)buf[pos++];
  value = (value << 8)|(unsigned char)buf[pos++];
  value = (value << 8)|(unsigned char)buf[pos++];
  value = (value << 8)|(unsigned char)buf[pos++];

  return pos;
}

int ParseUnsignedShort(const char* buf, int pos, int end, unsigned short& value) {
  value = 0;
  value = (unsigned char)buf[pos++];
  value = (value << 8)|(unsigned char)buf[pos++];
  return pos;
}

int ParseHost(const char* buf, int pos, int end, FXString& host) {
  if (buf == NULL) {
    return pos;
  }
  unsigned int limit = 0xc0;
  unsigned int len = (unsigned char)buf[pos++];
  while (len != 0 && !(len & limit)) {
    host.append(buf+pos, len);
    pos += len;
    len = (unsigned char)buf[pos++];
    if (len != 0) {
      host.append(".");
    }
  }
  if (len & limit) {
    unsigned int offset = ((limit ^ len) << 8) | (unsigned char)buf[pos++];
    ParseHost(buf, offset, end, host);
  }	
  return pos;
}

int ParseQuestionSection(const char* buf, int pos, int end, DnsQuestionSection& dns_question_section) {
  pos = ParseHost(buf, pos, end, dns_question_section.host);
  pos = ParseUnsignedShort(buf, pos, end, dns_question_section.query_type);
  pos = ParseUnsignedShort(buf, pos, end, dns_question_section.query_class);
  return pos;
}

int ParseResourceRecord(const char* buf, int pos, int end,
    DnsResource& dns_resource) {
  if (buf == NULL) {
    return pos;
  }
  pos = ParseHost(buf, pos, end, dns_resource.host);
  pos = ParseUnsignedShort(buf, pos, end, dns_resource.domain_type);
  pos = ParseUnsignedShort(buf, pos, end, dns_resource.domain_class);
  pos = ParseUnsignedInt(buf, pos, end, dns_resource.ttl);
  pos = ParseUnsignedShort(buf, pos, end, dns_resource.data_len);
  dns_resource.data_pos = pos;
  pos += dns_resource.data_len;
  return pos;
}

int ParseDnsRecordDataField(const char* buf, int pos, int end,
    DnsResource& res)
{
  unsigned short type = res.domain_type;
  if (type == 1) {
    res.data = IPBin2Dec(FXString(buf + res.data_pos, res.data_len));
  } else if (type == 2 || type == 5) {
    ParseHost(buf, res.data_pos, end, res.data);
  } else if (type == 28) {
    res.data = "IPV6 ADDR";
  } else {
    res.data = "OTHERS";
  }
  return 0;
}

std::vector<DnsResource> DnsResolver::ParseDnsResponsePacket(const char* buf,
    int end) {
  std::vector<DnsResource> answers;

  if (buf == NULL) {
    return answers;
  }
  int pos = 0;
  // query transaction id
  unsigned short query_id = 0;
  query_id = buf[pos++];
  query_id = (query_id << 8) | buf[pos++];

  //	bool req_recursive = false;
  unsigned short opcode_info = 0;
  // |qr| opcode |aa|tc|rd|rd|
  pos = ParseUnsignedShort(buf, pos, end, opcode_info);
  if (opcode_info & 0x0f) {
    printf("dns ret code non-zero, ret = %d\n", opcode_info & 0x0f);
    return answers;
  }

  if (opcode_info&0x80) {
    printf("recursived response.\n");
  } else {
    printf("non-recursived response.\n");
  }
  unsigned short query_cnt = 0;
  pos = ParseUnsignedShort(buf, pos, end, query_cnt);

  printf ("response query_cnt = %d\n", query_cnt);

  unsigned short answer_cnt = 0;
  pos = ParseUnsignedShort(buf, pos, end, answer_cnt);
  printf("response answer_cnt = %d\n", answer_cnt);

  unsigned short authority_cnt = 0;
  pos = ParseUnsignedShort(buf, pos, end, authority_cnt);
  printf("response authority_cnt = %d\n", authority_cnt);

  unsigned short additional_cnt = 0;
  pos = ParseUnsignedShort(buf, pos, end, additional_cnt);
  printf("response addtional_cnt = %d\n", additional_cnt);

  //============query section=================
  for (int i = 0; i < query_cnt; i++) {
    DnsQuestionSection dns_question;
    pos = ParseQuestionSection(buf, pos, end, dns_question);
    printf("question section: host = %s, type = %2d, class = %2d\n", dns_question.host.text(), dns_question.query_type, dns_question.query_class);
  }

  //===========answer section=================
  printf("[  answer section   ]\n");
  for (int i = 0; i < answer_cnt; i++) {
    DnsResource res;
    pos = ParseResourceRecord(buf, pos, end, res);
    ParseDnsRecordDataField(buf, pos, end, res);
    printf("host = %s, type = %2d, class = %2d, ttl = %4u, dlen = %2d, data = %s\n",
    res.host.text(), res.domain_type, res.domain_class, res.ttl, res.data_len, res.data.text());
    answers.push_back(res);
  }

  //==========authority section==============
  printf("[  authority section   ]\n");
  for (int i = 0; i < authority_cnt; i++) {
    DnsResource res;
    pos = ParseResourceRecord(buf, pos, end, res);
    ParseDnsRecordDataField(buf, pos, end, res);
    printf("host = %s, type = %2d, class = %2d, ttl = %4u, dlen = %2d, data = %s\n",
    res.host.text(), res.domain_type, res.domain_class, res.ttl, res.data_len, res.data.text());
  }

  //==========additional section=============
  printf("[  additional section   ]\n");
  for (int i = 0; i < additional_cnt; i++) {
    DnsResource res;
    pos = ParseResourceRecord(buf, pos, end, res);
    ParseDnsRecordDataField(buf, pos, end, res);
    printf("host = %s, type = %2d, class = %2d, ttl = %4u, dlen = %2d, data = %s\n",
    res.host.text(), res.domain_type, res.domain_class, res.ttl, res.data_len, res.data.text());
  }
  return answers;
}
