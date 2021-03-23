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

#ifndef NGNC_SOCKET_H
#define NGNC_SOCKET_H

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
#include <fcntl.h>
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <cerrno>
#include <vector>

#include "defs.h"
#include "dnsresolver.h"

class NakenClient : public FXObject {
  FXDECLARE(NakenClient);
public:
  NakenClient(FXApp *app, FXObject*, FXSelector);
  virtual ~NakenClient();
  enum {
    ID_READ = FXMainWindow::ID_LAST+1100,
    ID_CONNECT,
    ID_DNS_MSG,
    ID_SERVER
  };

  FXint Connect();
  void Disconnect(bool user_initiated = false);
  void SetServerName(const FXString &name) { serverName = name; }
  FXString GetServerName() { return serverName; }
  void SetServerPort(FXint port) { serverPort = port; }
  void SetNickName(const FXString &nick) { nickName = nick; }
  void SetSSLMode(bool ssl) { m_useSsl = ssl; }
  FXbool GetConnected() { return connected; }

  long OnIORead(FXObject*, FXSelector, void*);
  long OnConnected(FXObject*, FXSelector, void*);
  long OnDnsMsg(FXObject*, FXSelector, void*);

  int SendMessage(const FXString& msg);

  void AppendTarget(FXObject *tgt);
private:
  NakenClient() {}

  void SendEvent(ChatEventType);
  void SendEvent(ChatEventType, const FXString&);
  long ReadData();
  void ParseLine(const FXString &line);
  void Initialize();
  void AsyncConnect(const std::vector<FXString>& addrs);
  int SetBlocking(bool enable);


  FXApp *application;
  FXSelector selector;
  bool _initialized;
  bool m_hasSsl;
  bool connected;
  bool m_useSsl;
  FXint serverPort;
  FXString serverName, nickName;
  FXString receiveRest;
  std::vector<FXObject *> targets;
#ifdef WIN32
  SOCKET sock;
  WSAEVENT event;
#else
  int sock;
#endif
  sockaddr_in serverSock;

  DnsResolver *resolver;

  SSL_CTX *m_sslCtx;
  SSL *m_ssl;
};

#endif /* NGNC_SOCKET_H */

