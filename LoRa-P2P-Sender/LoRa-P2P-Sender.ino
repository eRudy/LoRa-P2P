#include <SPI.h>
#include <LoRa.h>

#define FASTLED_INTERNAL // disable PRAGMA warnings
#include <FastLED.h>

#define BAUD_RATE 9600

// we can solder leds on this cool PCB :) (https://github.com/hallard/WeMos-Lora)
#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      2
#define DATA_PIN      0

// and a switch button
#define BUTTON_PIN 2

const short led_duration = 1000;

unsigned long previousMillis = 0;	// will store last time message was sent
unsigned long previousMillis_led = 0;	// will store last time led was turned on

int counter = 0;			// messages counter

CRGB leds[NUM_LEDS];

const short intervals_msg[]={1000, 2000, 3000 , 5000};	// possible intervals to send messages
short interval_msg = 3;					// interval index

void setup() {
	Serial.begin(BAUD_RATE);
	while (!Serial);

	pinMode(BUTTON_PIN, INPUT);	// input switch
	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

	// Attach an interrupt to the ISR vector
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_ISR, FALLING);

#ifdef ESP8266
	LoRa.setPins(16, 17, 15); // set CS, reset, IRQ pin
#endif

	Serial.println("LoRa Sender");
	if (!LoRa.begin(868E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
}

void button_ISR() {	// called on button interruption
	interval_msg+=1;
	if (interval_msg>3) { interval_msg=0; }
}

void loop() {

	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= intervals_msg[interval_msg]) {
		// save the last time we sent a message
		previousMillis = currentMillis;

		Serial.print("Sending packet: ");
		Serial.println(counter);

		leds[0] = CRGB::Blue;
		FastLED.show();
  
		// save the last time the led was turned on
		previousMillis_led=previousMillis;

		// send packet
		LoRa.beginPacket();
		LoRa.print("hello ");
		LoRa.print(counter);
		LoRa.endPacket();

		counter++;
	}

	// turn off led after desired duration
	if (currentMillis - previousMillis_led >= led_duration) {
		leds[0] = CRGB::Black;
		FastLED.show();
	}
  
}
