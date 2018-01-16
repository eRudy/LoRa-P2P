#include <SPI.h>
#include <LoRa.h>

#define FASTLED_INTERNAL // disable PRAGMA warnings
#include <FastLED.h>

#define LED_TYPE      WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS      2
#define DATA_PIN      0

#define BUTTON_PIN 2

int counter = 0;

CRGB leds[NUM_LEDS];

unsigned long previousMillis = 0;        // will store last time message was sent
unsigned long previousMillis_led = 0;        // will store last time led was turned on

const short intervals_msg[]={1000, 2000, 3000 , 5000};

// constants won't change:
short interval_msg = 3;           // interval at which to send messages (milliseconds)
const short interval_led = 1000;

void setup() {
  Serial.begin(9600);
  while (!Serial);

pinMode(BUTTON_PIN, INPUT);
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

void button_ISR() {
  interval_msg+=1;
  if (interval_msg>3) { interval_msg=0; }
}

void loop() {

  unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= intervals_msg[interval_msg]) {
   // save the last time you blinked the LED
   previousMillis = currentMillis;

   Serial.print("Sending packet: ");
   Serial.println(counter);

  leds[0] = CRGB::Blue;
  //leds[0] = CRGB::Red;
  FastLED.show();
  
  previousMillis_led=currentMillis;

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
 }

// turn off after delay
if (currentMillis - previousMillis_led >= interval_led) {
  leds[0] = CRGB::Black;
  FastLED.show();
}
  
}
