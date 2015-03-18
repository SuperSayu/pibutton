/*
	We set pinmode more often than necessary because
	I use cron to schedule the daily event script, which
	overwrites the output pin mode.  The alternative is
	to actively poll the time in an attempt to 
*/
void set_pinmode() {
	pinMode(pin,INPUT);
	pullUpDnControl(pin,PUD_UP);
	if(lightpin >= 0) {
		pinMode(lightpin,OUTPUT);
		if(lightpwm)
			pinMode(lightpin,PWM_OUTPUT);
	}
}

void fade(int start, int finish, int periods) {
	int delta = (finish-start) / periods;
	int t_delta = blink_length * 100 / periods;
	int i;
	if(delta > 0)
		for(i=start; i<=finish; i+=delta) {
			pwmWrite(lightpin, i);
			delay(t_delta);
		}
	else
		for(i=start; i>=finish; i+=delta) {
			pwmWrite(lightpin, i);
			delay(t_delta);
		}
}

void light(int state) {
	if(lightpin < 0) return;
	if(!lightpwm) {
		digitalWrite(lightpin,state);
		return;
	}
	if(state)
		fade(pwm_min, pwm_max, blink_sections);
	else
		fade(pwm_max, pwm_min, blink_sections);
}

void blink() {
	set_pinmode();
	if(lightpin < 0) return;
	light(1);
	if(!lightpwm) delay(blink_length * 100);
	light(0);
}

