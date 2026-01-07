/*
 * ESP32 IR Sensor Test
 * Tests both digital and analog IR sensors
 * 
 * Connections:
 * - IR Sensor OUT pin -> GPIO pin (defined below)
 * - IR Sensor VCC -> 3.3V or 5V
 * - IR Sensor GND -> GND
 */

// Pin definitions
const int IR_DIGITAL_PIN = 2;  // Change to your digital IR sensor pin
const int IR_ANALOG_PIN = 34;  // Change to your analog IR sensor pin (ADC1 pins: 32-39)
const int LED_PIN = 2;         // Built-in LED on most ESP32 boards

// Variables
int digitalValue = 0;
int analogValue = 0;
bool objectDetected = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("ESP32 IR Sensor Test");
  Serial.println("====================");
  
  // Configure pins
  pinMode(IR_DIGITAL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Note: Analog pins don't need pinMode configuration on ESP32
  
  Serial.println("Setup complete. Reading sensors...\n");
}

void loop() {
  // Read digital IR sensor
  digitalValue = digitalRead(IR_DIGITAL_PIN);
  
  // Read analog IR sensor (ESP32 ADC: 0-4095 for 12-bit resolution)
  analogValue = analogRead(IR_ANALOG_PIN);
  
  // Check if object is detected (LOW typically means object detected for most IR sensors)
  objectDetected = (digitalValue == LOW);
  
  // Control LED based on detection
  digitalWrite(LED_PIN, objectDetected ? HIGH : LOW);
  
  // Print sensor readings
  Serial.print("Digital: ");
  Serial.print(digitalValue);
  Serial.print(" (");
  Serial.print(objectDetected ? "DETECTED" : "No object");
  Serial.print(")  |  Analog: ");
  Serial.print(analogValue);
  Serial.print(" / 4095");
  
  // Print visual bar for analog value
  Serial.print("  [");
  int barLength = map(analogValue, 0, 4095, 0, 20);
  for(int i = 0; i < 20; i++) {
    Serial.print(i < barLength ? "=" : " ");
  }
  Serial.println("]");
  
  delay(200);  // Update every 200ms
}