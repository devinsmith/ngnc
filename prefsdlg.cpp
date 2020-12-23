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

#include <fxkeys.h>

#include "defs.h"
#include "prefsdlg.h"

FXDEFMAP(ConfigDialog) ConfigDialogMap[] = {
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_ACCEPT, ConfigDialog::OnAccept),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_CANCEL, ConfigDialog::OnCancel),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_CHATCOLORS, ConfigDialog::OnColor),
  FXMAPFUNC(SEL_CHANGED, ConfigDialog::ID_CHATCOLORS, ConfigDialog::OnColor)
};

FXIMPLEMENT(ConfigDialog, FXDialogBox, ConfigDialogMap, ARRAYNUMBER(ConfigDialogMap))

ConfigDialog::ConfigDialog(FXMainWindow *owner) :
  FXDialogBox(owner, "Preferences", DECOR_RESIZE | DECOR_TITLE | DECOR_BORDER,
    0,0,0,0, 0,0,0,0, 0,0)
{
  // Setup some data targets / bindings.
  ChatColor& colors = Preferences::instance().colors;
  textTarget.connect(colors.text);
  textTarget.setTarget(this);
  textTarget.setSelector(ID_CHATCOLORS);
  backTarget.connect(colors.background);
  backTarget.setTarget(this);
  backTarget.setSelector(ID_CHATCOLORS);
  errorTarget.connect(colors.error);
  errorTarget.setTarget(this);
  errorTarget.setSelector(ID_CHATCOLORS);

  getApp()->getNormalFont()->create();
  FXFontDesc fontdescription;
  getApp()->getNormalFont()->getFontDesc(fontdescription);
  FXString defaultFont = getApp()->getNormalFont()->getFont();
  font = new FXFont(getApp(),fontdescription);
  font->create();

  // Build UI.
  FXHorizontalFrame *closeframe = new FXHorizontalFrame(this, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  FXButton *ok = new FXButton(closeframe, "&Close", NULL, this, ID_ACCEPT, BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
  ok->addHotKey(KEY_Return);
  new FXButton(closeframe, "Cancel", NULL, this, ID_CANCEL, BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);

  FXHorizontalFrame *contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXVerticalFrame *buttonframe = new FXVerticalFrame(contents, LAYOUT_FILL_Y|LAYOUT_LEFT|PACK_UNIFORM_WIDTH);
  FXSwitcher *switcher = new FXSwitcher(contents, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_RIGHT);

  FXVerticalFrame *colorpane = new FXVerticalFrame(switcher, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(colorpane, "Color settings", NULL, LAYOUT_LEFT);
  new FXHorizontalSeparator(colorpane, SEPARATOR_LINE|LAYOUT_FILL_X);
  FXHorizontalFrame *hframe = new FXHorizontalFrame(colorpane, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXMatrix *colormatrix = new FXMatrix(hframe, 2, MATRIX_BY_COLUMNS, 0,0,0,0, DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING, 1,1);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &backTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Text backround color", NULL, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &textTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Text color", NULL, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &errorTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Error text color", NULL, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  FXVerticalFrame *tframe = new FXVerticalFrame(hframe, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  text = new FXText(tframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y|TEXT_READONLY);
  text->setScrollStyle(HSCROLLING_OFF);

  // Other settings

  // Look settings
  FXVerticalFrame *lookpane = new FXVerticalFrame(switcher, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(lookpane, "Look and feel", NULL, LAYOUT_LEFT);
  new FXHorizontalSeparator(lookpane, SEPARATOR_LINE|LAYOUT_FILL_X);

  FXHorizontalFrame *fontframe = new FXHorizontalFrame(lookpane, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING);
  new FXLabel(fontframe, "Font:", NULL, LAYOUT_CENTER_Y);
  fontbutton = new FXButton(fontframe, "", NULL, this, ID_CHOOSE_FONT, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X);
  fontbutton->setText(defaultFont);


  new FXButton(buttonframe, "Co&lors", NULL, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED);
  new FXButton(buttonframe, "&Look", NULL, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED);

  for (int i = 0; i < 2; i++) {
    textStyle[i].normalForeColor = colors.text;
    textStyle[i].normalBackColor = colors.background;
    textStyle[i].selectForeColor = getApp()->getSelforeColor();
    textStyle[i].selectBackColor = getApp()->getSelbackColor();
    textStyle[i].hiliteForeColor = getApp()->getHiliteColor();
    textStyle[i].hiliteBackColor = FXRGB(255, 128, 128); // from FXText.cpp
    textStyle[i].activeBackColor = colors.background;
    textStyle[i].style = 0;
  }

  // Errors
  textStyle[1].normalForeColor = colors.error;

  text->setStyled(TRUE);
  text->setHiliteStyles(textStyle);
  text->setTextColor(colors.text);
  text->setBackColor(colors.background);
  text->appendText("[0]bill: Hello there!\n");
  text->appendText("[1]ted: Hey Now!\n");
  text->appendStyledText(FXString("No error\n"), COLOR_ERROR);
}

ConfigDialog::~ConfigDialog()
{
  delete font;
}

long ConfigDialog::OnAccept(FXObject* obj, FXSelector sel, void* ud)
{
  Preferences::instance().WriteRegistry(getApp()->reg());
  getApp()->stopModal(this, TRUE);
  hide();
  return 1;
}

long ConfigDialog::OnCancel(FXObject* obj, FXSelector sel, void* ud)
{
  getApp()->stopModal(this, FALSE);
  hide();
  return 1;
}

long ConfigDialog::OnColor(FXObject* obj, FXSelector sel, void* ud)
{
  ChatColor& colors = Preferences::instance().colors;

  text->setTextColor(colors.text);
  text->setBackColor(colors.background);
  for (int i = 0; i < 2; i++) {
    textStyle[i].normalBackColor = colors.background;
  }
  textStyle[1].normalForeColor = colors.error;
  text->update();
  return 1;
}

