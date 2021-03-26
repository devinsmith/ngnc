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
    ID_COLORS,
    ID_CHATCOLORS,
    ID_COLOR_THEME,
    ID_CHOOSE_FONT
  };

  virtual void create();

  long OnAccept(FXObject*,FXSelector,void*);
  long OnCancel(FXObject*,FXSelector,void*);
  long OnColor(FXObject*,FXSelector,void*);
  long OnChooseFont(FXObject*,FXSelector,void*);
  long OnColorTheme(FXObject*,FXSelector,void*);
  long OnColorChanged(FXObject*,FXSelector,void*);
private:
  ConfigDialog() = default;
  void setup();
  void setupColors();
  void initColors();

  ColorTheme        theme_user;     // Theme User may have set, which is different from the other themes
  FXColor           hilite;
  FXColor           shadow;

  FXDataTarget target_base;
  FXDataTarget target_back;
  FXDataTarget target_border;
  FXDataTarget target_fore;
  FXDataTarget target_hilite;
  FXDataTarget target_shadow;
  FXDataTarget target_selfore;
  FXDataTarget target_selback;
  FXDataTarget target_tipfore;
  FXDataTarget target_tipback;
  FXDataTarget target_menufore;
  FXDataTarget target_menuback;

  FXHiliteStyle textStyle[2]{};
  FXText *text{};
  FXDataTarget textTarget, backTarget, errorTarget;
  FXFont *chatfont{};
  FXButton *fontbutton{};

  FXTabItem* tabitem;
  FXVerticalFrame* tabframe;
  FXHorizontalFrame* tabsubframe;
  FXVerticalFrame* menuframe;
  FXLabel *menulabels[6];
  FXGroupBox* grpbox1;
  FXGroupBox* grpbox2;
  FXSeparator* sep1;
  FXSeparator* sep2;
  FXSeparator* sep3;
  FXHorizontalFrame *labeltextframe1;
  FXHorizontalFrame *labeltextframe2;
  FXLabel           *label1;
  FXLabel           *label2;
  FXLabel           *label3;
  FXLabel           *label4;
  FXLabel *label5;
  FXHorizontalFrame *textframe1;
  FXHorizontalFrame *textframe2;
  FXTextField* textfield1;
  FXToolTip *tooltip;

  FXListBox *list{};
};

#endif /* NGNC_PREFSDLG_H */

