/**
  Organization: Queen's Racing, 2022
  Program:      NeoPixel RPM Light Test
  Author:       Samuel Emard-Thibault
  Version:      Revision 1
  Date:         11/14/2022
**/

#include <Adafruit_NeoPixel.h>

#define BRIGHTNESS 200 // 0 to 255
#define PIN      6    // digital Arduino pin
#define N_LEDS 16     // number of pixels on strip
#define REDLINE 8000  // define the engine speed's limit
#define LOW_END 4000  // define the minimum RPM before LEDs start to light up

int idealShiftSpeed = REDLINE - 300;  // generalized, find real ideal speed using actual HP/TQ curves
int numPixels = 0;                    // Number of pixels we'd like to display for one iteration

// initialize strip object and colors
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t green = strip.Color(0, BRIGHTNESS, 0);
uint32_t red = strip.Color(BRIGHTNESS, 0, 0);
uint32_t blue = strip.Color(0, 0, BRIGHTNESS);

// test values
float currEngineSpeed[16] = {1000.0,1000.0, 1000.0, 1000.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7000.0, 7500.0, 8000.0, 8500.0, 8500.0, 8500.0};

void setup() {
  strip.begin();
  startAnimation();
}

void loop() {
  // for each RPM input (test data)
  for (int i = 0; i < 16; i++)
  {
    // if we're at or past the ideal engine speed for upshifts, flash
    if (currEngineSpeed[i] >= idealShiftSpeed)
    {
      flash();
    // if we're under the low end of the powerband, don't show anything
    } else if (currEngineSpeed[i] < LOW_END)
    {
      strip.clear();
    } else
    {
      // else, find % of current engine speed, and map it to the num of pixels we want to light up
      numPixels = map(currEngineSpeed[i], LOW_END, REDLINE, 0, N_LEDS);
      updateLEDS(numPixels);
    }
    delay(80);
  }
}

// flash full red strip once
void flash()
{
  strip.clear();
  strip.show();
  delay(30);
  for (int i = 0; i < N_LEDS; i++)
    strip.setPixelColor(i, red);
  strip.show();
}

// updates the strip based on current numLEDS calculated in loop()
void updateLEDS(int n)
{
  // clear the strip (in case engine speed decreased)
  strip.clear();

  // light up number of pixels based on parameter
  for (int i = 0; i < n; i++)
  {
    // RPM<33%      : GREEN
    // 33%<RPM<66%  : BLUE
    // RPM>66%      : RED
    if (i < N_LEDS * 0.33)
    {
      strip.setPixelColor(i, green);
    } else if (i < N_LEDS * 0.66)
    {
      strip.setPixelColor(i, blue);
    } else
    {
      strip.setPixelColor(i, red);
    }
  }
  strip.show();
}

// starting animation (~2sec)
void startAnimation()
{

  // collision animation
  for (int i = 0; i < N_LEDS; i++)
  {
    strip.setPixelColor(i, blue);
    strip.setPixelColor(N_LEDS - i, blue);
    strip.show();
    delay(25);
  }
  for (int i = 0; i < N_LEDS; i++)
  {
    strip.setPixelColor(i, 0);
    strip.setPixelColor(N_LEDS - i, 0);
    strip.show();
    delay(25);
  }

  // chase animation
  for (int i = 0; i < N_LEDS + 4; i++)
  {
    strip.setPixelColor(i, green);

    if (i > 3)
      strip.setPixelColor(i-4, 0);

    strip.show();
    delay(15);
  }
  for (int i = N_LEDS; i > -5; i--)
  {
    strip.setPixelColor(i, green);

    if (i < N_LEDS)
      strip.setPixelColor(i+4, 0);

    strip.show();
    delay(15);
  }

  // glowing animation
  int intensity = 0;
  while (intensity < BRIGHTNESS)
  {
    for (int i = 0; i < N_LEDS; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, intensity));
    }
    strip.show();
    delay(2);
    intensity++;
  }
  while (intensity >= 0)
  {
    for (int i = 0; i < N_LEDS; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, intensity));
    }
    strip.show();
    delay(2);
    intensity--;
  }
}
