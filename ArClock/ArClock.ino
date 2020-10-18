/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include <Pinger.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare" /* rolls eyes */
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#pragma GCC diagnostic pop

#include <LittleFS.h>

#include <NeoPixelBrightnessBus.h>

#define FASTLED_INTERNAL /* suppress warnings */
#include <FastLED.h>

#include <time.h>
#include <sys/time.h>

/**************************************************************************/

/*
 * Definition of the LED matrix layout 
 */
constexpr uint8_t PanelWidth = 32;
constexpr uint8_t PanelHeight = 8;
constexpr uint16_t PixelCount = PanelWidth * PanelHeight;

NeoTopology<ColumnMajorAlternating180Layout> topo (PanelWidth, PanelHeight);
NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip (PixelCount);

/**************************************************************************/

/*
 * Networking globals
 */
DNSServer dnsServer;
ESP8266WebServer server (80);
IPAddress APIP (192, 168, 4, 1);
WiFiClient client;
Pinger pinger;

/**************************************************************************/

/*
 * Additional global state
 */
int16_t year = 0;
int8_t month = 0;
int8_t day = 0;
int8_t hours = 0;
int8_t minutes = 0;
int8_t seconds = 0;
int16_t ping_time = 1000;
int16_t ping_history[PanelWidth];
float celsius = -99; /* bit nippy */
  
/**************************************************************************/

/*
 * Include the functionality
 */
#include "text_font.h"
#include "timezones.h"
#include "web.h"
#include "clock.h"
#include "effect.h"
#include "settings.h"
#include "weather.h"
#include "message.h"
#include "wifi_logo.h"

/**************************************************************************/

void setup ()
{ 
  /*
   * Setup serial for debug
   */
  Serial.begin (115200);
  while (!Serial);

  /*
   * Configure strip
   */
  strip.Begin ();
  strip.SetPixelColor (0, RgbColor (0x40, 0x00, 0x00));
  strip.Show ();

  /*
   * Load settings from flash
   * 
   * Comment this out if you think you've corrupted your settings - 
   * e.g. if you're seeing a boot loop. Don't forget to put it back
   * when you've fixed the problem!
   */
  load_settings ();

  /*
   * Start WiFi in station mode if possible
   */
  WiFi.mode (WIFI_STA);
  WiFi.persistent (false);
  if (settings.at (F("ssid")) != "")
  {
    Serial.print (F("Connecting to "));
    Serial.print (settings.at (F("ssid")));
    WiFi.begin (settings.at (F("ssid")), settings.at (F("password")));
  
    /*
     * Wait for WiFi to connect before starting up MDNS
     */
    auto timeout = millis () + (30 * 1000);
    while (WiFi.status () != WL_CONNECTED && millis () < timeout)
    {
      wifi_logo (RgbColor (0x80, 0x80, 0x80));
      delay (15);
    }
  }

  /*
   * We either succeeded in connecting to a network, or failed (because
   * the details are wrong or missing)
   */
  if (WiFi.status () == WL_CONNECTED)
  {
    Serial.print (F("\nIP address: "));
    Serial.println (WiFi.localIP ());
    
    /*
     * Start mDNS responder, so we can be found as http://arclock.local
     */
    MDNS.begin (F("arclock"));

    /*
     * Setup pinger
     */
    pinger.OnReceive ([] (const PingerResponse &response)
    {
      if (response.DestIPAddress == IPAddress (255, 255, 255, 255))
      {
        /*
         * Bail if the target is invalid
         */
        return false;
      }

      /*
       * 1000 == failure
       */
      ping_time = response.ReceivedResponse ? response.ResponseTime : 1000;
      for (int x = 1; x != PanelWidth; ++x)
      {
        ping_history[x - 1] = ping_history[x];
      }
      ping_history[PanelWidth - 1] = ping_time;
      return true;
    });

    /*
     * Indicate to the user that WiFi is up
     */
    for (int N = 0x200; N >= 0; N -= 3)
    {
      wifi_logo (RgbColor (N > 0x180 ? N - 0x180 : 0, N > 0x100 ? 0x80 : (N > 0x80 ? N - 0x80 : 0), N > 0x180 ? N - 0x180 : 0));
      delay (15);
    }
    String message (F("Visit http://arclock.local or http://"));
    message += WiFi.localIP ().toString ();
    message += F(" to configure");
    show_message (message);
  }
  else
  {
    /*          
     * Start SoftAP mode with captive portal
     */
    WiFi.mode (WIFI_AP);
    WiFi.softAPConfig (APIP, APIP, IPAddress (255, 255, 255, 0));
    WiFi.softAP (F("ArClock"));
    dnsServer.start (53, "*", APIP);
    for (int N = 0x200; N >= 0; N -= 3)
    {
      wifi_logo (RgbColor (N > 0x180 ? N - 0x180 : 0, N > 0x180 ? N - 0x180 : 0, N > 0x100 ? 0x80 : (N > 0x80 ? N - 0x80 : 0)));
      delay (15);
    }
    show_message (F("Connect to ArClock WiFi network to setup"));
  }
  
  /*
   * Apply other settings
   */
  update ();

  /*
   * Start the webserver
   */
  server.on (F("/"), handle_root);
  server.on (F("/update"), handle_update);
  server.on (F("/change"), handle_change);
  server.on (F("/timezones"), handle_timezones);
  server.on (F("/reboot"), handle_reboot);
  server.on (F("/pure-min.css"), [] { server.send (200, "text/css", pure_min_css); });
  server.on (F("/grids-responsive-min.css"), [] { server.send (200, "text/css", grids_responsive_min_css); });
  server.on (F("/script.js"), [] { server.send (200, "text/js", script_js); });
  server.on (F("/show"), handle_show);
  server.on (F("/load_preset"), handle_load_preset);
  server.onNotFound (handle_root);
  server.begin ();
}

/**************************************************************************/

void loop ()
{
  /*
   * Banner messages take priority
   */
  if (!message ())
  {
    /*
     * Update the effect
     */
    effect (settings.at (F("effect")));
  
    /*
     * Get the temperature
     */
    fetch_weather ();
      
    /*
     * Get the time
     */
    auto now = ::time (nullptr);
    auto tm = ::localtime (&now);
    year = 1900 + tm->tm_year;
    month = tm->tm_mon + 1;
    day = tm->tm_mday;
    hours = tm->tm_hour;
    minutes = tm->tm_min;
    seconds = tm->tm_sec;
    
    /*
     * Display the clocks (suppressed for the first few seconds to allow NTP startup)
     */
    if (now > 5)
    {
      clock (F("primary"));
      clock (F("secondary"));
    }  
  }
   
  /*
   * Display the data
   */
  strip.Show ();

  /*
   * Handle web server events
   */
  server.handleClient ();
  MDNS.update ();
  dnsServer.processNextRequest ();
}

/**************************************************************************/
