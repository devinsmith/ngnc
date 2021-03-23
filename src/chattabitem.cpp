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

#include "chattabitem.h"
#include "prefs.h"

static const FXListSortFunc sortfuncs[] = {
  FXList::ascendingCase,
  FXList::descendingCase,
  FXList::ascending,
  FXList::descending
};


FXDEFMAP(ChatTabItem) ChatTabItemMap[] = {
  FXMAPFUNC(SEL_COMMAND, ChatTabItem::ID_COMMANDLINE, ChatTabItem::OnCommandline),
  FXMAPFUNC(SEL_COMMAND, NakenClient::ID_SERVER, ChatTabItem::OnChatEvent)
};

FXIMPLEMENT(ChatTabItem, FXTabItem, ChatTabItemMap, ARRAYNUMBER(ChatTabItemMap))

ChatTabItem::ChatTabItem(FXTabBook *tab, const FXString& tabtext,
    FXuint opts = TAB_TOP_NORMAL, TABTYPE typ = CHANNEL,
    NakenClient *sock = nullptr) :
  FXTabItem(tab, tabtext, nullptr, opts), parent(tab), server(sock), type(typ), chatfont(nullptr)
{
  numberUsers = 0;

  mainframe = new FXVerticalFrame(parent, FRAME_RAISED | LAYOUT_SIDE_TOP |
      LAYOUT_FILL_X | LAYOUT_FILL_Y);

  splitter = new FXSplitter(mainframe, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
      LAYOUT_FILL_Y | SPLITTER_REVERSED | SPLITTER_TRACKING);

  textframe = new FXHorizontalFrame(splitter, FRAME_SUNKEN | FRAME_THICK |
      LAYOUT_FILL_X | LAYOUT_FILL_Y);
  text = new FXText(textframe, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y |
      TEXT_READONLY | TEXT_WORDWRAP | TEXT_SHOWACTIVE | TEXT_AUTOSCROLL);

  usersframe = new FXVerticalFrame(splitter, FRAME_SUNKEN | FRAME_THICK |
      LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH);
  users = new FXList(usersframe, this, ID_USERS, LAYOUT_FILL_X |
      LAYOUT_FILL_Y);
  users->setSortFunc(sortfuncs[0]);
  users->setScrollStyle(HSCROLLING_OFF);
  if (type != CHANNEL /*|| usersHidden*/) usersframe->hide();
  if (type != CHANNEL /*|| usersHidden*/) users->hide();

  commandline = new FXTextField(mainframe, 25, this, ID_COMMANDLINE,
      TEXTFIELD_ENTER_ONLY | FRAME_SUNKEN | JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_BOTTOM, 0, 0, 0, 0, 1, 1, 1, 1);

  ChatColor& colors = Preferences::instance().colors;
  FXString fontDesc = Preferences::instance().chatFontspec;

  text->setBackColor(colors.background);
  text->setActiveBackColor(colors.background);
  text->setTextColor(colors.text);

  commandline->setBackColor(colors.background);
  commandline->setTextColor(colors.text);
  commandline->setCursorColor(colors.text);

  if (!fontDesc.empty()) {
    chatfont = new FXFont(getApp(), fontDesc);
    chatfont->create();
    text->setFont(chatfont);
    commandline->setFont(chatfont);
  }
}

ChatTabItem::~ChatTabItem()
{
  delete chatfont;
}

void ChatTabItem::UpdateFromPrefs()
{
  const ChatColor& colors = Preferences::instance().colors;
  FXString fontDesc = Preferences::instance().chatFontspec;

  SetTextColor(colors.text);
  SetTextBackColor(colors.background);

  if (!fontDesc.empty()) {
    FXFont *oldfont = chatfont;
    chatfont = new FXFont(getApp(), fontDesc);
    chatfont->create();
    text->setFont(chatfont);
    commandline->setFont(chatfont);
    delete oldfont;
  }
}

