/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

/**************************************************************************/

/*
 * Message size is quite limited - we're running short on RAM...
 */
GFXcanvas1 message_canvas (512, PanelHeight);
unsigned long last_scroll = 0;
unsigned long next_repeat = 0;
int16_t message_scroll = 0;
int16_t message_width = 0;
constexpr auto message_margin = 8;
void show_message (String message);

/**************************************************************************/

/*
 * Called during the main loop to print the message
 */
bool message ()
{
  if (message_scroll >= message_width)
  {
    if (millis () > next_repeat)
    {
      auto interval = settings[F("messageRepeat")].toInt ();
      if (interval > 0)
      {
        next_repeat = millis () + (interval * 1000);
        show_message (settings[F("message")]);
      }
    }
    
    return false;
  }

  /*
   * Scroll the message if necessary
   */
  constexpr auto scroll_interval = 30; /* ms */
  auto now = millis (); 
  while (now - last_scroll > scroll_interval)
  {
    ++message_scroll;
    last_scroll += scroll_interval;
  }

  /*
   * We're printing
   */
  HtmlColor html_color;
  html_color.Parse<HtmlColorNames> (settings.at (F("messageColor")));
  for (int i = 0; i != PanelWidth; ++i)
  {
    for (int j = 0; j != PanelHeight; ++j)
    {
      auto pixel = topo.Map (i, j);
      if (message_canvas.getPixel (i + message_scroll, j))
      { 
        strip.SetPixelColor (pixel, html_color);
      }
      else
      { 
        strip.SetPixelColor (pixel, RgbColor (0, 0, 0));
      }
    }
  }       
  
  /*
   * Don't overwrite the display
   */
  return true;
}

/**************************************************************************/

/*
 * Print a new message
 * 
 * NB: overwrites anything already being displayed
 */
void show_message (String message)
{
  if (message.length () == 0)
  {
    return;
  }

  auto two_digit = [] (int n)
  {
    if (n < 10)
    {
      return String ("0" + String (n));
    }
    else
    {
      return String (n);
    }
  };

  /*
   * Transform the message - substitute in the same fields as we provide in the 
   * clock displays
   */
  message.replace ("%Y", String (year));
  message.replace ("%y", two_digit (year % 100));
  message.replace ("%m", two_digit (month));
  message.replace ("%d", two_digit (day));
  message.replace ("%H", two_digit (hours));
  message.replace ("%h", two_digit (hours == 0 ? 12 : (hours > 12 ? hours - 12 : hours)));
  message.replace ("%M", two_digit (minutes));
  message.replace ("%S", two_digit (seconds));
  if (ping_time < 1000)
  {
    message.replace ("%p", String (ping_time));
  }
  else
  {
    message.replace ("%p", "??");
  }
  if (celsius == -99)
  {
    message.replace ("%C", "??");
    message.replace ("%F", "??");
  }
  else
  {
    message.replace ("%C", String (celsius, 1));
    message.replace ("%F", String (32.0 + (celsius * 9) / 5, 1));
  }
  
  /*
   * Clear the canvas
   */
  message_canvas.writeFillRect (0, 0, message_canvas.width (), message_canvas.height (), 0);

  /*
   * Write the message into the canvas
   */
  message_canvas.setFont (&TextFont);
  message_canvas.setCursor (0, 7);
  message_canvas.println (message);

  /*
   * Get the bounds
   */
  int16_t x, y;
  uint16_t h, w;
  message_canvas.getTextBounds (message, 0, 0, &x, &y, &w, &h);

  /*
   * Leave a little margin otherwise it seems too abrupt
   */
  message_width = w + message_margin;
  message_scroll = -(PanelWidth + message_margin);
  last_scroll = millis ();
}

/**************************************************************************/
