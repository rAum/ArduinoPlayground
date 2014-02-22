const int LED_PIN = LED_BUILTIN;
const int SPK_PIN = 3;
const int BTN_PIN = 2;

const int BOUNCE_THRESHOLD = 50;
const int DOT_DELAY        = 300;
const int DASH_DELAY       = DOT_DELAY * 3; 
const int SPACE_DELAY      = DOT_DELAY;     // between da & dah 
const int WORD_DELAY       = DOT_DELAY * 6;                // between words

///////////////////////////////////////////////////////////

bool prev_button_state;
unsigned long timer_signal;
unsigned short sequence;

////////////////////////////////////////////////////////

inline void timer_start()          { timer_signal = millis(); }
inline unsigned long timer_diff()  { return millis() - timer_signal; }

void setup() {
	Serial.begin(9600);
	while (!Serial) { ; }

	pinMode(BTN_PIN, INPUT);
	digitalWrite(BTN_PIN, HIGH); // enable pullup resistor 

	pinMode(SPK_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);

	digitalWrite(LED_PIN, LOW);
	prev_button_state = false;

	sequence = 1;

	Serial.println("=:[READY]:=");
	
	timer_start();
}

void show_state(int state) {
	if (state) {
		digitalWrite(LED_PIN, HIGH);
		tone(SPK_PIN, 700);
	} else {
		digitalWrite(LED_PIN, LOW);
		noTone(SPK_PIN);
	}
}

const char decode_table[] ="##TEMNAIOGKDWRUS##QZYCXBJP#L#FVH09#8###7#######61#######2###3#45";

void translate_and_send() {
	char character;
	if (sequence <= 1) return;

	if (sequence < sizeof(decode_table)/sizeof(decode_table[0]))
		character = decode_table[sequence];
	else {
		switch (sequence) {
		case 71:  character = ':'; break;
		case 76:  character = ','; break;
		case 84:  character = '!'; break;
		case 94:  character = '-'; break;
		case 101: character = '@'; break;
		case 106: character = '.'; break;
		case 115: character = '?'; break;
		default:  character = '#'; break;
		}
	}
	Serial.print(character);
	sequence = 1;
}

void send_end_word() {
	translate_and_send();                                 // translate, if anything left
	Serial.print(" ");                                    // send word break
}

void loop() {
	bool button_state = digitalRead(BTN_PIN) == LOW;      // button_state == true -> then button is pressed
	
	unsigned long elapsedTime = timer_diff();
	
	if (elapsedTime < BOUNCE_THRESHOLD)                   // if the time is too short, then don't do anything.
		return;

	show_state(button_state);

	if (button_state != prev_button_state) {
		if (button_state) {                               // silent -> on
			if (elapsedTime > WORD_DELAY) {				
				send_end_word();
				timer_start();
			}
			else if (elapsedTime >= SPACE_DELAY) {
				translate_and_send();
				timer_start();
			}			                                  // else don't do anything, wait for another da-dah
		}
		else {                                            // on -> silent, 1 is da (.), 0 is dah (-), encoding in binary
			if (elapsedTime >= DASH_DELAY) {
				sequence <<= 1;
				Serial.print('-');
				timer_start();
			} else if (elapsedTime >= DOT_DELAY){
				Serial.print('.');
				sequence <<= 1;
				sequence += 1;
				timer_start();
			}
			
		}

		prev_button_state = button_state;
	}

	if (button_state == false && elapsedTime > WORD_DELAY) { // flush character, if any.
		translate_and_send();
	}
}
