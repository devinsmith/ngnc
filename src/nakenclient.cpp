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

#include "nakenclient.h"

#ifndef WIN32
#include <resolv.h>
#endif

FXDEFMAP(NakenClient) NakenClientMap[] = {
  FXMAPFUNC(SEL_IO_WRITE, NakenClient::ID_CONNECT, NakenClient::OnConnected),
  FXMAPFUNC(SEL_IO_READ, NakenClient::ID_READ, NakenClient::OnIORead),
  FXMAPFUNC(SEL_COMMAND, NakenClient::ID_DNS_MSG, NakenClient::OnDnsMsg)
};

FXIMPLEMENT(NakenClient, FXObject, NakenClientMap, ARRAYNUMBER(NakenClientMap))

NakenClient::NakenClient(FXApp *app, FXObject *tgt, FXSelector sel) :
  application(app), selector(sel), _initialized{false}, m_hasSsl(false), m_useSsl(false)
{
  serverName = "localhost";
  serverPort = 6666;
  nickName = "";
  receiveRest = "";
  connected = false;
  targets.push_back(tgt);

  resolver = new DnsResolver(app, this, FXSEL(SEL_COMMAND, ID_DNS_MSG));

  // Register the error strings for libcrypto & libssl
  SSL_load_error_strings();
  // Register the available ciphers and digests
  SSL_library_init();
  OpenSSL_add_all_algorithms();

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)
  const SSL_METHOD *method = TLSv1_2_client_method();
#else
  const SSL_METHOD *method = TLS_client_method();
#endif

  if ((m_sslCtx = SSL_CTX_new(method)) != nullptr) {
    m_hasSsl = true;

    // XXX: In the future do some verification.
    SSL_CTX_set_verify(m_sslCtx, SSL_VERIFY_NONE, nullptr);
    SSL_CTX_set_verify_depth(m_sslCtx, 0);
    SSL_CTX_set_mode(m_sslCtx, SSL_MODE_AUTO_RETRY);
    SSL_CTX_set_session_cache_mode(m_sslCtx, SSL_SESS_CACHE_CLIENT);
  }
}

NakenClient::~NakenClient()
{
  if (connected) Disconnect();
#ifdef WIN32
    WSACleanup();
#endif
}

void NakenClient::Initialize()
{
  if (_initialized)
    return;

#ifdef WIN32
  WORD wVersionRequested = MAKEWORD(2,0);
  WSADATA data;

  if (WSAStartup(wVersionRequested, &data) != 0) {
    SendEvent(CHAT_ERROR, "Unable initiliaze socket");
    return -1;
  }
#endif
  _initialized = true;
}

FXint NakenClient::Connect()
{
  Initialize();

  resolver->Resolve(serverName);

  return 0;
}

void NakenClient::AsyncConnect(const std::vector<FXString>& addrs)
{
  for (size_t i = 0; i < addrs.size(); i++) {
    const FXString& addr = addrs[i];
    SendEvent(CHAT_CONNECT, FXStringFormat("Connecting to %s (%s)... ",
          serverName.text(), addr.text()));

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
      SendEvent(CHAT_ERROR, "\nUnable create socket\n");
      return;
    }

    serverSock.sin_family = AF_INET;
    serverSock.sin_port = htons(serverPort);
    serverSock.sin_addr.s_addr = inet_addr(addr.text());

    // Set socket as non-blocking.
    SetBlocking(true);

    if (connect(sock, (sockaddr *)&serverSock, sizeof(serverSock)) != 0) {
      if (errno != EINPROGRESS) {
        SendEvent(CHAT_ERROR, FXStringFormat("Unable connect to: %s",
            serverName.text()));
        break;
      }
    }
    if (m_hasSsl && m_useSsl) {
      m_ssl = SSL_new(m_sslCtx);
      if (m_ssl == nullptr) {
        // XXX: Don't dump to stderr.
        ERR_print_errors_fp(stderr);
        return;
      }
    }
#ifdef WIN32
    event = WSACreateEvent();
    WSAEventSelect(sock, event, FD_CONNECT|FD_READ|FD_CLOSE); // sets non-blocking!!
    application->addInput((FXInputHandle)event, INPUT_WRITE, this, ID_CONNECT);
#else
    application->addInput((FXInputHandle)sock, INPUT_WRITE, this, ID_CONNECT);
#endif
    break;
  }
}

void NakenClient::SendEvent(ChatEventType eventType)
{
  ChatEvent ev;
  ev.eventType = eventType;
  ev.param1 = "";
  ev.param2 = "";
  ev.param3 = "";
  ev.param4 = "";
  ev.nakenClient = this;
  for (size_t i = 0; i < targets.size(); i++) {
    FXObject *tgt = targets[i];
    tgt->handle(this, FXSEL(SEL_COMMAND, ID_SERVER), &ev);
  }
}

void NakenClient::SendEvent(ChatEventType eventType, const FXString &param1)
{
  ChatEvent ev;
  ev.eventType = eventType;
  ev.param1 = param1;
  ev.param2 = "";
  ev.param3 = "";
  ev.param4 = "";
  ev.nakenClient = this;
  for (size_t i = 0; i < targets.size(); i++) {
    FXObject *tgt = targets[i];
    tgt->handle(this, FXSEL(SEL_COMMAND, ID_SERVER), &ev);
  }
}

