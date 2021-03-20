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

#include "connectdlg.h"

FXDEFMAP(ConnectDialog) ConnectDialogMap[] = {
  FXMAPFUNC(SEL_COMMAND, ConnectDialog::ID_ACCEPT, ConnectDialog::OnAccept),
  FXMAPFUNC(SEL_COMMAND, ConnectDialog::ID_CANCEL, ConnectDialog::OnCancel),
  FXMAPFUNC(SEL_COMMAND, ConnectDialog::ID_TOGGLE_SSL, ConnectDialog::ToggleSSL)
};

FXIMPLEMENT(ConnectDialog, FXDialogBox, ConnectDialogMap, ARRAYNUMBER(ConnectDialogMap))

ConnectDialog::ConnectDialog(FXMainWindow *owner) :
  FXDialogBox(owner, "Connect...", DECOR_TITLE | DECOR_BORDER,
    0,0,0,0, 0,0,0,0, 0,0)
{
  FXVerticalFrame *contents = new FXVerticalFrame(this,
      LAYOUT_SIDE_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y,
      0,0,0,0, 10,10,10,10,
      0,0);
  FXMatrix *matrix = new FXMatrix(contents, 2, MATRIX_BY_COLUMNS |
      LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);

  new FXLabel(matrix, "Hostname:", NULL, JUSTIFY_LEFT | LAYOUT_FILL_COLUMN | 
      LAYOUT_FILL_ROW);
  _hostname_field = new FXTextField(matrix, 25,NULL, 0, TEXTFIELD_ENTER_ONLY|FRAME_THICK|FRAME_SUNKEN|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW);
  _hostname_field->setText("mikekohn.net");

  new FXLabel(matrix, "", NULL, JUSTIFY_LEFT | LAYOUT_FILL_COLUMN | LAYOUT_FILL_ROW);
  _use_ssl = new FXCheckButton(matrix, "Use SSL?", this,
    ConnectDialog::ID_TOGGLE_SSL, CHECKBUTTON_NORMAL | JUSTIFY_RIGHT |
    LAYOUT_FILL_COLUMN | LAYOUT_FILL_ROW);
  _use_ssl->setCheck(TRUE);

  new FXLabel(matrix, "Port:", NULL, JUSTIFY_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW);
  _port_spinner = new FXSpinner(matrix, 23,NULL, 0, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW);
  _port_spinner->setRange(0, 65536);
  _port_spinner->setValue(6667);

  new FXLabel(matrix, "Username:", NULL, JUSTIFY_LEFT|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW);
  FXTextField *nick = new FXTextField(matrix, 25, NULL, 0, TEXTFIELD_ENTER_ONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_COLUMN|LAYOUT_FILL_ROW);
  nick->setText(FXSystem::currentUserName());

  FXHorizontalFrame *buttonframe = new FXHorizontalFrame(contents,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  new FXButton(buttonframe, "&OK", NULL, this, ConnectDialog::ID_ACCEPT, BUTTON_INITIAL|BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_X, 0,0,0,0, 32,32,5,5);
  new FXButton(buttonframe, "Cancel", NULL, this, ConnectDialog::ID_CANCEL, BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_CENTER_X, 0,0,0,0, 32,32,5,5);
}

ConnectDialog::~ConnectDialog()
{
}

long ConnectDialog::ToggleSSL(FXObject *obj, FXSelector sel, void *ud)
{
  if (_use_ssl->getCheck() == TRUE) {
    _port_spinner->setValue(6667);
  } else {
    _port_spinner->setValue(6666);
  }
  return 1;
}

long ConnectDialog::OnAccept(FXObject* obj, FXSelector sel, void* ud)
{
  if (_hostname_field->getText().trim().empty()) {
    _hostname_field->setText("");
    FXMessageBox::error(this, MBOX_OK, "Invalid parameter","Hostname must not be empty.");
    return 1;
  }

  getApp()->stopModal(this, TRUE);
  hide();
  return 1;
}

FXString ConnectDialog::GetHost()
{
  return _hostname_field->getText().trim();
}

FXint ConnectDialog::GetPort()
{
  return _port_spinner->getValue();
}

FXbool ConnectDialog::UseSSL()
{
  return _use_ssl->getCheck();
}

long ConnectDialog::OnCancel(FXObject* obj, FXSelector sel, void* ud)
{
  getApp()->stopModal(this, FALSE);
  hide();
  return 1;
}

