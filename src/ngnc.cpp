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
    FXMainWindow(app, "ngnc", nullptr, nullptr, DECOR_ALL, 0, 0, 800, 600)
{
  FXIcon *ICO_BIG = new FXICOIcon(app, spy_ico);
  FXIcon *ICO_SM = new FXICOIcon(app, spy_ico);

  setIcon(ICO_BIG);
  setMiniIcon(ICO_SM);

  // Initialize preferences
  //Preferences::instance().ReadRegistry(app->reg());

  menuBar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);

  // Server menu
  menuPanes[0] = new FXMenuPane(this);
  m_srv_connect = new FXMenuCommand(menuPanes[0], "Connect...", nullptr, this,
      ID_CONNECT);
  m_srv_disconnect = new FXMenuCommand(menuPanes[0], "Disconnect", nullptr,
      this, ID_DISCONNECT);
  m_srv_disconnect->disable();
  m_srv_sep = new FXMenuSeparator(menuPanes[0]);
  m_srv_quit = new FXMenuCommand(menuPanes[0], "Quit\tCtrl-Q", nullptr, this, ID_QUIT);
  menuTitle[0] = new FXMenuTitle(menuBar, "&Server", nullptr, menuPanes[0]);

  // Edit menu
  menuPanes[1] = new FXMenuPane(this);
  m_edit_pref = new FXMenuCommand(menuPanes[1], "&Preferences", nullptr, this, ID_PREFERENCES);
  menuTitle[1] = new FXMenuTitle(menuBar, "&Edit", nullptr, menuPanes[1]);

  // Help menu
  menuPanes[2] = new FXMenuPane(this);
  m_help_about = new FXMenuCommand(menuPanes[2], "&About...", nullptr, this, ID_ABOUT);
  menuTitle[2] = new FXMenuTitle(menuBar, "&Help", nullptr, menuPanes[2]);

  menuFrame = new FXVerticalFrame(this, FRAME_RAISED | LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);

  tabbook = new FXTabBook(menuFrame, this, ID_TABS, PACK_UNIFORM_WIDTH|PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_RIGHT);
  tabbook->setTabStyle(TABBOOK_BOTTOMTABS);
  FXuint packing = tabbook->getPackingHints();
  packing &= ~PACK_UNIFORM_WIDTH;
  tabbook->setPackingHints(packing);

  server = new NakenClient(app, this, 0);
  ChatTabItem *tabitem = new ChatTabItem(tabbook, "(server)", TAB_BOTTOM,
      SERVER, server);
  server->AppendTarget(tabitem);
}

ngnc::~ngnc()
{
  for (auto pane : menuPanes) {
    delete pane;
  }
}

void
ngnc::create()
{
  FXMainWindow::create();

  show(PLACEMENT_SCREEN);
}

void ngnc::SaveConfig()
{
}

long ngnc::OnCommandAbout(FXObject*, FXSelector, void*)
{
  FXDialogBox about(this, "About ngnc", DECOR_TITLE | DECOR_BORDER);
  FXVerticalFrame *content = new FXVerticalFrame(&about, LAYOUT_SIDE_LEFT |
      LAYOUT_FILL_X | LAYOUT_FILL_Y);

  new FXLabel(content, "ngnc 0.0.3", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_FILL_Y);
  new FXLabel(content, "Next Generation Naken Client (ngnc)", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X |
      LAYOUT_FILL_Y);
  new FXLabel(content, "Copyright (C) 2001 - 2021 Devin Smith (devin@devinsmith.net)", nullptr, JUSTIFY_LEFT |
  LAYOUT_FILL_X |
      LAYOUT_FILL_Y);

  // Ok button.
  FXButton *button = new FXButton(content, "OK", nullptr, &about, FXDialogBox::ID_ACCEPT, BUTTON_INITIAL |
  BUTTON_DEFAULT | FRAME_RAISED | FRAME_THICK | LAYOUT_CENTER_X, 0, 0, 0, 0, 32, 32, 5, 5);
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
    m_srv_connect->disable();
    m_srv_disconnect->enable();
  }
  return 1;
}

long ngnc::OnCommandDisconnect(FXObject*, FXSelector, void*)
{
  if (server->GetConnected()) {
    server->Disconnect(true);
  }
  m_srv_connect->enable();
  m_srv_disconnect->disable();
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
    // User clicked save, apply preferences to chat tab items.
    Retheme();
  }
  return 1;
}

