/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

/**************************************************************************/

RgbColor color_constant (RgbColor base, int, int)
{
  return base;
}

/**************************************************************************/

RgbColor color_varying (RgbColor base, int, int)
{
  HsbColor hsb_color (base);
  hsb_color.H += (millis () % 30000) / 30000.0;
  if (hsb_color.H >= 1)
  {
    hsb_color.H -= 1;
  }
  return RgbColor (hsb_color);
}

/**************************************************************************/

RgbColor color_rainbow (RgbColor base, int, int y)
{
  HsbColor hsb_color (base);
  hsb_color.H += y / static_cast<double> (PanelHeight);
  if (hsb_color.H >= 1)
  {
    hsb_color.H -= 1;
  }
  return RgbColor (hsb_color);
}

/**************************************************************************/

RgbColor color_random (RgbColor base, int, int)
{
  HsbColor hsb_color (base);
  hsb_color.H += ::drand48 ();
  if (hsb_color.H >= 1)
  {
    hsb_color.H -= 1;
  }
  return RgbColor (hsb_color);
}

/**************************************************************************/

RgbColor color_plasma (RgbColor base, int x, int y)
{
  auto k = millis () / 50;
  HsbColor hsb_color (base);
  hsb_color.H += (128 + inoise8 (x * 20, y * 20, k)) / 192.0;
  hsb_color.H -= (int)(hsb_color.H);
  return RgbColor (hsb_color);
}

/**************************************************************************/

const char *color_mode_names[] PROGMEM =
{
  "Constant",
  "Varying",
  "Rainbow",
  "Plasma",
  "Random",
};

using ColorModeFunction = RgbColor (*) (RgbColor, int, int);

const ColorModeFunction color_mode_functions[] PROGMEM =
{
  color_constant,
  color_varying,
  color_rainbow,
  color_plasma,
  color_random,  
};

/**************************************************************************/

ColorModeFunction color_mode (const String &name)
{
  for (ssize_t i = 0; i != std::end (color_mode_names) - std::begin (color_mode_names); ++i)
  {
    if (name == color_mode_names[i])
    {
      return color_mode_functions[i];
    }
  }
  return color_constant; 
}

/**************************************************************************/
