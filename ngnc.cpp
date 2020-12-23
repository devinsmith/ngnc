/*
 * Copyright (c) 2016 Devin Smith <devin@devinsmith.net>
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

#include "fx.h"
#include <FXICOIcon.h>
#include "chattabitem.h"
#include "connectdlg.h"
#include "icons.h"
#include "ngnc.h"
#include "prefsdlg.h"

FXDEFMAP(ngnc) ngncMap[] = {
  FXMAPFUNC(SEL_COMMAND, ngnc::ID_ABOUT, ngnc::OnCommandAbout),
  FXMAPFUNC(SEL_COMMAND, ngnc::ID_CONNECT, ngnc::OnCommandConnect),
  FXMAPFUNC(SEL_COMMAND, ngnc::ID_DISCONNECT, ngnc::OnCommandDisconnect),
  FXMAPFUNC(SEL_COMMAND, ngnc::ID_PREFERENCES, ngnc::OnCommandPreferences),
  FXMAPFUNC(SEL_COMMAND, ngnc::ID_QUIT, ngnc::OnCommandQuit)
};

FXIMPLEMENT(ngnc, FXMainWindow, ngncMap, ARRAYNUMBER(ngncMap))

ngnc::ngnc(FXApp *app) :
    FXMainWindow(app, "ngnc", NULL, NULL, DECOR_ALL, 0, 0, 800, 600)
{
  FXIcon *ICO_BIG = new FXICOIcon(app, spy_ico);
  FXIcon *ICO_SM = new FXICOIcon(app, spy_ico);

  setIcon(ICO_BIG);
  setMiniIcon(ICO_SM);

  Preferences::instance().ReadRegistry(app->reg());

  FXMenuBar *mb = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);

  // Server menu
  m_serverMenu = new FXMenuPane(this);
  m_connect = new FXMenuCommand(m_serverMenu, "Connect...", NULL, this,
      ID_CONNECT);
  m_disconnect = new FXMenuCommand(m_serverMenu, "Disconnect", NULL,
      this, ID_DISCONNECT);
  m_disconnect->disable();
  new FXMenuSeparator(m_serverMenu);
  new FXMenuCommand(m_serverMenu, "Quit\tCtrl-Q", NULL, this, ID_QUIT);
  new FXMenuTitle(mb, "&Server", NULL, m_serverMenu);

  // Edit menu
  m_editMenu = new FXMenuPane(this);
  new FXMenuCommand(m_editMenu, "&Preferences", NULL, this, ID_PREFERENCES);
  new FXMenuTitle(mb, "&Edit", NULL, m_editMenu);

  // Help menu
  m_helpMenu = new FXMenuPane(this);
  new FXMenuCommand(m_helpMenu, "&About...", NULL, this, ID_ABOUT);
  new FXMenuTitle(mb, "&Help", NULL, m_helpMenu);

  FXVerticalFrame *mf = new FXVerticalFrame(this, FRAME_RAISED | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);

  tabbook = new FXTabBook(mf, this, ID_TABS, PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_RIGHT);
  tabbook->setTabStyle(TABBOOK_BOTTOMTABS);
  FXuint packing = tabbook->getPackingHints();
  packing &= ~PACK_UNIFORM_WIDTH;
  tabbook->setPackingHints(packing);

  server = new NakenClient(app, this, 0);
  ChatColor& colors = Preferences::instance().colors;
  ChatTabItem *tabitem = new ChatTabItem(tabbook, "(server)", TAB_BOTTOM,
      SERVER, server, colors.text, colors.background,
      colors.error);
  server->AppendTarget(tabitem);
}

ngnc::~ngnc()
{
  delete m_serverMenu;
  delete m_editMenu;
  delete m_helpMenu;
}

void
ngnc::create()
{
  FXMainWindow::create();

  show(PLACEMENT_SCREEN);
}

void ngnc::SaveConfig()
{
  FXRegistry& reg = getApp()->reg();
  Preferences::instance().WriteRegistry(reg);
}

long ngnc::OnCommandAbout(FXObject*, FXSelector, void*)
{
  FXDialogBox about(this, "About ngnc", DECOR_TITLE | DECOR_BORDER);
  FXVerticalFrame *content = new FXVerticalFrame(&about, LAYOUT_SIDE_LEFT |
      LAYOUT_FILL_X | LAYOUT_FILL_Y);

  new FXLabel(content, "ngnc 0.0.3", 0, JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_FILL_Y);
  new FXLabel(content, "Next Generation Naken Client (ngnc)", 0, JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_FILL_Y);
  new FXLabel(content, "Copyright (C) 2001 - 2019 Devin Smith (devin@devinsmith.net)", 0, JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_FILL_Y);

  // Ok button.
  FXButton *button = new FXButton(content, "OK", 0, &about, FXDialogBox::ID_ACCEPT, BUTTON_INITIAL | BUTTON_DEFAULT | FRAME_RAISED | FRAME_THICK | LAYOUT_CENTER_X, 0, 0, 0, 0, 32, 32, 5, 5);
  button->setFocus();

  about.execute(PLACEMENT_OWNER);
  return 1;
}

long ngnc::OnCommandConnect(FXObject*, FXSelector, void*)
{
  ConnectDialog connectDlg(this);
  if (connectDlg.execute(PLACEMENT_OWNER)) {
    // User clicked connect.
    //
    if (!server->GetConnected()) {
      server->SetServerName(connectDlg.GetHost());
      server->SetServerPort(connectDlg.GetPort());
      server->SetSSLMode(connectDlg.UseSSL() == TRUE);
      server->Connect();
    }
    m_connect->disable();
    m_disconnect->enable();
  }
  return 1;
}

long ngnc::OnCommandDisconnect(FXObject*, FXSelector, void*)
{
  if (server->GetConnected()) {
    server->Disconnect(true);
  }
  m_connect->enable();
  m_disconnect->disable();
  return 1;
}

long ngnc::OnCommandQuit(FXObject*, FXSelector, void*)
{
  SaveConfig();
  getApp()->exit(0);
  return 1;
}

long ngnc::OnCommandPreferences(FXObject *, FXSelector, void*)
{
  ConfigDialog dlg(this);
  if (dlg.execute(PLACEMENT_OWNER)) {
    // User clicked save.
    ChatColor& colors = Preferences::instance().colors;

    for (FXint i = 0; i < tabbook->numChildren(); i = i + 2) {
      ((ChatTabItem *)tabbook->childAtIndex(i))->SetColor(colors.text,
        colors.background);
    }
  }
  return 1;
}
