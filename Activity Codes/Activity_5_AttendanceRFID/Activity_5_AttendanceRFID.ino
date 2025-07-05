#include <SPI.h>
#include <MFRC522.h>

// RFID pins definition
#define SS_PIN 10     // Connect SDA/SS pin of RFID to GPIO-10
#define RST_PIN 9     // Not required but defined for MFRC522 library

// LED pins definition
#define RED_LED     0  // Connect Red LED pin to GPIO-0
#define YELLOW_LED  1  // Connect Yellow LED pin to GPIO-1
#define GREEN_LED   2  // Connect Green LED pin to GPIO-2

// RFID initialization
SPIClass SPI(0);
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
// Define valid card UIDs (maximum 5 cards)
// First valid card from example: A8 AB B1 12 (hex)
const byte VALID_CARDS[][4] = {
  {0xA8, 0xAB, 0xB1, 0x12},  // Card 1
  // Add more valid cards here as needed
};
const int NUM_VALID_CARDS = 1;  // Update when adding more cards

// Security tracking variables
int invalidAttempts = 0;
const int MAX_INVALID_ATTEMPTS = 3;  // Changed from 5 to 3
bool systemLocked = false;

// LED feedback timing
unsigned long lastCardTime = 0;
const unsigned long FEEDBACK_DURATION = 3000;  // 3 seconds

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize SPI bus and RFID reader
  SPI.begin();
  rfid.PCD_Init();
  
  // Initialize LED pins
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Turn all LEDs off initially
  allLedsOff();
  
  // Initialize RFID key
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  Serial.println(F("RFID Access Control System"));
  Serial.println(F("Present your card for authentication"));
}

void loop() {
  // System locked state (after too many invalid attempts)
  if (systemLocked) {
    allLedsOff();               // Clear all LEDs first
    digitalWrite(RED_LED, HIGH); // Turn on ONLY red LED
    return;                     // Exit loop until reset
  }
  
  // Check if we're currently in LED feedback mode (valid/invalid card was just shown)
  if (millis() - lastCardTime < FEEDBACK_DURATION && lastCardTime > 0) {
    // Still in feedback period, don't do anything
    return;
  } else if (lastCardTime > 0) {
    // Feedback period has ended, turn off all LEDs
    allLedsOff();
    lastCardTime = 0;
  }
  
  // Reset the loop if no new card present on the sensor/reader
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Check if the card UID matches any valid card
  bool isValidCard = false;
  for (int i = 0; i < NUM_VALID_CARDS; i++) {
    if (memcmp(rfid.uid.uidByte, VALID_CARDS[i], 4) == 0) {
      isValidCard = true;
      break;
    }
  }

  if (isValidCard) {
    Serial.println(F("Valid card detected."));
    digitalWrite(GREEN_LED, HIGH);  // Turn on green LED
    lastCardTime = millis();        // Start feedback timer
    invalidAttempts = 0;            // Reset invalid attempts
  } else {
    Serial.println(F("Invalid card detected."));
    invalidAttempts++;
    Serial.print(F("Invalid attempts: "));
    Serial.println(invalidAttempts);
    
    if (invalidAttempts >= MAX_INVALID_ATTEMPTS) {
      Serial.println(F("SECURITY ALERT: System locked!"));
      allLedsOff();                  // Clear all LEDs
      digitalWrite(RED_LED, HIGH);   // Turn on ONLY red LED
      systemLocked = true;           // Lock the system
    } else {
      digitalWrite(YELLOW_LED, HIGH); // Turn on yellow LED for feedback
      lastCardTime = millis();        // Start feedback timer
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
/**
 * Turn off all LEDs.
 */
void allLedsOff() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}
