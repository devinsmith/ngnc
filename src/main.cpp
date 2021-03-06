/*
 * Copyright (c) 2020 Devin Smith <devin@devinsmith.net>
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

#include <fx.h>
#include "ngnc.h"

int
main(int argc, char *argv[])
{
  FXApp app("ngnc", "drsmicro");
  app.init(argc, argv);
  Preferences::instance().ReadRegistry(app.reg());

  // Set colors.
  ColorTheme& winColors = Preferences::instance().theme;

  app.setBaseColor(winColors.base);
  app.setShadowColor(makeShadowColor(winColors.base));
  app.setHiliteColor(makeHiliteColor(winColors.base));
  app.setBorderColor(winColors.border);
  app.setBackColor(winColors.back);
  app.setForeColor(winColors.fore);
  app.setSelbackColor(winColors.selback);
  app.setSelforeColor(winColors.selfore);
  app.setTipbackColor(winColors.tipback);
  app.setTipforeColor(winColors.tipfore);
  app.setSelMenuBackColor(winColors.menuback);
  app.setSelMenuTextColor(winColors.menufore);

  new ngnc(&app); // Deleted by FXTopWindow::Close
  app.create();

  app.run();
}
