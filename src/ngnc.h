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

#ifndef NGNC_H
#define NGNC_H

#include "defs.h"
#include "prefs.h"

/* NextGen Naken Client (ngnc) */

class ngnc : public FXMainWindow {
  FXDECLARE(ngnc)
public:
  ngnc(FXApp *app);
  virtual ~ngnc();
  enum {
    ID_ABOUT = FXMainWindow::ID_LAST,
    ID_QUIT,
    ID_CONNECT,
    ID_DISCONNECT,
    ID_PREFERENCES,
    ID_TABS
  };

  void create();

  long OnCommandAbout(FXObject*, FXSelector, void*);
  long OnCommandConnect(FXObject*, FXSelector, void*);
  long OnCommandDisconnect(FXObject*, FXSelector, void*);
  long OnCommandPreferences(FXObject*, FXSelector, void*);
  long OnCommandQuit(FXObject*, FXSelector, void*);
private:
  ngnc() {}
  void SaveConfig();
  void SortTabs();
  void Retheme();

  FXTabBook *tabbook;

  FXMenuPane *menuPanes[3];
  FXMenuTitle *menuTitle[3];

  FXVerticalFrame* menuFrame;
  FXMenuBar* menuBar;

  // Server menu commands
  FXMenuCommand *m_srv_connect;
  FXMenuCommand *m_srv_disconnect;
  FXMenuSeparator *m_srv_sep;
  FXMenuCommand *m_srv_quit;

  // Edit
  FXMenuCommand *m_edit_pref;

  // Help
  FXMenuCommand *m_help_about;


  NakenClient *server;
};

#endif /* NGNC_H */

