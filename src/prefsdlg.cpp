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

const ColorTheme ColorThemes[]={
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"FOX"               ,FXRGB(212,208,200),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 10, 36,106),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 10, 36,106),FXRGB(255,255,255)},
  {"Gnome"             ,FXRGB(214,215,214),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255)},
  {"Atlas Green"       ,FXRGB(175,180,159),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(111,122, 99),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(111,122, 99),FXRGB(255,255,255)},
  {"BeOS"              ,FXRGB(217,217,217),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(168,168,168),FXRGB(  0,  0,  0),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(168,168,168),FXRGB(  0,  0,  0)},
  {"Blue Slate"        ,FXRGB(239,239,239),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255)},
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"CDE"	       ,FXRGB(156,153,156),FXRGB(  0,  0,  0),FXRGB(131,129,131),FXRGB(255,255,255),FXRGB( 49, 97,131),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 49, 97,131),FXRGB(255,255,255)},
  {"Digital CDE"       ,FXRGB( 74,121,131),FXRGB(  0,  0,  0),FXRGB( 55, 77, 78),FXRGB(255,255,255),FXRGB( 82,102,115),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 82,102,115),FXRGB(255,255,255)},
  {"Dark Blue"         ,FXRGB( 66,103,148),FXRGB(  0,  0,  0),FXRGB(  0, 42, 78),FXRGB(255,255,255),FXRGB( 92,179,255),FXRGB(  0,  0,  0),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 92,179,255),FXRGB(  0,  0,  0)},
  {"Desert FOX"        ,FXRGB(214,205,187),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(128,  0,  0),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(128,  0,  0),FXRGB(255,255,255)},
  {"EveX"              ,FXRGB(230,222,220),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 10, 95,137),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 10, 95,137),FXRGB(255,255,255)},
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"Galaxy"            ,FXRGB(239,239,239),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255)},
  {"iMac"              ,FXRGB(205,206,205),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255)},
  {"KDE 1"             ,FXRGB(192,192,192),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255)},
  {"KDE 2"             ,FXRGB(220,220,220),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 10, 95,137),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 10, 95,137),FXRGB(255,255,255)},
  {"KDE 3"             ,FXRGB(238,238,230),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(255,221,118),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(255,221,118),FXRGB(255,255,255)},
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"Keramik"           ,FXRGB(234,233,232),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(169,209,255),FXRGB(  0,  0,  0),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(169,209,255),FXRGB(  0,  0,  0)},
  {"Keramik Emerald"   ,FXRGB(238,238,230),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(134,204,134),FXRGB(  0,  0,  0),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(134,204,134),FXRGB(  0,  0,  0)},
  {"Keramik White"     ,FXRGB(239,239,239),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255)},
  {"Mandrake"          ,FXRGB(230,231,230),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 33, 68,156),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 33, 68,156),FXRGB(255,255,255)},
  {"Media Peach"       ,FXRGB(239,239,239),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255)},
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"Next"              ,FXRGB(168,168,168),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,  0),FXRGB(255,255,255)},
  {"Pale Gray"         ,FXRGB(214,214,214),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,  0),FXRGB(255,255,255)},
  {"Plastik"           ,FXRGB(239,239,239),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(103,141,178),FXRGB(255,255,255)},
  {"Pumpkin"           ,FXRGB(238,216,174),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(205,133, 63),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(205,133, 63),FXRGB(255,255,255)},
  {"Redmond 95"        ,FXRGB(195,195,195),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0,  0,128),FXRGB(255,255,255)},
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
//|        Name        |        Base      |       Border     |       Back       |      Fore        |      Selback     |      Selfore     |      Tipback     |     Tipfore      |      Menuback    |      Menufore    |
//|--------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------|
  {"Redmond 2000"      ,FXRGB(212,208,200),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(  0, 36,104),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(  0, 36,104),FXRGB(255,255,255)},
  {"Redmond XP"        ,FXRGB(238,238,230),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 74,121,205),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 74,121,205),FXRGB(255,255,255)},
  {"Solaris"           ,FXRGB(174,178,195),FXRGB(  0,  0,  0),FXRGB(147,151,165),FXRGB(  0,  0,  0),FXRGB(113,139,165),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(113,139,165),FXRGB(255,255,255)},
  {"Storm"             ,FXRGB(192,192,192),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB(139,  0,139),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB(139,  0,139),FXRGB(255,255,255)},
  {"Sea Sky"           ,FXRGB(165,178,198),FXRGB(  0,  0,  0),FXRGB(255,255,255),FXRGB(  0,  0,  0),FXRGB( 49,101,156),FXRGB(255,255,255),FXRGB(255,255,225),FXRGB(  0,  0,  0),FXRGB( 49,101,156),FXRGB(255,255,255)},
};

