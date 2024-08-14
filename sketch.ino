#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_NeoPixel.h>

#define ONE_WIRE_BUS 26   // data pin temp sensor
#define RELAY_PIN 2       
#define BUZZER_PIN 3      
#define NEOPIXEL_PIN 0    // DIN
#define NUMPIXELS 1       // bin ships only one

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  sensors.begin();

  pixels.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial1.begin(115200);

  digitalWrite(RELAY_PIN, LOW);  // fan off
  digitalWrite(BUZZER_PIN, LOW); // buzzer off
}

void loop() {
  sensors.requestTemperatures();
  
  float temperatureC = sensors.getTempCByIndex(0);

  Serial1.print("Temperature: ");
  Serial1.print(temperatureC);
  Serial1.println(" C");

  delay(1000);
}
