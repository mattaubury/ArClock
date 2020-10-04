/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

/**************************************************************************/

/*
 * Crufty generative WiFi logo
 */
void wifi_logo (RgbColor base)
{
  auto time = millis ();
  for (int i = 0; i != PanelWidth; ++i)
  {
    for (int j = 0; j != PanelHeight; ++j)
    {
      auto pixel = topo.Map (i, j);
      strip.SetPixelColor (pixel, RgbColor (0, 0, 0));
      
      int dx = (PanelWidth / 2) - i;
      int dy = (PanelHeight - 1) - j;
      if (dy >= dx && dy >= -dx)
      {
        auto distance = 2.0 * sqrt ((dx * dx) + (dy * dy));
        auto shade = int ((sin (distance - (time / 300.0)) + 0.5) * 190);
        if (distance > 14)
        {
          shade -= (distance - 14) * 60;
        }          
        if (shade > 0)
        {
          RgbColor color ((shade * base.R) >> 8, (shade * base.G) >> 8, (shade * base.B) >> 8);
          strip.SetPixelColor (pixel, color);
        }
      }
    }
  }  
  strip.Show ();
}

/**************************************************************************/
