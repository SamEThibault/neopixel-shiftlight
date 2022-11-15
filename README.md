# NeoPixels Shiftlight
Formula-style shift light built for the Queen's Racing team using Adafruit RGB NeoPixels Strip powered by Arduino R3 microcontroller.

The file is mostly modular and includes a quick starting animation. Dependencies required include the Adafruit NeoPixels library.

For the shift light to work for any application, users must specify the controller data pin used, the number of LEDs in the strip, the brightness (0-255), the engine speed's redline, and the ideal shift range in the initial `define` statements.

This application will be integrated with the car's CAN bus to receive current RPM values. For now, test values are used for demonstration purposes.