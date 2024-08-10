/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

/**************************************************************************/

/*
 * Periodically request the weather (asynchronously) and parse any response
 */
void fetch_weather ()
{
  /*
   * Send requests every 5 minutes. OWM allows us up to every 1 minute, but
   * this seems a little excessive.
   */
  constexpr unsigned long interval = 5 * 60 * 1000;

  /*
   * If timeout or location has changed, send a weather request
   */
  static String last_longitude;
  static String last_latitude;
  static unsigned long last_request = 0;
  const auto &longitude = settings[F("longitude")];
  const auto &latitude = settings[F("latitude")];
  const auto changed = (longitude != last_longitude || latitude != last_latitude); 
  if (changed)
  {
    /*
     * Invalidate
     */
    celsius = -99;
  }
  if ((millis () - last_request > interval || changed) &&
      longitude.length () > 0 && latitude.length () > 0)
  {
    client.stop ();
    client.setTimeout (5000);
    if (client.connect ("api.open-meteo.com", 80)) 
    {
      String request (F("GET /v1/forecast?current=temperature_2m&forecast_days=1&latitude="));
      request += latitude;
      request += F("&longitude=");
      request += longitude;
      request += F(" HTTP/1.1\n");
      request += F("Host: api.open-meteo.com\n");
      request += F("Connection: close\n\n");
      client.print (request);
    }
    last_request = millis ();
    last_longitude = longitude;
    last_latitude = latitude;
  }
    
  /*
   * Read any response data - this isn't terribly robust but it does the trick
   */
  if (client.available ())
  {
    /*
     * Ignore header response data
     */
    while (client.available ()) 
    {
      /*
       * Ends with a blank line
       */
      auto str = client.readStringUntil ('\n'); 
      if (str.length () == 1)
      {
        break;
      }
    }

    /*
     * Read the JSON
     */
    String str;
    while (client.available ())
    {
      str += static_cast<char> (client.read ());
    }
    
    /*
     * Parse it for temperature - very basic, not trying to read most of the JSON response which looks like this:
     * 
     * {"latitude":52.52,"longitude":13.419998,"generationtime_ms":0.015020370483398438,"utc_offset_seconds":0,"timezone":"GMT","timezone_abbreviation":"GMT","elevation":38.0,"current_units":{"time":"iso8601","interval":"seconds","temperature_2m":"°C"},"current":{"time":"2024-08-10T15:15","interval":900,"temperature_2m":25.8}}
     */
    auto start = str.indexOf ("\"temperature_2m\":");
    if (start != -1)
    {
      start = str.indexOf ("\"temperature_2m\":", start + 1);
      if (start != -1)
      {
        start += 17;
        auto end = str.indexOf ("}", start);
        if (end != -1)
        {
          celsius = str.substring (start, end).toFloat ();
        }
      }
    }
    
    /*
     * Debug
     */
    if (celsius == -99)
    {
      Serial.print (F("Failed to parse weather response: "));
      Serial.println (str);
    }
  }
}

/**************************************************************************/