void ngnc::Retheme()
{
  const auto& theme = Preferences::instance().theme;

  // Apply themes to UI.
  const auto& shadow = makeShadowColor(theme.base);
  const auto& hilite = makeHiliteColor(theme.base);

  // This will handle all windows that aren't currently visible.
  auto app = getApp();
  app->setBaseColor(theme.base);
  app->setShadowColor(shadow);
  app->setHiliteColor(hilite);
  app->setBorderColor(theme.border);
  app->setBackColor(theme.back);
  app->setForeColor(theme.fore);
  app->setSelbackColor(theme.selback);
  app->setSelforeColor(theme.selfore);
  app->setTipbackColor(theme.tipback);
  app->setTipforeColor(theme.tipfore);
  app->setSelMenuBackColor(theme.menuback);
  app->setSelMenuTextColor(theme.menufore);
  app->refresh();

  // We now need to refresh the current windows
  this->setBackColor(theme.base);

  tabbook->setShadowColor(shadow);
  tabbook->setBackColor(theme.base);
  tabbook->setBaseColor(theme.base);
  tabbook->setBorderColor(theme.border);
  tabbook->setHiliteColor(hilite);

  menuFrame->setBorderColor(theme.border);
  menuFrame->setBaseColor(theme.base);
  menuFrame->setBackColor(theme.base);
  menuFrame->setShadowColor(shadow);
  menuFrame->setHiliteColor(hilite);

  menuBar->setBorderColor(theme.border);
  menuBar->setBaseColor(theme.base);
  menuBar->setBackColor(theme.base);
  menuBar->setShadowColor(shadow);
  menuBar->setHiliteColor(hilite);

  // Theme Server menu and items.
  for (auto & i : menuTitle) {
    i->setShadowColor(shadow);
    i->setBackColor(theme.base);
    i->setSelBackColor(theme.base);
    i->setSelTextColor(theme.fore);
    i->setHiliteColor(hilite);
    i->setTextColor(theme.fore);
  }

  for (auto & i : menuPanes) {
    i->setShadowColor(shadow);
    i->setBackColor(theme.base);
    i->setBaseColor(theme.base);
    i->setBorderColor(theme.border);
    i->setHiliteColor(hilite);
  }

  m_srv_connect->setShadowColor(shadow);
  m_srv_connect->setBackColor(theme.base);
  m_srv_connect->setSelBackColor(theme.menuback);
  m_srv_connect->setSelTextColor(theme.selfore);
  m_srv_connect->setHiliteColor(hilite);
  m_srv_connect->setTextColor(theme.fore);

  m_srv_disconnect->setShadowColor(shadow);
  m_srv_disconnect->setBackColor(theme.base);
  m_srv_disconnect->setSelBackColor(theme.menuback);
  m_srv_disconnect->setSelTextColor(theme.selfore);
  m_srv_disconnect->setHiliteColor(hilite);
  m_srv_disconnect->setTextColor(theme.fore);

  m_srv_sep->setShadowColor(shadow);
  m_srv_sep->setBackColor(theme.base);
  m_srv_sep->setHiliteColor(hilite);

  m_srv_quit->setShadowColor(shadow);
  m_srv_quit->setBackColor(theme.base);
  m_srv_quit->setSelBackColor(theme.menuback);
  m_srv_quit->setSelTextColor(theme.selfore);
  m_srv_quit->setHiliteColor(hilite);
  m_srv_quit->setTextColor(theme.fore);

  m_edit_pref->setShadowColor(shadow);
  m_edit_pref->setBackColor(theme.base);
  m_edit_pref->setSelBackColor(theme.menuback);
  m_edit_pref->setSelTextColor(theme.selfore);
  m_edit_pref->setHiliteColor(hilite);
  m_edit_pref->setTextColor(theme.fore);

  m_help_about->setShadowColor(shadow);
  m_help_about->setBackColor(theme.base);
  m_help_about->setSelBackColor(theme.menuback);
  m_help_about->setSelTextColor(theme.selfore);
  m_help_about->setHiliteColor(hilite);
  m_help_about->setTextColor(theme.fore);

  for (FXint i = 0; i < tabbook->numChildren(); i = i + 2) {
    ((ChatTabItem *)tabbook->childAtIndex(i))->UpdateFromPrefs();
  }
}
