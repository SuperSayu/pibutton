## Pibutton helper app

This helper app was created for the [Adafruit Internet of Things Printer kit](http://www.adafruit.com/products/1289) but works for any situation in which you are using one pushbutton as your primary input mechanism.  This includes support for one-pin LED output for pushbuttons such as [the one included in the kit](http://www.adafruit.com/products/481), both binary on/off and PWM intensity regulation.

The fundamental mechanism of this helper is to output a text string to standard output that describes what sort of action the user took: One to five taps, short hold, long hold, or none (if a timeout is supplied, optional).  This can be used with any scripting language to form a main loop, such as the provided example.py.

This app requires the [WiringPi library](https://github.com/WiringPi/WiringPi).  The example main.py relies upon the [Adafruit Thermal Printer library](https://github.com/adafruit/python-thermal-printer).

This code is open source and freely distributable.
