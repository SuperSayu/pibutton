
void catcher(void) {
	static unsigned long next_valid = 0;
	static int value = 0;
	static int taps = 0;
	static int b_start = 0;
	if(millis() < next_valid) return;
	next_valid = millis() + debounce;
	value = !value;
	lightpwm = 0;
	set_pinmode();
	light(value);
	switch(mode) {
		case before_first:
			mode = first_down;
			b_start = millis();
			if(holdmax > 0)
				result = "HOLD"; // if you die in this state, it was because you held until holdmax
				killtime = millis() + holdmax * 100;
			break;
		case first_down:
			mode = waiting_for_double;
			unsigned long total_time = millis() - b_start;
			if(holdmin <= 0 || total_time < (holdmin * 100)) result = "TAP";
			else result = "SHORT";
			killtime = millis() + timeout * 100;
			taps = 1;
			break;
		case waiting_for_double:
			mode = double_down;
			taps++;
			killtime = millis() + timeout * 100;
			switch(taps){
				case 1: result = "TAP"; break;
				case 2: result = "DOUBLE"; break;
				case 3: result = "TRIPLE"; break;
				case 4: result = "QUADRUPLE"; break;
				case 5: result = "QUINTUPLE"; break;
				default: printf("MULTI%i",taps);
			}
			break;
		case double_down:
			mode = waiting_for_double;
			killtime = millis() + timeout * 100;
			break;
	}
}
