
#include <ESP8266TimeAlarms.h>
#include "WifiConfig.h"
#include <ESP8266WiFi.h>

#ifndef WIFI_CONFIG_H

#endif // !WIFI_CONFIG_H

AlarmId id;

#define SLEEP_TIME  10*60*1000 // milliseconds
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

// For LEDs
#define FASTLED_RGBW
#include <FastLED.h>
#define DATA_PIN 5
#define NUM_LEDS 45
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#include "GradientPalettes.h"

CRGB leds[NUM_LEDS];

CRGBPalette16 gPal;

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Sunset_Real_gp,
  sky_22_gp,
  sky_02_gp,
  sky_12_gp,
  Magenta_Evening_gp,
  sky_04_gp
};

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

//CRGBPalette16 currentsunrisePalette;
//CRGBPalette16 currentsunsetPalette;

//CRGBPalette16 currentPalette;
//TBlendType    currentBlending;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);


  WiFi.mode(WIFI_STA);
  WiFi.begin(YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);

  configTime(0, 0, "time.nist.gov");
  //Europe/Stockholm": "CET-1CEST,M3.5.0,M10.5.0/3"
  //Get JSON of Olson to TZ string using this code https://github.com/pgurenko/tzinfo
  setenv("TZ", "CST6CDT,M3.2.0,M11.1.0", 1);
  tzset();
  Serial.print("Clock before sync: ");
  digitalClockDisplay();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Clock after Wifi: ");

  gCurrentPalette = gGradientPalettes[1];

  // Set alarms
  Alarm.alarmRepeat(19, 36, 0, startsunrise);
  Alarm.alarmRepeat(19, 37, 0, startdaylight);
  Alarm.alarmRepeat(19, 38, 0, startsunset);
  Alarm.alarmRepeat(19, 32, 0, startmoonglow);
  Alarm.alarmRepeat(19, 27, 0, startlightsoff);

}


//static uint8_t sunrisepalettenumber = 1;
//
//static uint8_t sunsetpalettenumber = 1;

boolean sunriseGo = false;
boolean sunsetGo = false;
boolean daylightGo = false;
boolean purplelightGo = false;
boolean moonglowGo = false;
boolean lightsoffGo = false;


// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;
//
//const TProgmemRGBGradientPalettePtr gGradientPalettes[];
//const uint8_t gGradientPaletteCount;


// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 1;





void loop() {

  if (sunriseGo == true) {
    sunrise(leds, NUM_LEDS, gTargetPalette);
    FastLED.show();
  }
  else if (daylightGo == true) {
    daylight();
    FastLED.show();
  }
  //  else if (purplelightGo == true) {
  //    purplelight();
  //    FastLED.show();
  //  }
  else if (sunsetGo == true) {
    sunset(leds, NUM_LEDS, gTargetPalette);
    FastLED.show();
  }
  else if (moonglowGo == true) {
    moonglow();
    FastLED.show();
  }
  else if (lightsoffGo == true) {
    lightsoff();
    FastLED.show();
  }

  digitalClockDisplay();
//  Serial.println("Target Palette");
//  Serial.println(gTargetPalette);
//  Serial.println("Current Palette");
//  Serial.println(gCurrentPalette);
  Serial.println("Current Palette Number");
  Serial.println(gCurrentPaletteNumber);
//  Serial.println("Palette");
//  Serial.println(palette);
  Serial.println("SunriseGo");
  Serial.println(sunriseGo);
  Serial.println("SunsetGo");
  Serial.println(sunsetGo);
  Serial.println("DaylightGo");
  Serial.println(daylightGo);
  Serial.println("PurplelightGo");
  Serial.println(purplelightGo);
  Serial.println("moonglow");
  Serial.println(moonglowGo);
  Serial.println("LightsOffGo");
  Serial.println(lightsoffGo);

  Alarm.delay(10);
}

void startsunrise() {
  sunriseGo = true;
  sunsetGo = false;
  daylightGo = false;
  purplelightGo = false;
  moonglowGo = false;
  lightsoffGo = false;

  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);



//  if (gCurrentPaletteNumber < 6 ) {
//    gCurrentPaletteNumber++;
//  }
//  else if (gCurrentPaletteNumber == '5' ) {
//    gCurrentPaletteNumber = 1;
//  }

}

void startsunset() {
  sunsetGo = true;
  sunriseGo = false;
  daylightGo = false;
  purplelightGo = false;
  moonglowGo = false;
  lightsoffGo = false;

  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);



//  if (gCurrentPaletteNumber < 6 ) {
//    gCurrentPaletteNumber++;
//  }
//  else if (gCurrentPaletteNumber == '5' ) {
//    gCurrentPaletteNumber = 1;
//  }

}

void startdaylight() {
  daylightGo = true;
  sunriseGo = false;
  sunsetGo = false;
  purplelightGo = false;
  moonglowGo = false;
  lightsoffGo = false;
}

void startpurplelight() {
  purplelightGo = true;
  sunriseGo = false;
  sunsetGo = false;
  daylightGo = false;
  moonglowGo = false;
  lightsoffGo = false;
}

void startmoonglow() {
  moonglowGo = true;
  sunriseGo = false;
  sunsetGo = false;
  daylightGo = false;
  purplelightGo = false;
  lightsoffGo = false;
}

void startlightsoff() {
  lightsoffGo = true;
  sunriseGo = false;
  sunsetGo = false;
  daylightGo = false;
  purplelightGo = false;
  moonglowGo = false;
}

