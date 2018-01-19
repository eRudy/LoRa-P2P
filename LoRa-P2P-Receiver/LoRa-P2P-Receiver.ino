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


unsigned long previousMillis_led = 0;	// will store last time led was turned on

int counter = 0;			// messages counter

CRGB leds[NUM_LEDS];




void setup() {
	Serial.begin(BAUD_RATE);
	while (!Serial);


	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);




#ifdef ESP8266
	LoRa.setPins(16, 17, 15); // set CS, reset, IRQ pin
#endif

	Serial.println("LoRa Receiver");
	if (!LoRa.begin(868E6)) {
		Serial.println("Starting LoRa failed!");
		while (1);
	}
}






void loop() {

	unsigned long currentMillis = millis();

	// try to parse packet
	int packetSize = LoRa.parsePacket();
	if (packetSize) {
		// received a packet
		Serial.print("Received packet '");

		leds[0] = CRGB::Green;
		FastLED.show();

		// save the last time the led was turned on
		previousMillis_led = currentMillis;

		// read packet
		while (LoRa.available()) {
			Serial.print((char)LoRa.read());
		}

		// print RSSI of packet
		Serial.print("' with RSSI ");
		Serial.println(LoRa.packetRssi());

		counter++;
	}

	// turn off led after desired duration
	if (currentMillis - previousMillis_led >= led_duration) {
		leds[0] = CRGB::Black;
		FastLED.show();
	}

}
