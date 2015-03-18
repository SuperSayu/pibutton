//
// These are the arguments to the program.  All arrays are ordered the same.
// All times in 1/10 seconds (except debounce)
//
// Pin (argument without switch): the pin the button is on.
// Kill-time: Timeout after which the button script will return "NONE" (unless processing a button press)
// Hold threshhold: After holding the button this long, the script will return "HOLD"
// Short-press threshhold: Presses shorter than this trigger "TAP" (or multi-taps), longer returns "SHORT" or HOLD"
// Maximum taps: The multi-tap logic will trigger until the number of taps equals or exceeds this number
// Multitap-timeout: The multi-tap logic will wait this long before giving up on the next tap.
// Debounce-time: Necessary to avoid multiple readings from non-debounced switches.  Adjustable just in case.  https://www.newbiehack.com/UnderstandingButtonDebouncing.aspx

#include <malloc.h>


int n_argcodes = 10;
char codes[] = { ' ', 'k', 'h', 's', 'm', 't', 'D', 'l', 'b', 'p' };
char* shortdesc[] = { "pin", "kill time", "hold max", "hold min", "max taps", "tap timeout", "debounce", "lightpin", "blink interval", "light PWM time" };
char* desc[] = {
	"The pin that the button is on.",
	"If set, after this many deciseconds the program will return NONE.",
	"If the first button press is longer than this many deciseconds, the program immediately returns HOLD",
	"If the first button press is longer than this many deciseconds, but less than the hold max, the program will return SHORT; otherwise it is a TAP.",
	"If the first button press is a tap, and this is greater than one, the program will wait multitap-timeout deciseconds looking for the next tap.",
	"If the multi-tap logic has activated, it waits this long for the next tap.",
	"Time period in milliseconds in which multiple presses are ignored.",
	"Pin corresponding to the button indicator LED, if any.",
	"Period of time between light blinks, in deciseconds",
	"If set, the light will fade in over this many deciseconds (and out over the same length of time).  The output pin must be PWM-enabled."
};

struct arg {
	char code;
	int value;
};

#define NUL (struct arg*)(0)
#define ERR (struct arg*)(1)

void list_args(int argc, char* argv[], int wrong) {
	int i;
	for(i=0;i < argc; i++)
		if(i == wrong)
			fprintf(stderr, "[%s] ", argv[i]);
		else
			fprintf(stderr, "%s ", argv[i]);
	fprintf(stderr, "\nUsage: %s ", argv[0]);
	
	for(i=1; i < n_argcodes; i++)
		fprintf(stderr, "[-%c %s] ", codes[i], shortdesc[i]);
	fprintf(stderr,"[%s]\n",shortdesc[0]);
	for(i=0;i < n_argcodes; i++)
		fprintf(stderr, "%15s:\t%s\n",shortdesc[i],desc[i]);
	exit(1);
}

struct arg* next_arg(int argc, char* argv[], int* current) {
	if(++(*current) >= argc) return NUL;
	char* arg = argv[*current];
	if(arg[0] >= '0' && arg[0] <= '9') {
		struct arg* ptr = malloc(sizeof(struct arg));
		ptr->code = ' ';
		ptr->value = atoi(arg);
		return ptr;
	}
	if(arg[0] != '-') return ERR;
	if(arg[1]==0) return ERR; // pibutton -
	char *val;
	if( arg[2] == '=' )
		val = arg + 3; // -l=1
	else {
		if(++(*current) >= argc) return ERR;
		val = argv[*current];
	}
	if(val[0] < '0' || val[0] > '9') return ERR;
	int i;
	for(i=1; i<n_argcodes; i++) {
		if(codes[i] != arg[1]) continue;
		struct arg* ptr = malloc(sizeof(struct arg));
		ptr->code = arg[1];
		ptr->value = atoi(val);
		return ptr;
	}
	return ERR; // invalid argcode or n_argcodes is too low
}
void parse_args(int argc, char* argv[]) {
	int i = 0;
	struct arg* ptr;
	while(1) {
		ptr = next_arg(argc, argv, &i);
		if(ptr == NUL) return; // eaten them all
		if(ptr == ERR) { // bad argument
			list_args(argc, argv, i);
			exit(1);
		}
		
		if(ptr->value == -1) continue; // not set
		//fprintf(stderr,"%c: %i",ptr->code,ptr->value);
		switch(ptr->code) {
			case ' ': pin = ptr->value; break;
			case 'k': killtime = ptr->value; break;
			case 'h': holdmax = ptr->value; break;
			case 's': holdmin = ptr->value; break;
			case 'm': maxtaps = ptr->value; break;
			case 't': timeout = ptr->value; break;
			case 'd': debounce = ptr->value; break;
			case 'l': lightpin = ptr->value; break;
			case 'b': blink_interval = ptr->value; break;
			case 'p': lightpwm = 1; blink_length = ptr->value; break;
		}
	}
}

