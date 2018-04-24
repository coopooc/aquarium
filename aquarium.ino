
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
#define NUM_LEDS 29
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#include "GradientPalettes.h"

CRGB leds[NUM_LEDS];


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
  Alarm.alarmRepeat(6, 15, 0, startsunrise);
  Alarm.alarmRepeat(6, 45, 0, startdaylight);
  Alarm.alarmRepeat(20, 0, 0, startsunset);
  Alarm.alarmRepeat(20, 30, 0, startmoonglow);
  Alarm.alarmRepeat(22, 0, 0, startlightsoff);



}

CRGBPalette16 currentsunrisePalette;

static uint8_t sunrisepalettenumber = 1;

CRGBPalette16 currentsunsetPalette;

static uint8_t sunsetpalettenumber = 1;

boolean sunriseGo = false;
boolean sunsetGo = false;
boolean daylightGo = false;
boolean purplelightGo = false;
boolean moonglowGo = false;
boolean lightsoffGo = false;


void loop() {

  if (sunriseGo == true) {
    sunrise();
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
    sunset();
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

  Alarm.delay(1);
}

void startsunrise() {
  sunriseGo = true;
  sunsetGo = false;
  daylightGo = false;
  purplelightGo = false;
  moonglowGo = false;
  lightsoffGo = false;

  if (sunrisepalettenumber < 5 ) {
    sunrisepalettenumber++;
  }
  else if (sunrisepalettenumber == 5 ) {
    sunrisepalettenumber = 1;
  }

}

void startsunset() {
  sunsetGo = true;
  sunriseGo = false;
  daylightGo = false;
  purplelightGo = false;
  moonglowGo = false;
  lightsoffGo = false;

  if (sunsetpalettenumber < 5 ) {
    sunsetpalettenumber++;
  }
  else if (sunsetpalettenumber == 5 ) {
    sunsetpalettenumber = 1;
  }
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

void sunrise() {

  // total sunrise length, in minutes
  static const uint8_t sunriseLength = 30;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float interval = ((float)(sunriseLength * 60) / 256) * 1000;

  // current gradient palette color index
  static uint8_t heatIndex = 0; // start out at 0
  static uint8_t middleheatIndex = 15; // start out at 0


  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(HeatColors_p, heatIndex);
  CRGB middlecolor = ColorFromPalette(HeatColors_p, middleheatIndex);

  // fill the entire strip with the current color
  fill_solid(leds, 10, color);
  fill_solid(leds + 11, 9, middlecolor);
  fill_solid(leds + 22, 10, color);
  Serial.println("Fill Leds Happening...");

  // slowly increase the heat
  EVERY_N_MILLISECONDS(interval ) {
    if (heatIndex < 255) {
      heatIndex++;
    }
  }

}

void sunset() {

  // total sunrise length, in minutes
  static const uint8_t sunsetLength = 30;

  // how often (in seconds) should the heat color increase?
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const float intervalA = ((float)(sunsetLength * 60) / 256) * 1000;

  // current gradient palette color index
  static uint8_t heatIndexset = 255; // start out at 0

  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(HeatColors_p, heatIndexset);

  // fill the entire strip with the current color
  fill_solid(leds, NUM_LEDS, color);

  // slowly increase the heat
  EVERY_N_MILLISECONDS(intervalA ) {
    if (heatIndexset > 0) {
      heatIndexset--;
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
  leds[0].setRGB(255, 255, 255);
  leds[1].setRGB(255, 255, 255);
  leds[2].setRGB(255, 255, 255);
}

void lightsoff() {
  fadeToBlackBy(leds, NUM_LEDS, 64 );
}

void digitalClockDisplay() {
  time_t tnow = time(nullptr);
  Serial.println(ctime(&tnow));

}

void changesunrisePalette () {
  if ( sunrisepalettenumber == 1) {
    currentsunrisePalette = HeatColors_p;
  }
  if ( sunrisepalettenumber == 2) {
    currentsunrisePalette = sky_02_gp;
  }
}

void changesunsetPalette () {
  if ( sunsetpalettenumber == 1) {
    currentsunsetPalette = HeatColors_p;
  }
  if ( sunsetpalettenumber == 2) {
    currentsunsetPalette = sky_02_gp;
  }
}


