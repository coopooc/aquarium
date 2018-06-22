
#include <ESP8266TimeAlarms.h>
#include "WifiConfig.h"
#include <ESP8266WiFi.h>

#ifndef WIFI_CONFIG_H

#endif // !WIFI_CONFIG_H

AlarmId id;

#define SLEEP_TIME  10*60*1000 // milliseconds
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

// For LEDs
#define FASTLED_RGBW
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_DMA
#include <FastLED.h>
#define DATA_PIN 3
#define NUM_LEDS 72
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
//#include "GradientPalettes.h"

CRGB leds[NUM_LEDS];

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;

TBlendType currentBlending;

uint8_t sunrisecolorIndex = 255; // start out at 0
uint8_t middlesunrisecolorIndex = 240; // start out at 0

static uint8_t sunsetcolorIndex = 0; // start out at 0
static uint8_t middlesunsetcolorIndex = 15; // start out at 0

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


  // Set alarms
  Alarm.alarmRepeat(20, 19, 0, startsunrise);
  Alarm.alarmRepeat(20, 21, 0, startlightsoff);
  Alarm.alarmRepeat(20, 22, 0, startsunrise);
  Alarm.alarmRepeat(20, 23, 0, startlightsoff);
  Alarm.alarmRepeat(20, 24, 0, startsunrise);
  Alarm.alarmRepeat(20, 25, 0, startlightsoff);

//  Alarm.alarmRepeat(19, 33, 0, startsunrise);
  //  Alarm.alarmRepeat(19, 21, 0, startpurplelight);
  //  Alarm.alarmRepeat(20, 34, 0, startdaylight);
  //  Alarm.alarmRepeat(19, 25, 0, startsunset);
  //  Alarm.alarmRepeat(21, 36, 0, startmoonglow);
  //  Alarm.alarmRepeat(19, 27, 0, startlightsoff);


  // Forward declarations of an array of cpt-city gradient palettes, and
  // a count of how many there are.  The actual color palette definitions
  // are at the bottom of this file.
  //  extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
  //  extern const uint8_t gGradientPaletteCount;
  //
  //  // Current palette number from the 'playlist' of color palettes
  //  uint8_t gCurrentPaletteNumber = 0;

  //CRGBPalette16 gCurrentPalette( CRGB::Black);
  //CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

  currentBlending = NOBLEND;

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
uint8_t gCurrentPaletteNumber = 0;





void loop() {

  if (sunriseGo == true) {
    sunrise(leds, NUM_LEDS);
    FastLED.show();
  }
  else if (daylightGo == true) {
    daylight();
    FastLED.show();
  }
  else if (purplelightGo == true) {
    purplelight();
    FastLED.show();
  }
  else if (sunsetGo == true) {
    sunset(leds, NUM_LEDS);
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

  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];

  digitalClockDisplay();
  //  Serial.println("Current Palette Number");
  //  Serial.println(gCurrentPaletteNumber);
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

  gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
  //  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];

  sunrisecolorIndex = 255;
  middlesunrisecolorIndex = 240;

  Serial.println("SunrisegCurrentPaletteNumber");
  Serial.println(gCurrentPaletteNumber);



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

  gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];

  sunsetcolorIndex = 0; // start out at 0
  middlesunsetcolorIndex = 15; // start out at 0

  Serial.println("SunsetgCurrentPaletteNumber");
  Serial.println(gCurrentPaletteNumber);


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

void sunrise( CRGB* ledarray, uint16_t numleds ) {

  uint8_t maxChanges = 48;
  nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, maxChanges);

  // total sunrise length, in minutes
  static const uint8_t sunriseLength = 20;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float interval = ((float)(sunriseLength * 60) / 256) * 1000;

  // current gradient palette color index
  //  static uint8_t sunrisecolorIndex = 255; // start out at 0
  //  static uint8_t middlesunrisecolorIndex = 240; // start out at 0


  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(gTargetPalette, sunrisecolorIndex);
  CRGB middlecolor = ColorFromPalette(gTargetPalette, middlesunrisecolorIndex);

  // fill the entire strip with the current color
  fill_solid(leds, 13, color);
  fill_solid(leds + 13, 10, middlecolor);
  fill_solid(leds + 23, 26, color);
  fill_solid(leds + 49, 10, middlecolor);
  fill_solid(leds + 59, 13, color);


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

