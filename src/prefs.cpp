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

#include "prefs.h"

Preferences::Preferences()
{
}

void Preferences::WriteRegistry(FXRegistry& reg)
{
  reg.writeColorEntry("SETTINGS", "chatTextColor", colors.text);
  reg.writeColorEntry("SETTINGS", "chatBackColor", colors.background);
  reg.writeColorEntry("SETTINGS", "chatErrorColor", colors.error);
  if (!chatFontspec.empty())
    reg.writeStringEntry("SETTINGS", "chatFont", chatFontspec.text());

  // Theme
  reg.writeColorEntry("SETTINGS", "basecolor", theme.base);
  reg.writeColorEntry("SETTINGS", "bordercolor", theme.border);
  reg.writeColorEntry("SETTINGS", "backcolor", theme.back);
  reg.writeColorEntry("SETTINGS", "forecolor", theme.fore);
  reg.writeColorEntry("SETTINGS", "selbackcolor", theme.selback);
  reg.writeColorEntry("SETTINGS", "selforecolor", theme.selfore);
  reg.writeColorEntry("SETTINGS", "tipbackcolor", theme.tipback);
  reg.writeColorEntry("SETTINGS", "tipforecolor", theme.tipfore);
  reg.writeColorEntry("SETTINGS", "selmenubackcolor", theme.menuback);
  reg.writeColorEntry("SETTINGS", "selmenutextcolor", theme.menufore);
}

void Preferences::ReadRegistry(FXRegistry& reg)
{
  colors.text = reg.readColorEntry("SETTINGS", "chatTextColor", FXRGB(0,0,0));
  colors.background = reg.readColorEntry("SETTINGS", "chatBackColor", FXRGB(255,255,255));
  colors.error = reg.readColorEntry("SETTINGS", "charErrorColor", FXRGB(255,0,0));
  const FXchar* fontspec = reg.readStringEntry("SETTINGS", "chatFont", nullptr);
  if (fontspec) {
    chatFontspec = fontspec;
  }

  // Theme (default to FOX standard colors)
  theme.base = reg.readColorEntry("SETTINGS", "basecolor", FXRGB(212, 208, 200));
  theme.border = reg.readColorEntry("SETTINGS", "bordercolor", FXRGB(0, 0, 0));
  theme.back = reg.readColorEntry("SETTINGS", "backcolor", FXRGB(255, 255, 255));
  theme.fore = reg.readColorEntry("SETTINGS", "forecolor", FXRGB(0, 0, 0));
  theme.selback = reg.readColorEntry("SETTINGS", "selbackcolor", FXRGB(10, 36, 106));
  theme.selfore = reg.readColorEntry("SETTINGS", "selforecolor", FXRGB(255, 255, 255));
  theme.tipback = reg.readColorEntry("SETTINGS", "tipbackcolor", FXRGB(255, 255, 225));
  theme.tipfore = reg.readColorEntry("SETTINGS", "tipforecolor", FXRGB(0, 0, 0));
  theme.menuback = reg.readColorEntry("SETTINGS", "selmenubackcolor", FXRGB(10, 36, 106));
  theme.menufore = reg.readColorEntry("SETTINGS", "selmenutextcolor", FXRGB(255, 255, 255));
}

