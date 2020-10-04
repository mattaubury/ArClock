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
  static String last_location;
  static unsigned long last_request = 0;
  const auto &location = settings.at (F("weatherLocation"));
  const auto &api_key = settings[F("OWMAPIKey")];
  if (location != last_location)
  {
    /*
     * Invalidate
     */
    celsius = -99;
  }
  if ((millis () - last_request > interval || location != last_location) &&
      location.length () > 0 && api_key.length () > 0)
  {
    client.stop ();
    client.setTimeout (5000);
    if (client.connect ("api.openweathermap.org", 80)) 
    {
      String request (F("GET /data/2.5/forecast?q="));
      request += location;
      request += F("&appid=");
      request += api_key;
      request += F("&mode=json&units=metric&cnt=2 HTTP/1.1\n");
      request += F("Host: api.openweathermap.org\n");
      request += F("Connection: close\n\n");
      client.print (request);
    }
    last_request = millis ();
    last_location = location;
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
     * {"cod":"200","message":0,"cnt":2,"list":[{"dt":1601035200,"main":{"temp":11.28,"feels_like":3.31,"temp_min":11.28,"temp_max":12.46,"pressure":1004,"sea_level":1004,"grnd_level":999,"humidity":63,"temp_kf":-1.18},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03d"}],"clouds":{"all":36},"wind":{"speed":9.64,"deg":326},"visibility":10000,"pop":0,"sys":{"pod":"d"},"dt_txt":"2020-09-25 12:00:00"},{"dt":1601046000,"main":{"temp":12.33,"feels_like":4.08,"temp_min":12.33,"temp_max":12.93,"pressure":1006,"sea_level":1006,"grnd_level":1001,"humidity":56,"temp_kf":-0.6},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03d"}],"clouds":{"all":47},"wind":{"speed":9.85,"deg":329},"visibility":10000,"pop":0,"sys":{"pod":"d"},"dt_txt":"2020-09-25 15:00:00"}],"city":{"id":XXX,"name":"XXX","coord":{"lat":XXX,"lon":XXX},"country":"XX","population":XXX,"timezone":3600,"sunrise":XXX,"sunset":XXX}}
     */
    auto start = str.indexOf ("\"temp\":");
    if (start != -1)
    {
      start += 7;
      auto end = str.indexOf (",", start);
      if (end != -1)
      {
        celsius = str.substring (start, end).toFloat ();
      }
    }
    else
    {
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
}

/**************************************************************************/
