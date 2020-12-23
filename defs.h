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

#ifndef NGNC_DEFS_H
#define NGNC_DEFS_H

#include <fx.h>

class NakenClient; // forward declaration.

enum TABTYPE {
  SERVER,
  CHANNEL,
  QUERY,
  OTHER
};

enum ChatEventType {
  CHAT_CONNECT,
  CHAT_DISCONNECT,
  CHAT_ERROR,
  CHAT_MSG
};

struct ChatEvent {
  ChatEventType eventType;
  NakenClient *nakenClient;
  FXString param1, param2, param3, param4;
};

enum ChatColorType {
  COLOR_NORMAL = 1,
  COLOR_ERROR = 2
};

#endif /* NGNC_DEFS_H */

