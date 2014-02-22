const uint8_t  FREQ_PIN   = A2;
const uint8_t    TRIG_PIN = 7;
const uint8_t    ECHO_PIN = 6;
const uint8_t SPEAKER_PIN = 3;
const uint8_t REC_LED_PIN = 13;
const uint8_t REC_BTN_PIN = 4;

const long min_freq = 31;
const long DURATION = 280;

const int REC_BUFF_SIZE = 100;
int rec_end, rec_i;
int rec_buffer[REC_BUFF_SIZE];
int rec_buffer_base[REC_BUFF_SIZE];

void setup() {
        pinMode(SPEAKER_PIN, OUTPUT);
        pinMode(FREQ_PIN, INPUT);
	pinMode(ECHO_PIN, INPUT);
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(REC_LED_PIN, OUTPUT);
        pinMode(REC_BTN_PIN, INPUT);
        digitalWrite(REC_BTN_PIN, HIGH);

	rec_end = rec_i = 0;

        Serial.begin(9600);
}


long measure_distance() {
	long duration;

	digitalWrite(TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);

	duration = pulseIn(ECHO_PIN, HIGH);

	return duration / 58;
}


void playTone(int tone, int duration) {;
	for (long i = 0; i < duration * 1000L; i += tone * 2) {
	digitalWrite(SPEAKER_PIN, HIGH);
	delayMicroseconds(tone);
	digitalWrite(SPEAKER_PIN, LOW);
	delayMicroseconds(tone);
	}
}


long distance;

void record(long freq) {
	rec_buffer[rec_i] = freq;
	rec_i = (rec_i + 1) % REC_BUFF_SIZE;
	rec_end++;
	rec_end = rec_end < REC_BUFF_SIZE? rec_i : REC_BUFF_SIZE;
}

void play(long f, long duration) {
	//noTone(SPEAKER_PIN);
	//tone(SPEAKER_PIN, f, duration);

	playTone(f, duration);
}


long dist_to_freq(long distance, long freq_offset) {
	distance *= 58;
	distance >>= 1;
	return distance + freq_offset;
}

void loop() {
	bool button_state = digitalRead(REC_BTN_PIN) == LOW;
	long freq_offset  = analogRead(FREQ_PIN); // 0..1023 * 2
        Serial.println(button_state);
	distance = (3 * distance + measure_distance()) >> 2;
        if(button_state)
          digitalWrite(REC_LED_PIN, HIGH);
        else digitalWrite(REC_LED_PIN, LOW);    
      	if (distance <= 50) {		
		long f = dist_to_freq(distance, freq_offset);
		
		play(f, DURATION);
		
		if (button_state) {
                        Serial.println("DOWN");
		//	record(f);
		}
	} else {
 
		//noTone(SPEAKER_PIN);
		//if (button_state && distance > 350) {
		//	play(rec_buffer[rec_i], DURATION);
		//	rec_i = (rec_i + 1) % rec_end;
		//}
	}
}
