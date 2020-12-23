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

#ifndef NGNC_TABITEM_H
#define NGNC_TABITEM_H

#include "defs.h"
#include "nakenclient.h"

class ChatTabItem : public FXTabItem {
  FXDECLARE(ChatTabItem);
public:
  ChatTabItem(FXTabBook *tab, const FXString& tabtext, FXuint opts, TABTYPE typ,
      NakenClient *sock);
  virtual ~ChatTabItem();
  enum {
    ID_COMMANDLINE = FXTabItem::ID_LAST,
    ID_USERS
  };

  void CreateGeom();
  FXString GetServerName() { return server->GetServerName(); }
  void SetType(const TABTYPE &typ, const FXString &tabText);
  TABTYPE GetType() { return type; }
  void ReparentTab();
  void MakeLastRowVisible(FXbool);

  void UpdateFromPrefs();

  FXbool IsCurrent();
  FXbool IsNoCurrent();

  long OnChatEvent(FXObject *, FXSelector, void *);
  long OnCommandline(FXObject *, FXSelector, void *);

private:
  ChatTabItem() {}
  void AppendChatText(FXString);
  void AppendChatStyledText(FXString, FXint);
  void SetTextBackColor(FXColor clr);
  void SetTextColor(FXColor clr);

  FXTabBook *parent;
  NakenClient *server;
  TABTYPE type;
  FXHorizontalFrame *textframe;
  FXVerticalFrame *usersframe, *mainframe;
  FXSplitter *splitter;
  FXText *text;
  FXList *users;
  FXTextField *commandline;
  FXint numberUsers;
  FXFont *chatfont;
};

#endif /* NGNC_TABITEM_H */

