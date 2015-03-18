#!/usr/bin/python

# This is an example main loop using the pibutton helper.
# This assumes you are using the Adafruit internet of things printer,
# although you can change it to have whatever other output you like.

# The pibutton app requires root because the library it uses does.
# That means this script needs root access, or the executable needs
# root ownership and suid, which I cannot officially recommend.
# Shutdown and other tasks similarly need superuser access.

import subprocess
from Adafruit_Thermal import *

button_path = "./pibutton" # compiled executable
pin=4 			# button input pin
lightpin=1		# button light pin: -1 to disable
light_pwm=0		# set to 1 if the light pin is on a pwm-enabled pin, otherwise 0


# -s: after this many 1/10 sec, tap becomes short hold (1/2 sec)
# -h: after this many 1/10 sec, short hold becomes hold and pibutton terminates (5s)
# Input pin has no switch before it.
button_args = [button_path, "-s=5", "-h=50", str(pin)]
# -l: light pin
# -p: use pwm (required argument: fade in time)
# -b: period between blinks
lightpin_args = [ "-b=13", "-l", str(lightpin) ]
light_pwm_args = [ "-p=11" ]

if lightpin > -1:
	button_args = button_args + lightpin_args
	if light_pwm >= 1:
		button_args = button_args + light_pwm_args

		
def unused(printer):
	return

def show_args(printer):
	args = [
		"Short hold (1-3 sec): Show help",
		"One tap: Nothing",
		"Two taps: Nothing",
		"Three taps: Nothing",
		"Four taps: Nothing",
		"Five or more taps: Nothing",
		"Long hold (5+ sec): Shutdown"
	]
	for arg in args:
		printer.println(arg)
	printer.feed(1)
	printer.println("My IP is " + getIP())
	printer.feed(1)

def shutdown(printer):
	print("Long button hold")
	printer.println("Showdown printer?")
	printer.println("Double-tap the button to confirm")
	printer.feed(2)
	if getButton(50) == "DOUBLE":
		printer.println("Shutting down, please wait")
		printer.feed(3)
		subprocess.call(["shutdown","-h","now"])
	else:
		printer.println("Okay, not yet")
		printer.feed(2)

def getButton(killtime="0"):
	p = subprocess.Popen(button_args, stdout=subprocess.PIPE)
	p.wait() # this is unsafe I am told
	return p.communicate()[0]
		
def getIP():
	p = subprocess.Popen(["hostname", "-I"], stdout=subprocess.PIPE)
	p.wait() # this is unsafe I am told
	return p.communicate()[0]

	

# Keep this up to date
button_funcs = {
	"SHORT"		: show_args,
	"HOLD"		: shutdown,
	"TAP"		: unused,
	"DOUBLE"	: unused,
	"TRIPLE"	: unused,
	"QUADRUPLE"	: unused,
	"QUINTUPLE"	: unused
#	"MULTI6"	: unused # etc
}


# If executed rather than included
if __name__ == "__main__":
	printer=Adafruit_Thermal()

	while 1:
		action = getButton()
		print(action) # In case you are monitoring the terminal
		button_funcs.get(action,unused)(printer) # call func or unused()
		






