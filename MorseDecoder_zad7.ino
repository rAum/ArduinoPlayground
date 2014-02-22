/// Mors Encoder
/// Systemy wbudowane * 2013
///////////////////////////////

#include "MorsTabela.h"

const int LED_PIN = 9;
const int SPK_PIN = 8;

const int DOT_DELAY   = 100;
const int DASH_DELAY  = DOT_DELAY * 3; // kreska musi trwac co najmniej 3x kropka
const int SPACE_DELAY = DASH_DELAY;  // miedzy pojedynczymi znakami
const int WORD_DELAY  = DASH_DELAY * 3; // miedzy wyrazami

int SOUND_FREQ  = 600;

/////////////////////////////////////////////

void emit_(const int duration) {
	tone(SPK_PIN, SOUND_FREQ); digitalWrite(LED_PIN, HIGH);
	delay(duration);
	noTone(SPK_PIN); digitalWrite(LED_PIN, LOW);
}

const char* lookup(int c) {
	
	if (isdigit(c)) return number_table[c - '0'];
	if (isalpha(c)) return letter_table[toLowerCase(c) - 'a'];
	if (c == ' ')   return " ";
	return "\0";
}

void emit_signal(int c) {
	const char* s = lookup(c);
	Serial.print("nadaje: ");
	Serial.println(s);
	while(*s != '\0') {
		switch(*(s++)) {
		case '.': emit_(DOT_DELAY);  break;
		case '-': emit_(DASH_DELAY); break;
		case ' ': delay(WORD_DELAY); continue;
		default:; 
		}
		delay(SPACE_DELAY);
	}
}

//////////////////////////////////////////////

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(SPK_PIN, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	//digitalWrite(LED_PIN, HIGH);
	//SOUND_FREQ = analogRead(1)+50;
	while (Serial.available() > 0) {
		int r = Serial.read();
		//Serial.print((char)r);
		emit_signal(r);
	}
}
