#include <SPI.h>
#include <LoRa.h>

#define FASTLED_INTERNAL // disable PRAGMA warnings
#include <FastLED.h>

#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      2
#define DATA_PIN      0

CRGB leds[NUM_LEDS];

const long interval_led = 1000;
unsigned long previousMillis_led = 0;        // will store last time led was turned on

void setup() {
  Serial.begin(9600);
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

    previousMillis_led = currentMillis;
    leds[0] = CRGB::Green;
    FastLED.show();

    
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  // turn off after delay
if (currentMillis - previousMillis_led >= interval_led) {
  leds[0] = CRGB::Black;
  FastLED.show();
}

}
