// Language: Arduino C++

#include <SoftwareSerial.h>

// GSM module pins
SoftwareSerial mySerial(9, 10); // RX, TX

// User & Twilio numbers (Twilio not used in Arduino)
String userNumber = "+917671078028";

// Sensor and actuator pins
const int flamePin = A0;
const int tempPin = A1;
const int motorPin1 = 3;
const int motorPin2 = 4;
const int pumpPin = 5;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(flamePin, INPUT);
  pinMode(tempPin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  int flameValue = analogRead(flamePin);   // Read flame sensor
  int tempValue = analogRead(tempPin);     // Read temperature sensor
  float temperature = (tempValue * 5.0 / 1024.0) * 100.0; // Convert to °C

  // Fire detection condition
  if (flameValue < 500 || temperature > 50) {
    // Activate motors and pump
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(pumpPin, HIGH);

    sendAlert(); // Send SMS alert

    delay(5000); // Keep pump running for 5 sec

    // Turn off motors and pump
    digitalWrite(pumpPin, LOW);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
}

void sendAlert() {
  mySerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  mySerial.println("AT+CMGS=\"" + userNumber + "\""); // Set recipient
  delay(1000);
  mySerial.println("Fire Detected! Taking Action."); // SMS content
  delay(100);
  mySerial.println((char)26); // End SMS with Ctrl+Z
  delay(1000);
}