long NakenClient::OnDnsMsg(FXObject *, FXSelector, void *obj)
{
  DNSEvent *event = static_cast<DNSEvent *>(obj);

  ChatEventType evType = CHAT_CONNECT;
  if (event->type == DNS_ERROR) {
    evType = CHAT_ERROR;
  }
  SendEvent(evType, event->msg);

  if (event->type == DNS_RESOLVED) {
    AsyncConnect(event->addrs);
  }
  return 1;
}

long NakenClient::OnConnected(FXObject *, FXSelector, void *)
{

  if (m_useSsl) {
    if (!SSL_set_fd(m_ssl, sock))
      ERR_print_errors_fp(stderr);

    int r;

    if ((r = SSL_connect(m_ssl)) != 1) {
      switch (SSL_get_error(m_ssl, r)) {
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
          return 1; // Just retry this.
        default:
          // Error.
          SendEvent(CHAT_CONNECT, "error!\n");
          connected = false;
          return 1;
      }
    }
  }
  SendEvent(CHAT_CONNECT, "done!\n");
  connected = true;

  // Setup read
#ifdef WIN32
  application->removeInput((FXInputHandle)event, INPUT_WRITE);
  application->addInput((FXInputHandle)event, INPUT_READ, this, ID_READ);
#else
  application->removeInput((FXInputHandle)sock, INPUT_WRITE);
  application->addInput((FXInputHandle)sock, INPUT_READ, this, ID_READ);
#endif

  return 1;
}

long NakenClient::OnIORead(FXObject *, FXSelector, void *)
{
  if (connected) {
    ReadData();
  }
  return 1;
}

long NakenClient::ReadData()
{
  FXchar buffer[1024];
  int i, size = 0;

  FXString data = receiveRest;
#ifdef WIN32
  WSANETWORKEVENTS network_events;
  WSAEnumNetworkEvents(sock, event, &network_events);
  if (network_events.lNetworkEvents & FD_READ) {
    // Read into buffer.
    size = recv(sock, buffer, sizeof(buffer), 0);
    if (size > 0) {
      // Should we check for non-utf8 ?
      for (i = 0; i < size; i++) {
        // Yes the server sends NULL bytes!?!?!
        if (buffer[i] == '\r' || buffer[i] == '\0')
          continue;

        if (buffer[i] == '\n') {
          // data contains line.
          ParseLine(data);
          data.clear();
        } else {
          data.append(buffer[i]);
        }
      }
      receiveRest = data;
    } else if (size < 0) {
      SendEvent(CHAT_ERROR, FXStringFormat("Error in reading data from %s",
            serverName.text()));
    } else { // size == 0
      Disconnect();
    }
  } else if (network_events.lNetworkEvents & FD_CLOSE) Disconnect();
#else
  if (m_useSsl) {
    size = SSL_read(m_ssl, buffer, sizeof(buffer));
    if (size <= 0) {
      switch (SSL_get_error(m_ssl, size)) {
      case SSL_ERROR_WANT_READ:
      case SSL_ERROR_WANT_WRITE:
        return 1;
      case SSL_ERROR_ZERO_RETURN:
        break;
      default:
        break;
      }
    }
  } else {
    size = recv(sock, buffer, sizeof(buffer), 0);
  }
  if (size > 0) {
    // Should we check for non-utf8 ?
    for (i = 0; i < size; i++) {
      // Yes the server sends NULL bytes!?!?!
      if (buffer[i] == '\r' || buffer[i] == '\0')
        continue;

      if (buffer[i] == '\n') {
        // data contains line.
        ParseLine(data);
        data.clear();
      } else {
        data.append(buffer[i]);
      }
    }
    receiveRest = data;
  } else if (size < 0) {
    SendEvent(CHAT_ERROR, FXStringFormat("Error in reading data from %s",
          serverName.text()));
  } else { // size == 0
    Disconnect();
  }
#endif
  return size;
}

void NakenClient::Disconnect(bool user_initiated)
{
  if (user_initiated) {
    SendEvent(CHAT_DISCONNECT, "You have disconnected\n");
  } else {
    SendEvent(CHAT_DISCONNECT, FXStringFormat("Server %s was disconnected\n",
        serverName.text()));
  }
#ifdef WIN32
  shutdown(sock, SD_BOTH);
  closesocket(sock);
  if(event) {
   application->removeInput((FXInputHandle)event, INPUT_READ);
   WSACloseEvent(event);
   event = NULL;
  }
#else
#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif
  shutdown(sock, SHUT_RDWR);
  close(sock);
  application->removeInput(sock, INPUT_READ);
#endif
  connected = false;
}

void NakenClient::ParseLine(const FXString& line)
{
  SendEvent(CHAT_MSG, line + '\n');
}

int NakenClient::SetBlocking(bool enable)
{
#ifdef WIN32
  u_long on;

  on = enable ? 1 : 0;

  int ret = ioctlsocket(sock, FIONBIO, &on);

  // normalize error to -1.
  if (ret != 0)
    return -1;
#else
  int flags;

  if ((flags = fcntl(sock, F_GETFL)) == -1) {
    fprintf(stderr, "Failed to get socket flags: %s", strerror(errno));
    return -1;
  }

  if (enable)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;

  if (fcntl(sock, F_SETFL, flags) == -1) {
    fprintf(stderr, "Failed to set socket flags: %s", strerror(errno));
    return -1;
  }
#endif
  return 0;
}

int NakenClient::SendMessage(const FXString& msg)
{
  int written;

  if (m_useSsl) {
    written = SSL_write(m_ssl, msg.text(), msg.length());
  } else {
    written = write(sock, msg.text(), msg.length());
  }
  return written;
}

void NakenClient::AppendTarget(FXObject *tgt)
{
  targets.push_back(tgt);
}
