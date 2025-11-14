/*
 * ESP32 Relay Control via Serial
 * Pin 13: Relay Output
 * Baud Rate: 9600
 * Command: "ON" - Activate relay for 5 seconds
 */

const int RELAY_PIN = 13;
const unsigned long RELAY_DURATION = 5000; // 5 detik dalam milidetik

String inputString = "";
bool relayActive = false;
unsigned long relayStartTime = 0;

void setup() {
  // Inisialisasi Serial
  Serial.begin(9600);
  
  // Setup pin relay sebagai output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay OFF saat startup
  
  Serial.println("ESP32 Relay Controller Ready");
  Serial.println("Waiting for commands...");
}

void loop() {
  // Baca data dari serial
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      // Proses command saat menerima newline
      inputString.trim();
      
      if (inputString == "ON") {
        activateRelay();
      }
      
      // Clear string untuk command berikutnya
      inputString = "";
    } else {
      inputString += inChar;
    }
  }
  
  // Cek apakah relay perlu dimatikan
  if (relayActive && (millis() - relayStartTime >= RELAY_DURATION)) {
    deactivateRelay();
  }
}

void activateRelay() {
  if (!relayActive) {
    digitalWrite(RELAY_PIN, HIGH);
    relayActive = true;
    relayStartTime = millis();
    Serial.println("RELAY ON - 5 seconds");
  }
}

void deactivateRelay() {
  digitalWrite(RELAY_PIN, LOW);
  relayActive = false;
  Serial.println("RELAY OFF");
}