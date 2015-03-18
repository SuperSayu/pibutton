/*
	Button handling wait loop for raspberry pi internet of things printer kit
	or really any wiringpi-compatible button with optional light pin with optional pwm support
	For syntax see args.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wiringPi.h>

int pin = 4;
int lightpin = -1; // pin to light up, if any
int lightpwm = 0;
int killtime = 0; // time until "NONE", deciseconds
int holdmax = 70; // 7s to "HOLD"
int holdmin = 30; // 3s to "SHORT"
int maxtaps = 5;  // quintuple-tap
int timeout = 5; // 1s between taps
int debounce = 100; // 0.1s debounce
int blink_interval = 10; // time between blinks
int blink_length = 5; // fade in and fade out are each this long
int blink_sections = 10; // split fade into this many sections
int pwm_min = 2;
int pwm_max = 152;

volatile enum catch_mode {
	before_first = 0,
	first_down = 1,
	waiting_for_double = 2,
	double_down = 3
} mode = 0;
volatile char* result = "NONE";
volatile unsigned long kill_after = 0;

#include "light.h"
#include "catcher.h"
#include "args.h"


int main(int argc, char* argv[]) {
	parse_args(argc, argv);
	if (wiringPiSetup () < 0) {
//		fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
		return 1;
	}
	set_pinmode();
	if(killtime != 0) {
		killtime = millis() + killtime * 100;
	}
	if(maxtaps <= 1) timeout = 0; // this will ignore multitap logic
	if (wiringPiISR (pin, INT_EDGE_BOTH, &catcher) < 0) {
//		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
		return 1;
	}
	while(1) {
		if(killtime != 0 && killtime < millis()) break;
		delay(blink_interval * 100);
		if(mode == before_first)
			blink();
	}
	printf("%s",result);
	lightpwm = 0;
	set_pinmode();
	light(0);
	return 0;
}
