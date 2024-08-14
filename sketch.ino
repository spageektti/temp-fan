#include <Adafruit_NeoPixel.h>

#define THERMISTOR_PIN 26  // GPIO pin for thermistor (ADC)
#define RELAY_PIN 2       
#define BUZZER_PIN 3      
#define NEOPIXEL_PIN 4    // DIN
#define NUMPIXELS 1       // bin ships only one

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const float BETA = 3950; // coefficient of the thermistor from wokwi docs

const float FAN_THRESHOLD = 25.0; // temp to turn on the fan
const float BUZZER_THRESHOLD = 30.0; // temp to turn on the sound
unsigned long long lastBuzzerTime = 0;

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
  float celsius =  1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  unsigned long currentMillis = millis();
  Serial1.print("[");
  Serial1.print(currentMillis / 1000);
  Serial1.print("s] Temperature: ");
  Serial1.print(celsius);
  Serial1.println(" C");

  if (celsius >= FAN_THRESHOLD) {
    digitalWrite(RELAY_PIN, HIGH);  
  } else {
    digitalWrite(RELAY_PIN, LOW); 
  }

  if (celsius >= BUZZER_THRESHOLD) {
    unsigned long buzzerInterval = map(celsius, BUZZER_THRESHOLD, 40.0, 1000, 200); // higher temp = beep faster
    if (millis() - lastBuzzerTime >= buzzerInterval) {
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN)); 
      lastBuzzerTime = millis();
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (celsius < FAN_THRESHOLD) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  } else if (celsius >= FAN_THRESHOLD && celsius < BUZZER_THRESHOLD) {
    uint8_t greenValue = map(celsius, FAN_THRESHOLD, BUZZER_THRESHOLD, 255, 0);
    uint8_t redValue = map(celsius, FAN_THRESHOLD, BUZZER_THRESHOLD, 0, 255);
    pixels.setPixelColor(0, pixels.Color(redValue, greenValue, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  }
  
  pixels.show();

  delay(1000);
}

