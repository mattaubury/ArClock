/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

#include "clock_fonts.h"
#include "settings.h"

/**************************************************************************/

/*
 * Draw a single glyph
 */
void print (int &x, int &y, int digit, const Font &font, std::function<RgbColor (int, int)> color)
{
  const auto &glyph = font.glyphs[digit];
  for (int i = 0; i < glyph.width; ++i)
  {
    for (int j = 0; j < font.height; ++j)
    {
      auto ix = x + i;
      auto iy = y + j;
      if (ix >= 0 && ix < PanelWidth && iy >= 0 && iy < PanelHeight)
      {
        if (glyph.data[j] & (1 << ((glyph.width - 1) - i)))
        {
          auto pixel = topo.Map (ix, iy);
          auto text_color = color (ix, iy);
          auto effect_color = effect_state[iy][ix];
          auto addsat = [] (int a, int b) { return a + b > 255 ? 255 : a + b; };
          strip.SetPixelColor (pixel, RgbColor (
            addsat (text_color.R, effect_color.R),
            addsat (text_color.G, effect_color.G),
            addsat (text_color.B, effect_color.B)));
        }
      } 
    }
  }

  /*
   * Shift cursor position
   */
  x += glyph.width + 1;
}

/**************************************************************************/

/*
 * Generate one of the clocks (using settings from prefix "item")
 */
void clock (String item)
{
  /*
   * Position 
   */ 
  int x = settings.at (item + "X").toInt ();
  int y = settings.at (item + "Y").toInt ();

  /*
   * Font 
   */
  const auto &font = clock_font (settings.at (item + "Font"));

  /*
   * Colour generating function
   */
  HtmlColor html_color;
  html_color.Parse<HtmlColorNames> (settings.at (item + "Color"));
  auto base = RgbColor (html_color);
  auto color_function = color_mode (settings.at (item + "ColorMode"));
  std::function<RgbColor (int, int)> color =
  [&] (int x, int y)
  {
    return color_function (base, x, y);
  };
      
  /*
   * Print each item
   */
  for (auto format : settings.at (item + "Format"))
  {
    switch (format)
    {
      case 'Y':
        print (x, y, year / 1000, font, color);
        print (x, y, (year / 100) % 10, font, color);
        print (x, y, (year / 10) % 10, font, color);
        print (x, y, year % 10, font, color);
        break;
        
      case 'y':
        print (x, y, (year / 10) % 10, font, color);
        print (x, y, year % 10, font, color);
        break;
        
      case 'm':
        print (x, y, (month / 10) % 10, font, color);
        print (x, y, month % 10, font, color);
        break;
        
      case 'd':
        print (x, y, (day / 10) % 10, font, color);
        print (x, y, day % 10, font, color);
        break;
        
      case 'H': /* 24-hour */
        print (x, y, hours / 10, font, color);
        print (x, y, hours % 10, font, color);
        break;

      case 'h': /* 12-hour */
        if (hours > 12) /* PM */
        {
          hours -= 12;
        }
        if (hours == 0) /* 12 AM */
        {
          hours = 12;
        }
        if (hours < 10)
        {
          print (x, y, BLANK, font, color);
        }
        else
        {
          print (x, y, hours / 10, font, color);
        }
        print (x, y, hours % 10, font, color);
        break;

      case 'M':
        print (x, y, minutes / 10, font, color);
        print (x, y, minutes % 10, font, color);
        break;

      case 'S':
        print (x, y, seconds / 10, font, color);
        print (x, y, seconds % 10, font, color);
        break;

      case ':':
        print (x, y, COLON, font, color);
        break;

      case '.':
        print (x, y, PERIOD, font, color);
        break;

      case '\'':
      case '*': /* for iOS */
        print (x, y, DEGREES, font, color);
        break;

      case '-':
        print (x, y, MINUS, font, color);
        break;

      case 'p':
        if (ping_time < 1000)
        {
          print (x, y, ping_time >= 100 ? ping_time / 100 : BLANK, font, color);
          print (x, y, ping_time >= 10 ? (ping_time / 10) % 10 : BLANK, font, color);
          print (x, y, (ping_time % 10), font, color);
        }
        break;

      case 'C':
      case 'F':
        if (celsius == -99)
        {
          /*
           * No current value
           */
          print (x, y, MINUS, font, color);
          print (x, y, MINUS, font, color);
        }
        else
        {
          auto temp = static_cast<int> (round (format == 'C' ? celsius : 32 + ((celsius *9) / 5)));
          if (temp >= 0 && temp < 10)
          {
            print (x, y, BLANK, font, color);
          }
          if (temp < 0) 
          {
            print (x, y, MINUS, font, color);
            temp = -temp;
          }
          if (temp >= 100)
          {
            print (x, y, temp / 100, font, color);
          }
          if (temp >= 10)
          {
            print (x, y, (temp / 10) % 10, font, color);
          }
          print (x, y, temp % 10, font, color);
        }
        break; 
        
      case ' ':
        ++x;
        break;
    }
  }
}

/**************************************************************************/
