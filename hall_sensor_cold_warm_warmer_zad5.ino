#include <TinkerKit.h>

const int RESET_BUTTON = 2;
const int SOUND_PIN    = 3;
const int LED_PIN      = LED_BUILTIN;

const int THRESHOLD    = 5;

////////////////////////////////////////////////////

TKHallSensor hs(I0);

int value_to_find;
unsigned long time;
unsigned long interval;

///////////////////////////////////////////////////


void reset() {
	digitalWrite(LED_PIN, LOW);
	time = millis();
	value_to_find = rand() % 500 + 100;
	Serial.print("!!!Value to find: ");
	Serial.println(value_to_find);
}


void setup() {
	Serial.begin(9600);
	while (!Serial) { ; }

	pinMode(RESET_BUTTON, INPUT);
	digitalWrite(RESET_BUTTON, HIGH); // use pull-up resisotr
	

	pinMode(SOUND_PIN, OUTPUT);
	pinMode(LED_PIN,   OUTPUT);

	reset();
}

 
int compute_interval(int distance) {
	return distance << 2;
}


void loop() {
	while (digitalRead(RESET_BUTTON) == LOW) {
		reset();
	}

	int val = hs.read();

	int distance_to_target = abs(val - value_to_find);

	interval = compute_interval(distance_to_target);
	if (millis() - time > interval) {
		tone(SOUND_PIN, 500, 10);
		time = millis();
	}
	
	if (distance_to_target < THRESHOLD) {
		Serial.println("HURA!! Znaleziono szukana wartosc");
		digitalWrite(LED_PIN, HIGH);
		tone(SOUND_PIN, 200, 1000);
		delay(2000);
		reset();
	} 
}
