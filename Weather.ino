#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int bluePin = 5;    // Cold (<40F)
const int greenPin = 21;  // Nice (40-70F)
const int redPin = 18;    // Hot (>80F)

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
}

void loop() {
  float localTemp = dht.readTemperature(true); // Read room temp in Fahrenheit
  
  if (!isnan(localTemp)) {
    Serial.print("ROOM_TEMP:"); 
    Serial.println(localTemp); // Send room temp to Python
  }

  if (Serial.available() > 0) {
    float cheyenneTemp = Serial.parseFloat(); // Receive weather from Python
    
    // Reset LEDs
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, LOW);
    
    // Light up based on Cheyenne weather
    if (cheyenneTemp < 40) digitalWrite(bluePin, HIGH);
    else if (cheyenneTemp > 80) digitalWrite(redPin, HIGH);
    else digitalWrite(greenPin, HIGH);
  }
  delay(2000); 
}