void testalarm() {
  FastLED.showColor(CRGB::Blue);
  Serial.println("Testing testing 123...");
}

void sunrise( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette ) {

  //CRGBPalette16 gCurrentPalette;
  //  gCurrentPalette = gGradientPalettes[ gCurrentPaletteNumber ];

  // total sunrise length, in minutes
  static const uint8_t sunriseLength = 1;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float interval = ((float)(sunriseLength * 60) / 256) * 1000;

  // current gradient palette color index
  static uint8_t sunrisecolorIndex = 255; // start out at 0
  static uint8_t middlesunrisecolorIndex = 240; // start out at 0


  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(palette, sunrisecolorIndex);
  CRGB middlecolor = ColorFromPalette(palette, middlesunrisecolorIndex);

  // fill the entire strip with the current color
  fill_solid(leds, 10, color);
  fill_solid(leds + 10, 9, middlecolor);
  fill_solid(leds + 19, 10, color);
  Serial.println("Sunrise Happening on Palette-");


  // slowly increase the heat
  EVERY_N_MILLISECONDS(interval ) {
    if (sunrisecolorIndex > 0) {
      sunrisecolorIndex--;
    }
  }
  // slowly increase the heat
  EVERY_N_MILLISECONDS(interval ) {
    if (middlesunrisecolorIndex > 0) {
      middlesunrisecolorIndex--;
    }
  }

}

void sunset( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette ) {

  // total sunrise length, in minutes
  static const uint8_t sunsetLength = 1;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float intervalA = ((float)(sunsetLength * 60) / 256) * 1000;

  // current gradient palette color index
  static uint8_t sunsetcolorIndex = 255; // start out at 0
  static uint8_t middlesunsetcolorIndex = 240; // start out at 0

  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(palette, sunsetcolorIndex);
  CRGB middlecolor = ColorFromPalette(palette, middlesunsetcolorIndex);

  // fill the entire strip with the current color
  fill_solid(leds, 10, color);
  fill_solid(leds + 10, 9, middlecolor);
  fill_solid(leds + 19, 10, color);

  // slowly increase the heat
  EVERY_N_MILLISECONDS(intervalA ) {
    if (sunsetcolorIndex < 255) {
      sunsetcolorIndex++;
    }
  }

  // slowly increase the heat
  EVERY_N_MILLISECONDS(intervalA ) {
    if (middlesunsetcolorIndex < 255) {
      middlesunsetcolorIndex++;
    }
  }
}

void daylight() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 255, 255));
}

void purplelight() {
  fill_solid(leds, NUM_LEDS, CRGB(102, 0, 102));
}

void moonglow() {
  fadeToBlackBy(leds + 3, NUM_LEDS - 4, 64 );
  fill_solid(leds, 3, CRGB(102, 0, 102));
}

void lightsoff() {
  fadeToBlackBy(leds, NUM_LEDS, 64 );
}

void digitalClockDisplay() {
  time_t tnow = time(nullptr);
  Serial.println(ctime(&tnow));

}


//// Gradient palette "sky_02_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-02.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 24 bytes of program space.
//// This one goes from white through some blus to black
//
//DEFINE_GRADIENT_PALETTE( sky_02_gp ) {
//  0, 215, 197, 49,
//  38, 171, 146, 82,
//  89, 118, 138, 130,
//  151,  28, 61, 93,
//  203,   1, 14, 75,
//  255,   1,  4, 47
//};
//
////CRGBPalette16 gCurrentPalette = sky_02_gp;
//
//// Gradient palette "sky_12_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-12.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 16 bytes of program space.
//// This one starts in the pinks and goes through violets to black
//
//DEFINE_GRADIENT_PALETTE( sky_12_gp ) {
//  0, 206, 78, 44,
//  68, 167, 57, 155,
//  165,  12,  1, 37,
//  255,   1,  1,  9
//};
//
//
//// Gradient palette "sky_22_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-22.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 16 bytes of program space.
//// A very blue one ideal for a cool sunrise ending in white.
//
//DEFINE_GRADIENT_PALETTE( sky_22_gp ) {
//  0, 224, 244, 255,
//  87,  83, 203, 255,
//  178,  48, 156, 233,
//  255,   3, 59, 162
//};
//
//// Gradient palette "Sunset_Real_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 28 bytes of program space.
//
//DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
//  0, 120,  0,  0,
//  22, 179, 22,  0,
//  51, 255, 104,  0,
//  85, 167, 22, 18,
//  135, 100,  0, 103,
//  198,  16,  0, 130,
//  255,   0,  0, 160
//};
//
//
//// Gradient palette "Magenta_Evening_gp", originally from
//// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Magenta_Evening.png.index.html
//// converted for FastLED with gammas (2.6, 2.2, 2.5)
//// Size: 28 bytes of program space.
//
//DEFINE_GRADIENT_PALETTE( Magenta_Evening_gp ) {
//  0,  71, 27, 39,
//  31, 130, 11, 51,
//  63, 213,  2, 64,
//  70, 232,  1, 66,
//  76, 252,  1, 69,
//  108, 123,  2, 51,
//  255,  46,  9, 35
//};

// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly
// by name every time.
// Since it is const, this array could also be moved
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
//const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
//  Sunset_Real_gp,
//  sky_22_gp,
//  sky_02_gp,
//  sky_12_gp,
//  Magenta_Evening_gp
//};


//// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount =
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
