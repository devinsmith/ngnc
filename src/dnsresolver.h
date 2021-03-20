/*
 * Copyright (c) 2020 Devin Smith <devin@devinsmith.net>
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

#ifndef NGNC_DNSRESOLVER_H
#define NGNC_DNSRESOLVER_H

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "defs.h"

#include <vector>

enum DnsEventType {
  DNS_RESOLVING,
  DNS_RESOLVED,
  DNS_ERROR
};

struct DNSEvent {
  DnsEventType type;
  FXString msg;

  std::vector<FXString> addrs;
};

struct DnsResource {
  FXString host;
  unsigned short domain_type;
  unsigned short domain_class;
  unsigned int ttl;
  unsigned short data_len;
  unsigned short data_pos;
  FXString data;
};

class DnsResolver : public FXObject {
  FXDECLARE(DnsResolver);
public:
  DnsResolver(FXApp *app, FXObject*, FXSelector);
  virtual ~DnsResolver();
  enum {
    ID_READ = FXMainWindow::ID_LAST+1100,
    ID_RESOLVING,
    ID_RESOLVED
  };

  int Resolve(const FXString &name);
  long OnIORead(FXObject*, FXSelector, void*);
private:
  DnsResolver() {}
  FXString GetDNS();
  void SendEvent(DnsEventType type, const FXString& msg);
  void SendEvent(DnsEventType type, const FXString& msg,
      const std::vector<FXString>& addrs);

  int BuildDnsQueryPacket(const char* host, char* buf, int pos, int end);
  std::vector<DnsResource> ParseDnsResponsePacket(const char* buf, int end);


  FXApp *application;
  FXObject *target;
  FXSelector selector;

#ifdef WIN32
  SOCKET socketid;
  WSAEVENT event;
#else
  int socketid;
#endif
  sockaddr_in dnsServerAddr;
};

#endif /* NGNC_DNSRESOLVER_H */

