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

#ifndef NGNC_PREFS_H
#define NGNC_PREFS_H

#include <fx.h>

struct ChatColor {
  FXColor text;
  FXColor background;
  FXColor error;
};

struct ColorTheme {
  const FXchar* name;
  FXColor       base;
  FXColor       border;
  FXColor       back;
  FXColor       fore;
  FXColor       selback;
  FXColor       selfore;
  FXColor       tipback;
  FXColor       tipfore;
  FXColor       menuback;
  FXColor       menufore;
};

class Preferences {
public:
  static Preferences& instance()
  {
    static Preferences inst;

    return inst;
  }

  FXString chatFontspec;
  ChatColor colors;
  ColorTheme theme;

  void WriteRegistry(FXRegistry& reg);
  void ReadRegistry(FXRegistry& reg);
private:
  Preferences();
  Preferences(Preferences const&);
  void operator=(Preferences const&);
};

#endif /* NGNC_PREFS_H */

