/*
 * ESP32 Relay Control for Dual Camera X-Ray Detection System
 * Pin 13: Relay Output
 * Baud Rate: 9600
 * Command: "ON" - Activate relay for 5 seconds
 * 
 * Wiring:
 * ESP32 Pin 13 -> Relay IN
 * ESP32 GND    -> Relay GND
 * ESP32 5V/3V  -> Relay VCC (sesuaikan dengan relay module)
 */

const int RELAY_PIN = 13;
const unsigned long RELAY_DURATION = 5000; // 5 detik dalam milidetik

String inputString = "";
bool relayActive = false;
unsigned long relayStartTime = 0;
int triggerCount = 0;

void setup() {
  // Inisialisasi Serial
  Serial.begin(9600);
  
  // Setup pin relay sebagai output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay OFF saat startup
  
  // LED builtin untuk indikator (opsional)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Startup message
  Serial.println("====================================");
  Serial.println("ESP32 Dual Camera Relay Controller");
  Serial.println("====================================");
  Serial.println("System Ready");
  Serial.println("Relay Pin: 13");
  Serial.println("Waiting for commands...");
  Serial.println("====================================");
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
      else if (inputString == "STATUS") {
        printStatus();
      }
      else if (inputString == "RESET") {
        resetCounter();
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
  
  // Blink LED saat relay aktif (visual indicator)
  if (relayActive) {
    digitalWrite(LED_BUILTIN, (millis() / 250) % 2);
  }
}

void activateRelay() {
  if (!relayActive) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    relayActive = true;
    relayStartTime = millis();
    triggerCount++;
    
    Serial.println(">>> RELAY ACTIVATED <<<");
    Serial.print("Trigger Count: ");
    Serial.println(triggerCount);
    Serial.println("Duration: 5 seconds");
    Serial.println("---");
  } else {
    Serial.println("Relay already active, ignoring command");
  }
}

void deactivateRelay() {
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  relayActive = false;
  
  Serial.println(">>> RELAY DEACTIVATED <<<");
  Serial.println("System ready for next detection");
  Serial.println("---");
}

void printStatus() {
  Serial.println("====================================");
  Serial.println("SYSTEM STATUS");
  Serial.println("====================================");
  Serial.print("Relay State: ");
  Serial.println(relayActive ? "ACTIVE" : "INACTIVE");
  Serial.print("Total Triggers: ");
  Serial.println(triggerCount);
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  Serial.println("====================================");
}

void resetCounter() {
  triggerCount = 0;
  Serial.println("Trigger counter reset to 0");
}