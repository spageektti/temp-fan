#include <Adafruit_NeoPixel.h>

#define THERMISTOR_PIN 26  // GPIO pin for thermistor (ADC)
#define RELAY_PIN 2       
#define BUZZER_PIN 3      
#define NEOPIXEL_PIN 0    // DIN
#define NUMPIXELS 1       // bin ships only one

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const float BETA = 3950; // coefficient of the thermistor from wokwi docs

void setup() {
  pixels.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial1.begin(115200);

  digitalWrite(RELAY_PIN, LOW);  // fan off
  digitalWrite(BUZZER_PIN, LOW); // buzzer off
}

void loop() {
  int analogValue = analogRead(THERMISTOR_PIN);

  // like in wokwi docs
  float celsius = 1 / (log(1 / (4095.0 / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  Serial1.print("Temperature: ");
  Serial1.print(celsius);
  Serial1.println(" C");

  delay(1000);
}
