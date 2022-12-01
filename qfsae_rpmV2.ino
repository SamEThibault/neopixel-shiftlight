/**
  Organization: Queen's Racing, 2022
  Program:      NeoPixel RPM Light Test
  Author:       Samuel Emard-Thibault
  Version:      Revision 1
  Date:         11/14/2022
**/

#include <Adafruit_NeoPixel.h>

#define BRIGHTNESS 50 // 0 to 255
#define PIN      6    // digital Arduino pin
#define N_LEDS 16     // number of pixels on strip
#define REDLINE 8000  // define the engine speed's limit

int idealShiftSpeed = REDLINE - 300;  // generalized, find real ideal speed using actual HP/TQ curves

// initialize strip object and colors
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t green = strip.Color(0, BRIGHTNESS, 0);
uint32_t red = strip.Color(BRIGHTNESS, 0, 0);
uint32_t blue = strip.Color(0, 0, BRIGHTNESS);

// test values
float currEngineSpeed[13] = {1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7000.0, 7300.0, 7400.0, 7450.0, 7500.0, 7750.0, 8000.0};

void setup() {
  strip.begin();
  startAnimation();
}

void loop() {
  // for each RPM input (test data)
  for (int i = 0; i < 13; i++)
  {
    // if we're at or past the ideal engine speed for upshifts, flash
    if (currEngineSpeed[i] >= idealShiftSpeed)
    {
      flash();
    } else
    {
      // else, find % of current engine speed, and map it to the percentage of pixels we want to light up
      int numPixels = (currEngineSpeed[i] / REDLINE) * N_LEDS;
      updateLEDS(numPixels);
    }
    delay(50);
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
    // if we're at least 60% to the redline, light up blue, else light up green
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