void sunset( CRGB* ledarray, uint16_t numleds ) {

  uint8_t maxChanges = 48;
  nblendPaletteTowardPalette(gCurrentPalette, gTargetPalette, maxChanges);

  // total sunrise length, in minutes
  static const uint8_t sunsetLength = 20;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float intervalA = ((float)(sunsetLength * 60) / 256) * 1000;

  //  // current gradient palette color index
  //  static uint8_t sunsetcolorIndex = 0; // start out at 0
  //  static uint8_t middlesunsetcolorIndex = 15; // start out at 0

  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(gTargetPalette, sunsetcolorIndex);
  CRGB middlecolor = ColorFromPalette(gTargetPalette, middlesunsetcolorIndex);

  // fill the entire strip with the current color
  fill_solid(leds, 13, color);
  fill_solid(leds + 13, 10, middlecolor);
  fill_solid(leds + 23, 26, color);
  fill_solid(leds + 49, 10, middlecolor);
  fill_solid(leds + 59, 13, color);

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
  fill_solid(leds, NUM_LEDS, CRGB(235, 235, 255));
}

void purplelight() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 255));
}

void moonglow() {
  fadeToBlackBy(leds, 36, 1 );
  fadeToBlackBy(leds + 39, 33, 1 );
  fill_solid(leds + 36, 3, CRGB(102, 0, 102));
}

void lightsoff() {
  fadeToBlackBy(leds, NUM_LEDS, 1 );
}

void digitalClockDisplay() {
  time_t tnow = time(nullptr);
  Serial.println(ctime(&tnow));

}

// Gradient palette "sky_02_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
// This one goes from white through some blus to black

DEFINE_GRADIENT_PALETTE( sky_02_gp ) {
  0, 255, 255, 255,
  10, 215, 197, 49,
  38, 171, 146, 82,
  89, 118, 138, 130,
  151,  28, 61, 93,
  203,   1, 14, 75,
  245,   1,  4, 47,
  255,   0,  0,  0
};

// Gradient palette "sky_03_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-03.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_03_gp ) {
  0, 255, 255, 255,
  10, 255, 239, 130,
  76, 120, 118, 59,
  151,   5, 38, 54,
  250,   0,  9, 18,
  255,   0,  0,  0
};


// Gradient palette "sky_04_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-04.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_04_gp ) {
  0, 255, 255, 255,
  10, 255, 189,  3,
  76, 110, 19,  1,
  151,  27,  5,  1,
  255,   1,  1,  1
};

// Gradient palette "sky_05_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-05.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_05_gp ) {
  0, 255, 255, 255,
  10, 252, 61,  2,
  25, 255, 146,  4,
  63, 224, 255, 255,
  101,  46, 114, 226,
  127,   6, 40, 127,
  191,   1,  3, 17,
  250,   1,  1,  4,
  255,   0,  0,  0
};

// Gradient palette "sky_09_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-09.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_09_gp ) {
  0, 255, 255, 255,
  10, 252, 97,  2,
  89,  17, 17,  4,
  178,   7,  8,  2,
  255,   1,  1,  1
};


// Gradient palette "sky_10_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-10.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_10_gp ) {
  0, 255, 255, 255,
  10, 247, 235,  9,
  38, 217, 117, 52,
  89, 123, 43, 22,
  165,  26,  7,  4,
  255,   1,  1,  1
};

// Gradient palette "sky_11_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-11.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_11_gp ) {
  0, 255, 255, 255,
  10, 210, 80,  3,
  38, 255, 215, 106,
  165,  64, 114, 176,
  250,   5, 12, 38,
  255,   0,  0,  0
};



// Gradient palette "sky_12_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-12.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
// This one starts in the pinks and goes through violets to black

DEFINE_GRADIENT_PALETTE( sky_12_gp ) {
  0, 255, 255, 255,
  10, 206, 78, 44,
  68, 167, 57, 155,
  165,  12,  1, 37,
  250,   1,  1,  9,
  255,   0,  0,  0
};

// Gradient palette "sky_21_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-21.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_21_gp ) {
  0, 255, 255, 255,
  10, 255, 164, 49,
  40, 227, 141, 72,
  87, 125, 149, 135,
  178,   0, 31, 52,
  250,   1, 10, 22,
  255,   0,  0,  0
};


// Gradient palette "sky_22_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-22.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
// A very blue one ideal for a cool sunrise ending in white.

DEFINE_GRADIENT_PALETTE( sky_22_gp ) {
  0, 255, 255, 255,
  10, 224, 244, 255,
  87,  83, 203, 255,
  178,  48, 156, 233,
  245,   3, 59, 162,
  255,   0,  0,  0
};

