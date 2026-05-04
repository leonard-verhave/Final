#include <DHT.h>


#define DHTPIN 4  // The DHT11 data pin is connected to GPIO 4
#define DHTTYPE DHT11  // Specifying the sensor type as DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED Pin Assignments
const int bluePin = 19;  // Blue LED for cold weather
const int greenPin = 21; // Green LED for mild weather
const int redPin = 18;   // Red LED for hot weather

void setup() {
  Serial.begin(115200);
// Initialize the temperature sensor
  dht.begin();          

  // Set all LED pins as outputs
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  // Ensure all LEDs start in the "OFF" state
  digitalWrite(bluePin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
}

void loop() {

  // Read the temperature from the breadboard sensor in Fahrenheit
  float localTemp = dht.readTemperature(true); 
  
  // Send the local room temperature to the Thonny Shell
  Serial.print("Room Temp: "); 
  if (isnan(localTemp)) {
    Serial.println("Reading..."); // Displayed if wires are loose
  } else {
    Serial.println(localTemp); 
  }

  // --- STEP 2: CHECK FOR INTERNET DATA ---
  // Check if the Python script has sent a new temperature via USB
  if (Serial.available() > 0) {
    float incoming = Serial.parseFloat(); // Convert the incoming text to a number

    // STABILITY FILTER: Only update if the temperature is valid (above 1.0)
    if (incoming > 1.0) { 
      
      // Turn off the current LED before switching to the new one
      digitalWrite(bluePin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, LOW);

      if (incoming < 40.0) {
        digitalWrite(bluePin, HIGH); // COLD: Turn on Blue
      } else if (incoming > 80.0) {
        digitalWrite(redPin, HIGH);  // HOT: Turn on Red
      } else {
        digitalWrite(greenPin, HIGH); // MILD: Turn on Green 
      }
    }
  }
  
  // Wait 1 second before repeating the loop to keep the display readable
  delay(1000); 
}