const FXint numThemes=ARRAYNUMBER(ColorThemes);

FXDEFMAP(ConfigDialog) ConfigDialogMap[] = {
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_ACCEPT, ConfigDialog::OnAccept),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_CANCEL, ConfigDialog::OnCancel),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_CHATCOLORS, ConfigDialog::OnColor),
  FXMAPFUNC(SEL_CHANGED, ConfigDialog::ID_CHATCOLORS, ConfigDialog::OnColor),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_COLORS, ConfigDialog::OnColorChanged),
  FXMAPFUNC(SEL_CHANGED, ConfigDialog::ID_COLORS, ConfigDialog::OnColorChanged),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_CHOOSE_FONT, ConfigDialog::OnChooseFont),
  FXMAPFUNC(SEL_COMMAND, ConfigDialog::ID_COLOR_THEME, ConfigDialog::OnColorTheme)
};

FXIMPLEMENT(ConfigDialog, FXDialogBox, ConfigDialogMap, ARRAYNUMBER(ConfigDialogMap))

ConfigDialog::ConfigDialog(FXMainWindow *owner) :
  FXDialogBox(owner, "Preferences", DECOR_RESIZE | DECOR_TITLE | DECOR_BORDER,
    0,0,0,0, 0,0,0,0, 0,0)
{
  tooltip = new FXToolTip(getApp());

  hilite = getApp()->getHiliteColor();
  shadow = getApp()->getShadowColor();

  ColorTheme& theme_current = Preferences::instance().theme;

  target_base.connect(theme_current.base);
  target_back.connect(theme_current.back);
  target_border.connect(theme_current.border);
  target_fore.connect(theme_current.fore);
  target_hilite.connect(hilite);
  target_shadow.connect(shadow);
  target_selfore.connect(theme_current.selfore);
  target_selback.connect(theme_current.selback);
  target_tipfore.connect(theme_current.tipfore);
  target_tipback.connect(theme_current.tipback);
  target_menufore.connect(theme_current.menufore);
  target_menuback.connect(theme_current.menuback);

  target_base.setTarget(this);
  target_back.setTarget(this);
  target_border.setTarget(this);
  target_fore.setTarget(this);
  target_hilite.setTarget(this);
  target_shadow.setTarget(this);
  target_selfore.setTarget(this);
  target_selback.setTarget(this);
  target_tipfore.setTarget(this);
  target_tipback.setTarget(this);
  target_menufore.setTarget(this);
  target_menuback.setTarget(this);

  target_base.setSelector(ID_COLORS);
  target_back.setSelector(ID_COLORS);
  target_border.setSelector(ID_COLORS);
  target_fore.setSelector(ID_COLORS);
  target_hilite.setSelector(ID_COLORS);
  target_shadow.setSelector(ID_COLORS);
  target_selfore.setSelector(ID_COLORS);
  target_selback.setSelector(ID_COLORS);
  target_tipfore.setSelector(ID_COLORS);
  target_tipback.setSelector(ID_COLORS);
  target_menufore.setSelector(ID_COLORS);
  target_menuback.setSelector(ID_COLORS);

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

  setup();
}