// Gradient palette "sky_25_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-25.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_25_gp ) {
  0, 255, 255, 255,
  30,  10,  4, 25,
  40,  90,  5, 12,
  87,  74, 24, 22,
  178,  51, 42, 73,
  235,  27, 35, 108,
  255,   0,  0,  0
};

// Gradient palette "sky_26_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-26.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_26_gp ) {
  0, 255, 255, 255,
  10, 247, 124, 37,
  51, 115, 58, 13,
  89, 109, 55, 14,
  178,  55, 31, 16,
  250,  13, 12, 12,
  255,   0,  0,  0
};

// Gradient palette "sky_33_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-33.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_33_gp ) {
  0, 255, 255, 255,
  10, 237, 229, 140,
  51, 227, 107, 79,
  87, 155, 55, 54,
  178,  22, 28, 36,
  245,   5, 19, 31,
  255,   0,  0,  0
};

// Gradient palette "sky_34_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-34.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_34_gp ) {
  0, 255, 255, 255,
  20, 199, 142, 11,
  51, 171, 60, 17,
  87,  21, 39, 24,
  178,   1,  8,  6,
  255,   0,  1,  1
};

// Gradient palette "sky_39_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rafi/tn/sky-39.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( sky_39_gp ) {
  0, 255, 255, 255,
  10, 247, 255, 85,
  36, 255, 248,  0,
  140,  77,  6,  1,
  250,  16,  1,  0,
  255,   0,  0,  0
};


// Gradient palette "Magenta_Evening_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Magenta_Evening.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Magenta_Evening_gp ) {
  0, 255, 255, 255,
  25,  71, 27, 39,
  31, 130, 11, 51,
  63, 213,  2, 64,
  70, 232,  1, 66,
  76, 252,  1, 69,
  108, 123,  2, 51,
  240,  46,  9, 35,
  255,   0,  0,  0
};


// Gradient palette "Another_Sunset_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Another_Sunset.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( Another_Sunset_gp ) {
  0, 255, 255, 255,
  24, 110, 49, 11,
  29,  55, 34, 10,
  68,  22, 22,  9,
  68, 239, 124,  8,
  97, 220, 156, 27,
  124, 203, 193, 61,
  178,  33, 53, 56,
  245,   0,  1, 52,
  255,   0,  0,  0
};

// Gradient palette "Night_Stormy_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Night_Stormy.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( Night_Stormy_gp ) {
  0, 255, 255, 255,
  40,  10,  2, 26,
  57,  55, 39, 62,
  93, 155, 131, 117,
  101, 201, 187, 178,
  105, 255, 255, 255,
  108, 201, 187, 178,
  113, 155, 131, 117,
  136, 161, 73, 54,
  172, 165, 33, 18,
  211,  67, 17, 23,
  245,  16,  7, 30,
  255,   0,  0,  0
};


// Gradient palette "SummerSunset_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/SummerSunset.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( SummerSunset_gp ) {
  0, 255, 255, 255,
  20, 163, 23,  1,
  33, 206, 34,  5,
  70, 255, 48, 17,
  95, 255, 45, 31,
  100, 255, 44, 52,
  106, 249, 40, 23,
  122, 244, 37,  7,
  137, 249, 65,  6,
  163, 255, 100,  4,
  207, 100, 44,  3,
  245,  22, 11,  3,
  255,   0,  0,  0
};


// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 255, 255, 255,
  15, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  240,   0,  0, 160,
  255,   0,  0,  0
};


// Gradient palette "Sunset_Wow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Wow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( Sunset_Wow_gp ) {
  0, 255, 255, 255,
  15, 109,  4, 24,
  25, 173, 25, 15,
  51, 255, 67,  8,
  76, 255, 87,  9,
  103, 255, 111, 10,
  108, 184, 36, 14,
  112, 126,  3, 20,
  117, 179, 21, 14,
  123, 244, 55,  9,
  135, 249, 70,  8,
  152, 255, 86,  8,
  196, 192, 28, 12,
  240, 139,  2, 17,
  255,   0,  0,  0
};

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  sky_02_gp,
  sky_03_gp,
  sky_04_gp,
  sky_05_gp,
  sky_09_gp,
  sky_10_gp,
  sky_11_gp,
  sky_12_gp,
  sky_21_gp,
  sky_22_gp,
  sky_25_gp,
  sky_26_gp,
  sky_33_gp,
  sky_34_gp,
  sky_39_gp,
  Magenta_Evening_gp,
  Another_Sunset_gp,
  Night_Stormy_gp,
  SummerSunset_gp,
  Sunset_Real_gp,
  Sunset_Wow_gp
};



//// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount =
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );


