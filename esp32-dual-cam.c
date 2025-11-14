// ESP32 Relay Control
// Pin 13 connected to relay module

const int RELAY_PIN = 13;
const int RELAY_DURATION = 5000; // 5 seconds in milliseconds

unsigned long relayStartTime = 0;
bool relayActive = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay OFF initially
  
  Serial.println("ESP32 Relay Control Ready");
  Serial.println("Waiting for commands...");
}

void loop() {
  // Check for incoming serial data
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove whitespace
    
    if (command == "RELAY_ON") {
      // Activate relay
      digitalWrite(RELAY_PIN, HIGH);
      relayActive = true;
      relayStartTime = millis();
      Serial.println("Relay activated for 5 seconds");
    }
  }
  
  // Check if relay should be turned off
  if (relayActive && (millis() - relayStartTime >= RELAY_DURATION)) {
    digitalWrite(RELAY_PIN, LOW);
    relayActive = false;
    Serial.println("Relay deactivated");
  }
}