void ConfigDialog::setup()
{
  FXString fontDesc = Preferences::instance().chatFontspec;

  if (!fontDesc.empty()) {
    chatfont = new FXFont(getApp(), fontDesc);
  } else {
    getApp()->getNormalFont()->create();

    FXFontDesc fontdescription;
    getApp()->getNormalFont()->getFontDesc(fontdescription);
    chatfont = new FXFont(getApp(), fontdescription);
  }
  chatfont->create();
  FXString defaultFont = chatfont->getFont();

  // Build UI.
  FXHorizontalFrame *closeframe = new FXHorizontalFrame(this, LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X);
  FXButton *ok = new FXButton(closeframe, "&Close", nullptr, this, ID_ACCEPT,
                              BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
  ok->addHotKey(KEY_Return);
  new FXButton(closeframe, "Cancel", nullptr, this, ID_CANCEL, BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK, 0,0,0,
               0, 20,20);

  FXHorizontalFrame *contents = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXVerticalFrame *buttonframe = new FXVerticalFrame(contents, LAYOUT_FILL_Y|LAYOUT_LEFT|PACK_UNIFORM_WIDTH);
  FXSwitcher *switcher = new FXSwitcher(contents, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_RIGHT);

  FXVerticalFrame *colorpane = new FXVerticalFrame(switcher, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXLabel(colorpane, "Color settings", nullptr, LAYOUT_LEFT);
  new FXHorizontalSeparator(colorpane, SEPARATOR_LINE|LAYOUT_FILL_X);
  FXHorizontalFrame *hframe = new FXHorizontalFrame(colorpane, LAYOUT_FILL_X|LAYOUT_FILL_Y);
  FXMatrix *colormatrix = new FXMatrix(hframe, 2, MATRIX_BY_COLUMNS, 0,0,0,0, DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING, 1,1);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &backTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Text backround color", nullptr, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &textTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Text color", nullptr, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  new FXColorWell(colormatrix, FXRGB(0,0,255), &errorTarget, FXDataTarget::ID_VALUE, COLORWELL_OPAQUEONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_LEFT|LAYOUT_CENTER_Y|LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW, 0,0,40,24);
  new FXLabel(colormatrix, "Error text color", nullptr, JUSTIFY_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_ROW);

  FXVerticalFrame *tframe = new FXVerticalFrame(hframe, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  text = new FXText(tframe, nullptr, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y|TEXT_READONLY);
  text->setScrollStyle(HSCROLLING_OFF);

  // Other settings

  // Theme settings
  FXVerticalFrame *vframe = new FXVerticalFrame(switcher, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0,0,0,0,0,0,0);
  hframe = new FXHorizontalFrame(vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0,0,0,0,0,0,0);
  new FXSeparator(vframe, SEPARATOR_GROOVE | LAYOUT_FILL_X);

  FXVerticalFrame *frame = new FXVerticalFrame(hframe, LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0, 0, 0);
  new FXSeparator(hframe,SEPARATOR_GROOVE|LAYOUT_FILL_Y);
  FXVerticalFrame * themeframe = new FXVerticalFrame(frame,LAYOUT_FILL_X,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING);
  new FXLabel(themeframe,"Theme: ", nullptr,LAYOUT_CENTER_Y);
  list = new FXListBox(themeframe,this,ID_COLOR_THEME,LAYOUT_FILL_X|FRAME_SUNKEN|FRAME_THICK);
  list->setNumVisible(9);

  new FXSeparator(frame,SEPARATOR_GROOVE|LAYOUT_FILL_X);

  FXMatrix* matrix = new FXMatrix(frame,2,LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,
                                  DEFAULT_SPACING,DEFAULT_SPACING,1,1);

  new FXColorWell(matrix,FXRGB(0,0,255),&target_base,FXDataTarget::ID_VALUE);
  //FXLabel *label = new FXLabel(matrix,"Base Color");
  new FXLabel(matrix,"Base Color");
  new FXColorWell(matrix,FXRGB(0,0,255),&target_border,FXDataTarget::ID_VALUE);
  //label = new FXLabel(matrix,"Border Color");
  new FXLabel(matrix,"Border Color");
  new FXColorWell(matrix,FXRGB(0,0,255),&target_fore,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Text Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_back,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Background Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_selfore,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Selected Text Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_selback,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Selected Background Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_menufore,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Selected Menu Text Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_menuback,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Selected Menu Background Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_tipfore,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Tip Text Color");

  new FXColorWell(matrix,FXRGB(0,0,255),&target_tipback,FXDataTarget::ID_VALUE);
  new FXLabel(matrix,"Tip Background Color");

  frame = new FXVerticalFrame(hframe,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,0,0);
  FXTabBook* tabbook = new FXTabBook(frame, nullptr,0,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0);
  tabitem = new FXTabItem(tabbook, " Item 1 ");
  tabframe = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_THICK|FRAME_RAISED);

  labeltextframe1 = new FXHorizontalFrame(tabframe,LAYOUT_FILL_X);
  label1 = new FXLabel(labeltextframe1,"Label with Text", nullptr);
  textfield1 = new FXTextField(labeltextframe1,30,NULL,0,LAYOUT_FILL_X|FRAME_THICK|FRAME_SUNKEN);
  textfield1->setText("Select this text, to see the selected colors");

  labeltextframe2 = new FXHorizontalFrame(tabframe,LAYOUT_FILL_X);
  textframe1 = new FXHorizontalFrame(labeltextframe2,LAYOUT_FILL_X|FRAME_THICK|FRAME_SUNKEN,0,0,0,
                                                        0,2,2,2,2,0,0);
  label3 = new FXLabel(textframe1,"Selected Text (with focus)",NULL,LAYOUT_FILL_X,0,0,0,0,1,1,1,1);
  textframe2 = new FXHorizontalFrame(labeltextframe2,LAYOUT_FILL_X|FRAME_THICK|FRAME_SUNKEN,0,0,0,
                                                        0,2,2,2,2,0,0);
  label4 = new FXLabel(textframe2,"Selected Text (no focus)",NULL,LAYOUT_FILL_X,0,0,0,0,1,1,1,1);

  sep1 = new FXSeparator(tabframe,LAYOUT_FILL_X|SEPARATOR_LINE);
  tabsubframe = new FXHorizontalFrame(tabframe,LAYOUT_FILL_X|LAYOUT_FILL_Y);

  grpbox1 = new FXGroupBox(tabsubframe,"MenuPane",FRAME_GROOVE|LAYOUT_FILL_Y|LAYOUT_FILL_X);

  menuframe = new FXVerticalFrame(grpbox1,FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_X|LAYOUT_CENTER_Y,0,
                                                   0,0,0,0,0,
                                                   0,0,0,0);
  menulabels[0]=new FXLabel(menuframe,"&Open",NULL,LABEL_NORMAL,0,0,0,0,16,4);
  menulabels[1]=new FXLabel(menuframe,"S&ave",NULL,LABEL_NORMAL,0,0,0,0,16,4);
  sep2 = new FXSeparator(menuframe,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  menulabels[2]=new FXLabel(menuframe,"I&mport",NULL,LABEL_NORMAL,0,0,0,0,16,4);
  menulabels[4]=new FXLabel(menuframe,"Selected Menu Entry",NULL,LABEL_NORMAL,0,0,0,0,16,4);
  menulabels[3]=new FXLabel(menuframe,"Print",NULL,LABEL_NORMAL,0,0,0,0,16,4);
  sep3 = new FXSeparator(menuframe,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  menulabels[5]=new FXLabel(menuframe,"&Quit",NULL,LABEL_NORMAL,0,0,0,0,16,4);

  grpbox2 = new FXGroupBox(tabsubframe,"Tooltips",FRAME_GROOVE|LAYOUT_FILL_Y|LAYOUT_FILL_X);

  label2 = new FXLabel(grpbox2,"Sample Tooltip",NULL,FRAME_LINE|LAYOUT_CENTER_X);
  label5 = new FXLabel(grpbox2,"Multiline Sample\n Tooltip",NULL,FRAME_LINE|LAYOUT_CENTER_X);



  FXHorizontalFrame *fontframe = new FXHorizontalFrame(vframe, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING,DEFAULT_SPACING);
  new FXLabel(fontframe, "Chat Font:", nullptr, LAYOUT_CENTER_Y);
  fontbutton = new FXButton(fontframe, "", nullptr, this, ID_CHOOSE_FONT, LAYOUT_CENTER_Y | FRAME_RAISED | JUSTIFY_CENTER_X | JUSTIFY_CENTER_Y | LAYOUT_FILL_X);
  fontbutton->setText(defaultFont);


  new FXButton(buttonframe, "Co&lors", nullptr, switcher, FXSwitcher::ID_OPEN_FIRST, FRAME_RAISED);
  new FXButton(buttonframe, "&Themes", nullptr, switcher, FXSwitcher::ID_OPEN_SECOND, FRAME_RAISED);

  ChatColor& colors = Preferences::instance().colors;

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

  setupColors();
}

ConfigDialog::~ConfigDialog()
{
  delete chatfont;
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

long ConfigDialog::OnChooseFont(FXObject* obj, FXSelector sel, void *ud)
{
  FXFontDialog dialog(this, "Select Chat Font");
  FXFontDesc fontdescription;
  chatfont->getFontDesc(fontdescription);
  strncpy(fontdescription.face, chatfont->getActualName().text(),
      sizeof(fontdescription.face) - 1);
  fontdescription.face[sizeof(fontdescription.face) - 1] = '\0';

  dialog.setFontSelection(fontdescription);
  if (dialog.execute(PLACEMENT_SCREEN)) {
    FXFont *oldfont = chatfont;

    dialog.getFontSelection(fontdescription);
    chatfont = new FXFont(getApp(), fontdescription);
    chatfont->create();
    delete oldfont;

    FXString newDesc = chatfont->getFont();
    fontbutton->setText(newDesc);

    // Save to preferences
    Preferences::instance().chatFontspec = newDesc;
  }
  return 1;
}

void ConfigDialog::create()
{
  FXDialogBox::create();
  initColors();
}

void ConfigDialog::setupColors()
{
  ColorTheme& theme_current = Preferences::instance().theme;

  shadow = makeShadowColor(theme_current.base);
  hilite = makeHiliteColor(theme_current.base);

  tabitem->setBorderColor(theme_current.border);
  tabitem->setBaseColor(theme_current.base);
  tabitem->setBackColor(theme_current.base);
  tabitem->setTextColor(theme_current.fore);
  tabitem->setShadowColor(shadow);
  tabitem->setHiliteColor(hilite);

  tabframe->setBorderColor(theme_current.border);
  tabframe->setBaseColor(theme_current.base);
  tabframe->setBackColor(theme_current.base);
  tabframe->setShadowColor(shadow);
  tabframe->setHiliteColor(hilite);


  tabsubframe->setBorderColor(theme_current.border);
  tabsubframe->setBaseColor(theme_current.base);
  tabsubframe->setBackColor(theme_current.base);
  tabsubframe->setShadowColor(shadow);
  tabsubframe->setHiliteColor(hilite);

  menuframe->setBorderColor(theme_current.border);
  menuframe->setBaseColor(theme_current.base);
  menuframe->setBackColor(theme_current.base);
  menuframe->setShadowColor(shadow);
  menuframe->setHiliteColor(hilite);

  grpbox1->setBorderColor(theme_current.border);
  grpbox1->setBaseColor(theme_current.base);
  grpbox1->setBackColor(theme_current.base);
  grpbox1->setShadowColor(shadow);
  grpbox1->setHiliteColor(hilite);
  grpbox1->setTextColor(theme_current.fore);

  grpbox2->setBorderColor(theme_current.border);
  grpbox2->setBaseColor(theme_current.base);
  grpbox2->setBackColor(theme_current.base);
  grpbox2->setShadowColor(shadow);
  grpbox2->setHiliteColor(hilite);
  grpbox2->setTextColor(theme_current.fore);

  sep1->setBorderColor(theme_current.border);
  sep1->setBaseColor(theme_current.base);
  sep1->setBackColor(theme_current.base);
  sep1->setShadowColor(shadow);
  sep1->setHiliteColor(hilite);

  sep2->setBorderColor(theme_current.border);
  sep2->setBaseColor(theme_current.base);
  sep2->setBackColor(theme_current.base);
  sep2->setShadowColor(shadow);
  sep2->setHiliteColor(hilite);

  sep3->setBorderColor(theme_current.border);
  sep3->setBaseColor(theme_current.base);
  sep3->setBackColor(theme_current.base);
  sep3->setShadowColor(shadow);
  sep3->setHiliteColor(hilite);

  labeltextframe1->setBorderColor(theme_current.border);
  labeltextframe1->setBaseColor(theme_current.base);
  labeltextframe1->setBackColor(theme_current.base);
  labeltextframe1->setShadowColor(shadow);
  labeltextframe1->setHiliteColor(hilite);


  labeltextframe2->setBorderColor(theme_current.border);
  labeltextframe2->setBaseColor(theme_current.base);
  labeltextframe2->setBackColor(theme_current.base);
  labeltextframe2->setShadowColor(shadow);
  labeltextframe2->setHiliteColor(hilite);

  label1->setBorderColor(theme_current.border);
  label1->setBaseColor(theme_current.base);
  label1->setBackColor(theme_current.base);
  label1->setTextColor(theme_current.fore);
  label1->setShadowColor(shadow);
  label1->setHiliteColor(hilite);

  label2->setBorderColor(theme_current.tipfore);
  label2->setBaseColor(theme_current.tipback);
  label2->setBackColor(theme_current.tipback);
  label2->setTextColor(theme_current.tipfore);
  label2->setShadowColor(shadow);
  label2->setHiliteColor(hilite);

  label3->setBorderColor(theme_current.border);
  label3->setBaseColor(theme_current.base);
  label3->setBackColor(theme_current.selback);
  label3->setTextColor(theme_current.selfore);
  label3->setShadowColor(shadow);
  label3->setHiliteColor(hilite);

  label4->setBorderColor(theme_current.border);
  label4->setBaseColor(theme_current.base);
  label4->setBackColor(theme_current.base);
  label4->setTextColor(theme_current.fore);
  label4->setShadowColor(shadow);
  label4->setHiliteColor(hilite);

  label5->setBorderColor(theme_current.tipfore);
  label5->setBaseColor(theme_current.tipback);
  label5->setBackColor(theme_current.tipback);
  label5->setTextColor(theme_current.tipfore);
  label5->setShadowColor(shadow);
  label5->setHiliteColor(hilite);

  for (int i=0;i<6;i++){
    menulabels[i]->setBorderColor(theme_current.border);
    menulabels[i]->setBaseColor(theme_current.base);
    menulabels[i]->setBackColor(theme_current.base);
    menulabels[i]->setTextColor(theme_current.fore);
    menulabels[i]->setShadowColor(shadow);
    menulabels[i]->setHiliteColor(hilite);
  }

  menulabels[4]->setBorderColor(theme_current.border);
  menulabels[4]->setBaseColor(theme_current.menuback);
  menulabels[4]->setBackColor(theme_current.menuback);
  menulabels[4]->setTextColor(theme_current.menufore);
  menulabels[4]->setShadowColor(shadow);
  menulabels[4]->setHiliteColor(hilite);

  textframe1->setBorderColor(theme_current.border);
  textframe1->setBaseColor(theme_current.base);
  textframe1->setBackColor(theme_current.back);
  textframe1->setShadowColor(shadow);
  textframe1->setHiliteColor(hilite);

  textframe2->setBorderColor(theme_current.border);
  textframe2->setBaseColor(theme_current.base);
  textframe2->setBackColor(theme_current.back);
  textframe2->setShadowColor(shadow);
  textframe2->setHiliteColor(hilite);

  textfield1->setBorderColor(theme_current.border);
  textfield1->setBackColor(theme_current.back);
  textfield1->setBaseColor(theme_current.base);
  textfield1->setTextColor(theme_current.fore);
  textfield1->setSelTextColor(theme_current.selfore);
  textfield1->setSelBackColor(theme_current.selback);
  textfield1->setCursorColor(theme_current.fore);
  textfield1->setShadowColor(shadow);
  textfield1->setHiliteColor(hilite);

  tooltip->setTextColor(theme_current.tipfore);
  tooltip->setBackColor(theme_current.tipback);
}

void ConfigDialog::initColors()
{
  FXint i,scheme=-1;

  ColorTheme& theme_current = Preferences::instance().theme;

  /// Find the correct current scheme
  for(i=0;i<numThemes;i++){
    if((theme_current.base==ColorThemes[i].base) &&
       (theme_current.border==ColorThemes[i].border) &&
       (theme_current.back==ColorThemes[i].back) &&
       (theme_current.fore==ColorThemes[i].fore) &&
       (theme_current.selfore==ColorThemes[i].selfore) &&
       (theme_current.selback==ColorThemes[i].selback) &&
       (theme_current.menufore==ColorThemes[i].menufore) &&
       (theme_current.menuback==ColorThemes[i].menuback) &&
       (theme_current.tipfore==ColorThemes[i].tipfore) &&
       (theme_current.tipback==ColorThemes[i].tipback)){
      scheme=i;
      break;
    }
  }

  if (scheme==-1) {
    theme_user.base = theme_current.base;
    theme_user.border = theme_current.border;
    theme_user.back = theme_current.back;
    theme_user.fore = theme_current.fore;
    theme_user.selfore = theme_current.selfore;
    theme_user.selback = theme_current.selback;
    theme_user.menufore = theme_current.menufore;
    theme_user.menuback = theme_current.menuback;
    theme_user.tipfore = theme_current.tipfore;
    theme_user.tipback = theme_current.tipback;
    list->appendItem("Current",NULL,&theme_user);
  }

  /// Add Standard Themes to List
  for(i=0; i<numThemes; i++){
    list->appendItem(ColorThemes[i].name,NULL,(void*)&ColorThemes[i]);
  }
  list->appendItem("User Defined");
  list->setCurrentItem(scheme);

}

long ConfigDialog::OnColorTheme(FXObject *, FXSelector, void *ptr)
{
  FXint no=(FXint)(FXival)ptr;
  ColorTheme * theme_selected = reinterpret_cast<ColorTheme*>(list->getItemData(no));
  ColorTheme& theme_current = Preferences::instance().theme;

  if (theme_selected) {
    /// Set new colors from selected theme
    theme_current.base     = theme_selected->base;
    theme_current.border   = theme_selected->border;
    theme_current.back     = theme_selected->back;
    theme_current.fore     = theme_selected->fore;
    theme_current.selfore  = theme_selected->selfore;
    theme_current.selback  = theme_selected->selback;
    theme_current.tipfore  = theme_selected->tipfore;
    theme_current.tipback  = theme_selected->tipback;
    theme_current.menufore = theme_selected->menufore;
    theme_current.menuback = theme_selected->menuback;

    /// Apply New Colors to Widgets
    setupColors();
  }
  return 1;

}

long ConfigDialog::OnColorChanged(FXObject *, FXSelector, void *)
{
  list->setCurrentItem(list->getNumItems()-1);
  setupColors();
  return 1;
}