void ChatTabItem::SetTextBackColor(FXColor clr)
{
  text->setBackColor(clr);
  text->setActiveBackColor(clr);
  commandline->setBackColor(clr);
}

void ChatTabItem::SetTextColor(FXColor clr)
{
  textColor = clr;
  text->setTextColor(clr);
  commandline->setTextColor(clr);
  commandline->setCursorColor(clr);
}

void ChatTabItem::CreateGeom()
{
  mainframe->create();
  splitter->create();
  textframe->create();
  text->create();
  usersframe->create();
  users->create();
  commandline->create();
}

void ChatTabItem::SetType(const TABTYPE &typ, const FXString &tabtext)
{
  if (typ == CHANNEL) {
    /* if (!usersHidden) */ usersframe->show();
    /* if (!usersHidden) */ users->show();
    splitter->recalc();
    setText(tabtext);
//    server->SendMode(getText());
    type = typ;
  } else if(typ == SERVER || typ == QUERY) {
    usersframe->hide();
    users->hide();
    setText(tabtext);
    splitter->setSplit(1, 0);
    if(type == CHANNEL) {
      users->clearItems();
      numberUsers = 0;
    }
    type = typ;
  }
}

void ChatTabItem::ReparentTab()
{
  reparent(parent);
  mainframe->reparent(parent);
}

long ChatTabItem::OnChatEvent(FXObject *, FXSelector, void *data)
{
  ChatEvent *ev = (ChatEvent *)data;

  if (ev->eventType == CHAT_CONNECT) {
    if (type == SERVER || IsCurrent() || IsNoCurrent()) {
      AppendChatText(ev->param1);
    }
  } else if (ev->eventType == CHAT_ERROR) {
    AppendChatStyledText(ev->param1, COLOR_ERROR);
  } else if (ev->eventType == CHAT_DISCONNECT) {
    if (IsCurrent() || IsNoCurrent()) {
      AppendChatStyledText(ev->param1, COLOR_ERROR);
    }
  } else if (ev->eventType == CHAT_MSG) {
    AppendChatText(ev->param1);
  }
  return 1;
}

FXbool ChatTabItem::IsCurrent()
{
  FXint index = parent->getCurrent()*2;
  if((ChatTabItem *)parent->childAtIndex(index) == this) return true;
  return false;
}

FXbool ChatTabItem::IsNoCurrent()
{
  FXint index = parent->getCurrent()*2;
  if(((ChatTabItem *)parent->childAtIndex(index))->GetServerName() == server->GetServerName()) return false;
  return true;
}

void ChatTabItem::MakeLastRowVisible(FXbool force)
{
  if(force)
    text->makePositionVisible(text->rowStart(text->getLength()));
  else {
   FXScrollBar *textScrollbar = text->verticalScrollBar();
   if (IsCurrent()) {
     if ((textScrollbar->getPosition() + textScrollbar->getHeight() +
         textScrollbar->getLine()) * 100 > textScrollbar->getRange() * 95) {
       text->makePositionVisible(text->rowStart(text->getLength()));
     }
   } else
     text->makePositionVisible(text->rowStart(text->getLength()));
  }
}

void ChatTabItem::AppendChatText(FXString msg)
{
  text->appendText(msg);
  MakeLastRowVisible(false);
}

void ChatTabItem::AppendChatStyledText(FXString styled, FXint stylenum)
{
  text->appendStyledText(styled, stylenum);
  MakeLastRowVisible(false);
}

long ChatTabItem::OnCommandline(FXObject *, FXSelector, void *data)
{
  FXString commandtext = commandline->getText();
  if (server->GetConnected()) {
    if (!commandtext.empty()) {
      commandtext += "\r\n";
      if (server->SendMessage(commandtext) <= 0) {
        AppendChatStyledText("Failed to send message. Are you still connected?", COLOR_ERROR);
      }
    }
  } else {
    AppendChatStyledText("You aren't connected", COLOR_ERROR);
  }
  commandline->setText("");
  return 1;
}
