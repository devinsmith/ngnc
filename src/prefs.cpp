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
  reg.writeColorEntry("SETTINGS", "textColor", colors.text);
  reg.writeColorEntry("SETTINGS", "backColor", colors.background);
  reg.writeColorEntry("SETTINGS", "errorColor", colors.error);
  if (!chatFontspec.empty())
    reg.writeStringEntry("SETTINGS", "chatFont", chatFontspec.text());
}

void Preferences::ReadRegistry(FXRegistry& reg)
{
  colors.text = reg.readColorEntry("SETTINGS", "textColor", FXRGB(0,0,0));
  colors.background = reg.readColorEntry("SETTINGS", "backColor", FXRGB(255,255,255));
  colors.error = reg.readColorEntry("SETTINGS", "errorColor", FXRGB(255,0,0));
  const FXchar* fontspec = reg.readStringEntry("SETTINGS", "chatFont", nullptr);
  if (fontspec) {
    chatFontspec = fontspec;
  }
}

