/*
 * ArClock
 * 
 * (c) Matt Aubury 2020
 */
 
#pragma once

#include <sstream>

/**************************************************************************/

using Settings = std::map<String, String>;

Settings settings;

/**************************************************************************/

/*
 * Default settings
 */
const char default_settings[] PROGMEM = 
R""(OWMAPIKey=
effect=None
effectColor=#ff0022
effectColorMode=Varying
effectDecay=0
effectSpeed=50
masterBrightness=32
messageColor=#1ab500
password=
pingTarget=8.8.8.8
primaryColor=#408080
primaryColorMode=Constant
primaryFont=Medium
primaryFormat=H:M
primaryX=6
primaryY=0
secondaryColor=#364ca0
secondaryColorMode=Constant
secondaryFont=Small
secondaryFormat=
secondaryX=23
secondaryY=1
ssid=
timezone=Etc/UTC
weatherLocation=
)"";

/**************************************************************************/

/*
 * Standard presets
 */
 
const char *preset_names[] PROGMEM =
{
  "",
  "Vanilla",
  "Minimalist",
  "Utility",
  "Perpetual",
  "Pride",
  "Ping",
  "1990s"
};

const char null_preset[] PROGMEM = "";

const char vanilla_preset[] PROGMEM =
R""(effect=None
primaryColor=#505030
primaryColorMode=Constant
primaryFont=Medium
primaryFormat=H:M
primaryX=6
primaryY=0
secondaryFormat=
)"";

const char minimalist_preset[] PROGMEM =
R""(effect=None
primaryColor=#184640
primaryColorMode=Varing
primaryFont=Small
primaryFormat=H:M
primaryX=14
primaryY=2
secondaryFormat=
)"";

const char utility_preset[] PROGMEM =
 R""(effect=None
primaryColor=#20440f
primaryColorMode=Constant
primaryFont=Medium
primaryFormat=H:M
primaryX=0
primaryY=0
secondaryColor=#364ca0
secondaryColorMode=Constant
secondaryFont=Small
secondaryFormat=C*
secondaryX=23
secondaryY=2
)"";

const char perpetual_preset[] PROGMEM =
R""(effect=None
primaryColor=#204284
primaryColorMode=Plasma
primaryFont=Small
primaryFormat=y.m.d
primaryX=3
primaryY=1
secondaryFormat=
)"";

const char pride_preset[] PROGMEM =
R""(effect=None
primaryColor=#82166c
primaryColorMode=Rainbow
primaryFont=Extra large
primaryFormat=H:M
primaryX=1
primaryY=0
secondaryFormat=
)"";

const char ping_preset[] PROGMEM =
R""(effect=Ping graph
effectColor=#320000
effectColorMode=Constant
effectDecay=58
effectSpeed=500
pingTarget=8.8.8.8
primaryColor=#6b6b6b
primaryColorMode=Constant
primaryFont=Small
primaryFormat=p
primaryX=10
primaryY=1
secondaryFormat=
)"";

const char nineties_preset[] PROGMEM =
R""(effect=Rain
effectColor=#00e000
effectColorMode=Constant
effectDecay=0
effectSpeed=305
primaryColor=#600000
primaryColorMode=Constant
primaryFont=7-segment
primaryFormat=H:M
primaryX=3
primaryY=1
secondaryFormat=
)"";

const char *presets[] PROGMEM =
{
  null_preset,
  vanilla_preset,
  minimalist_preset,
  utility_preset,
  perpetual_preset,
  pride_preset,
  ping_preset,
  nineties_preset,
};

/**************************************************************************/

void parse_settings (const char *data)
{
  /*
   * TODO: Arduino-ise this
   */
  std::string string (data);
  std::istringstream iss (string);
  while (!iss.eof ())
  {
    std::string key, value;
    std::getline (iss, key, '=');
    std::getline (iss, value, '\n');
    if (!key.empty ())
    {
      settings[key.c_str ()] = value.c_str ();
    }
  }  
}

/**************************************************************************/

void load_settings ()
{
  /*
   * Load fall-back defaults
   */
  parse_settings (String (FPSTR (default_settings)).c_str ());
  
  /*
   * Read from flash
   */
  LittleFS.begin ();
  auto file = LittleFS.open (F("arclock_settings"), "r");
  auto bytes = file.size ();
  auto buffer = reinterpret_cast<char *> (malloc (bytes + 1));
  file.read (reinterpret_cast<uint8_t *> (buffer), bytes);
  buffer[bytes] = '\0';
  file.close ();

  /*
   * Load them
   */
  parse_settings (buffer);
  free (buffer);
}

/**************************************************************************/

void save_settings ()
{
  /*
   * Create settings string
   */
  String str;
  for (const auto &key_value : settings)
  {
    str += key_value.first;
    str += '=';
    str += key_value.second;
    str += '\n';
  }
  
  /*
   * Save changes to flash
   */
  auto file = LittleFS.open (F("arclock_settings"), "w");
  file.write (str.c_str (), str.length ());
  file.close ();
}

/**************************************************************************/

void load_preset (const String &name)
{
  for (ssize_t i = 0; i != std::end (preset_names) - std::begin (preset_names); ++i)
  {
    if (name == preset_names[i])
    {
      parse_settings (String (FPSTR (presets[i])).c_str ());
    }
  }  
}

/**************************************************************************/
