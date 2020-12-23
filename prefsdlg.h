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

#ifndef NGNC_PREFSDLG_H
#define NGNC_PREFSDLG_H

#include "prefs.h"

class ConfigDialog : public FXDialogBox {
  FXDECLARE(ConfigDialog);
public:
  ConfigDialog(FXMainWindow *owner);
  virtual ~ConfigDialog();
  enum {
    ID_ACCEPT = FXTopWindow::ID_LAST,
    ID_CANCEL,
    ID_CHATCOLORS,
    ID_CHOOSE_FONT
  };

  long OnAccept(FXObject*,FXSelector,void*);
  long OnCancel(FXObject*,FXSelector,void*);
  long OnColor(FXObject*,FXSelector,void*);
  long OnChooseFont(FXObject*,FXSelector,void*);
private:
  ConfigDialog() {}
  void WriteRegistry();

  FXHiliteStyle textStyle[2];
  FXText *text;
  FXDataTarget textTarget, backTarget, errorTarget;
  FXFont *chatfont;
  FXButton *fontbutton;
};

#endif /* NGNC_PREFSDLG_H */

