
// Define Ultrasonic Sensor pins
#define TRIG_PIN2 3
#define ECHO_PIN2 4
#define TRIG_PIN3 5
#define ECHO_PIN3 6

// Define IR sensor pin
#define IR_SENSOR 11

// Define LED pins
#define RED_LED 9    // Add back the Red LED
#define YELLOW_LED 8
#define GREEN_LED 7

// Define Buzzer pins
#define BUZZER1 12  // First buzzer for Sensor 2
#define BUZZER2 13  // Second buzzer for Sensor 3

// Define musical notes for better melodies
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

// Melody for Sensor 2 (Yellow LED) - "Charge" fanfare
const int yellowMelodySize = 6;
const int yellowMelody[yellowMelodySize] = {NOTE_C4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_C5};
const int yellowDurations[yellowMelodySize] = {100, 100, 100, 100, 100, 200};
int yellowNoteIndex = 0;
unsigned long lastYellowNoteTime = 0;

// Melody for Sensor 3 (Green LED) - "Star Wars" theme hint
const int greenMelodySize = 5;
const int greenMelody[greenMelodySize] = {NOTE_G4, NOTE_G4, NOTE_G4, NOTE_D4, NOTE_B4};
const int greenDurations[greenMelodySize] = {130, 130, 130, 100, 200};
int greenNoteIndex = 0;
unsigned long lastGreenNoteTime = 0;

// Harmony patterns for dual buzzer mode
const int harmonySize = 4;
const int harmonyMelody1[harmonySize] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5};
const int harmonyMelody2[harmonySize] = {NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5};
const int harmonyDurations[harmonySize] = {120, 120, 120, 200};
int harmonyIndex = 0;
unsigned long lastHarmonyTime = 0;

// Variables for distance measurement
long duration2, duration3;
int distance2, distance3;
int threshold = 9; // Threshold distance in cm
int minValidDistance = 2; // Minimum valid distance (cm) to filter out false readings

// Variables for mode control and timing
boolean irDetected = false;
unsigned long lastLEDToggle = 0;
unsigned long lastArpeggioChange = 0;
boolean ledState = false;
unsigned long discoInterval = 100; 
unsigned long arpeggioInterval = 150;
int activeNote = 0; // 0: no note, 1: note from sensor 2, 2: note from sensor 3

// Variable for disco mode LED sequencing
int discoStep = 0;  // 0: Red, 1: Yellow, 2: Green
unsigned long lastDiscoStep = 0;
unsigned long discoStepInterval = 150;  // Time between LED changes in disco mode

// Variables for pulsing tones when IR detected
unsigned long lastPulseChange = 0;
boolean pulseState = false;
unsigned long pulseOnTime = 200;   // Tone on time
unsigned long pulseOffTime = 100;  // Tone off time

// Variables for tone frequencies
int freq2 = 500;  // Lower frequency for sensor 2 (Hz)
int freq3 = 1000; // Higher frequency for sensor 3 (Hz)

void setup() {
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
  
  // Initialize IR sensor pin
  pinMode(IR_SENSOR, INPUT);
  
  // Initialize LED pins
  pinMode(RED_LED, OUTPUT);    // Add Red LED initialization
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Initialize buzzer pins
  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);
  digitalWrite(BUZZER1, LOW);
  digitalWrite(BUZZER2, LOW);
  
  // Turn off all LEDs initially
  digitalWrite(RED_LED, LOW);   // Add Red LED
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Dual Buzzer Interactive Musical Light System");
  Serial.println("Two modes based on IR sensor detection:");
  Serial.println("1) IR detected: Disco lights and stereo music effects");
  Serial.println("2) IR not detected: Steady lights and dedicated buzzer melodies");
  
  // Startup sequence to test components
  testComponents();
}

void testComponents() {
  // Test LEDs in sequence
  digitalWrite(RED_LED, HIGH);
  delay(200);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  delay(200);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(200);
  digitalWrite(GREEN_LED, LOW);
  
  // Test both buzzers
  Serial.println("Testing Buzzer 1");
  playTone(BUZZER1, 440, 200);
  delay(300);
  
  Serial.println("Testing Buzzer 2");
  playTone(BUZZER2, 587, 200);
  delay(300);
  
  Serial.println("Testing Dual Buzzer Harmony");
  playDualTone(BUZZER1, 440, BUZZER2, 659, 300);
  delay(100);
}

// Function to measure distance from an ultrasonic sensor
int measureDistance(int trigPin, int echoPin) {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set trigPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin, returns sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 25000); // Add timeout of 25ms
  
  // Check if reading timed out (returned 0)
  if (duration == 0) {
    return 400; // Return a large value (no object detected)
  }
  
  // Calculate distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Filter out unreasonable values
  if (distance < minValidDistance || distance > 400) {
    return 400; // Return a large value for invalid readings
  }
  
  return distance;
}

// Function to play a tone on a specific buzzer
void playTone(int buzzer, int frequency, int duration) {
  // Simple tone generation with specified buzzer
  int period = 1000000 / frequency; // Period in microseconds
  for (long i = 0; i < duration * 1000L / period; i++) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(period / 2);
  }
}

// Function to play two tones simultaneously on both buzzers (harmony)
void playDualTone(int buzzer1, int freq1, int buzzer2, int freq2, int duration) {
  // Approximate dual-tone generation
  long cycles = duration * 1000L / 2000; // Number of 2ms cycles
  int period1 = 1000000 / freq1;
  int period2 = 1000000 / freq2;
  
  for (long i = 0; i < cycles; i++) {
    // Generate a short burst of each frequency
    for (int j = 0; j < 10; j++) {
      digitalWrite(buzzer1, HIGH);
      delayMicroseconds(period1 / 2);
      digitalWrite(buzzer1, LOW);
      delayMicroseconds(period1 / 2);
    }
    
    for (int j = 0; j < 10; j++) {
      digitalWrite(buzzer2, HIGH);
      delayMicroseconds(period2 / 2);
      digitalWrite(buzzer2, LOW);
      delayMicroseconds(period2 / 2);
    }
  }
}

// Function to play the yellow sensor melody on buzzer 1
void playYellowMelody() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastYellowNoteTime > 200) {
    lastYellowNoteTime = currentTime;
    
    // Play current note on buzzer 1
    playTone(BUZZER1, yellowMelody[yellowNoteIndex], yellowDurations[yellowNoteIndex]);
    
    // Move to next note or reset to beginning
    yellowNoteIndex = (yellowNoteIndex + 1) % yellowMelodySize;
  }
}

// Function to play the green sensor melody on buzzer 2
void playGreenMelody() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastGreenNoteTime > 200) {
    lastGreenNoteTime = currentTime;
    
    // Play current note on buzzer 2
    if (greenMelody[greenNoteIndex] > 0) {
      playTone(BUZZER2, greenMelody[greenNoteIndex], greenDurations[greenNoteIndex]);
    }
    
    // Move to next note or reset to beginning
    greenNoteIndex = (greenNoteIndex + 1) % greenMelodySize;
  }
}

// Function to play harmony when both sensors are triggered
void playHarmonyMelody() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastHarmonyTime > 300) {
    lastHarmonyTime = currentTime;
    
    // Play harmony notes on both buzzers
    playDualTone(BUZZER1, harmonyMelody1[harmonyIndex], 
                BUZZER2, harmonyMelody2[harmonyIndex], 
                harmonyDurations[harmonyIndex]);
    
    // Move to next note pair
    harmonyIndex = (harmonyIndex + 1) % harmonySize;
  }
}

// Function to turn off all LEDs
void allLedsOff() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

// Function to run the disco light sequence (when IR detects)
void runDiscoSequence() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastDiscoStep > discoStepInterval) {
    lastDiscoStep = currentMillis;
    
    // Turn off all LEDs
    allLedsOff();
    
    // Move to next LED in sequence
    discoStep = (discoStep + 1) % 3;
    
    // Turn on the current LED in the sequence
    switch (discoStep) {
      case 0:
        digitalWrite(RED_LED, HIGH);
        break;
      case 1:
        digitalWrite(YELLOW_LED, HIGH);
        break;
      case 2:
        digitalWrite(GREEN_LED, HIGH);
        break;
    }
  }
}

// Function for stereo pulsed tones in disco mode
void playStereoPulsedTones() {
  unsigned long currentMillis = millis();
  
  // Check which sensors are triggered
  if (distance2 < threshold && distance2 >= minValidDistance && 
      distance3 < threshold && distance3 >= minValidDistance) {
    // Both sensors - stereo ping-pong effect
    if (currentMillis - lastPulseChange > pulseOffTime) {
      lastPulseChange = currentMillis;
      
      if (!pulseState) {
        // Left to right
        playTone(BUZZER1, freq2, 50);
        delay(50);
        playTone(BUZZER2, freq3, 50);
      } else {
        // Right to left
        playTone(BUZZER2, freq3, 50);
        delay(50);
        playTone(BUZZER1, freq2, 50);
      }
      
      pulseState = !pulseState;
    }
  }
  else if (distance2 < threshold && distance2 >= minValidDistance) {
    // Only sensor 2 - pulse buzzer 1
    if (currentMillis - lastPulseChange > (pulseState ? pulseOnTime : pulseOffTime)) {
      pulseState = !pulseState;
      lastPulseChange = currentMillis;
      
      if (pulseState) {
        playTone(BUZZER1, freq2, 50);
      }
    }
  }
  else if (distance3 < threshold && distance3 >= minValidDistance) {
    // Only sensor 3 - pulse buzzer 2
    if (currentMillis - lastPulseChange > (pulseState ? pulseOnTime : pulseOffTime)) {
      pulseState = !pulseState;
      lastPulseChange = currentMillis;
      
      if (pulseState) {
        playTone(BUZZER2, freq3, 50);
      }
    }
  }
}

void loop() {
  // Measure distances from ultrasonic sensors
  distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);
  distance3 = measureDistance(TRIG_PIN3, ECHO_PIN3);
  
  // Check IR sensor - NOTE: Logic is now inverted from previous implementation
  irDetected = digitalRead(IR_SENSOR) == LOW; // Assuming IR sensor outputs LOW when object detected
  
  // Print debug info
  Serial.print("Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm | Distance 3: ");
  Serial.print(distance3);
  Serial.print(" cm | IR: ");
  Serial.println(irDetected ? "Detected" : "Not Detected");
  
  // Turn off all LEDs initially
  allLedsOff();
  
  // MODE 1: IR object detected - Disco lights and stereo pulsed tones
  if (irDetected) {
    // Run disco light sequence
    runDiscoSequence();
    
    // Play stereo pulsed tones based on which sensors are triggered
    playStereoPulsedTones();
    
    Serial.println("MODE 1: Disco lights and stereo pulsed tones");
  }
  // MODE 2: No IR object - Steady lights and continuous melodies
  else {
    // Check sensors and play appropriate continuous tones
    if (distance2 < threshold && distance2 >= minValidDistance &&
        distance3 < threshold && distance3 >= minValidDistance) {
      // Both sensors - turn on both LEDs, play harmony
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      playHarmonyMelody();
      Serial.println("MODE 2: Both LEDs ON - Playing harmony on both buzzers");
    }
    else if (distance2 < threshold && distance2 >= minValidDistance) {
      // Sensor 2 only - turn on Yellow LED, play yellow melody on buzzer 1
      digitalWrite(YELLOW_LED, HIGH);
      playYellowMelody();
      Serial.println("MODE 2: YELLOW LED ON - Playing melody on buzzer 1");
    }
    else if (distance3 < threshold && distance3 >= minValidDistance) {
      // Sensor 3 only - turn on Green LED, play green melody on buzzer 2
      digitalWrite(GREEN_LED, HIGH);
      playGreenMelody();
      Serial.println("MODE 2: GREEN LED ON - Playing melody on buzzer 2");
    }
    else {
      // No sensors triggered - silence
      digitalWrite(BUZZER1, LOW);
      digitalWrite(BUZZER2, LOW);
    }
  }
  
  // Short delay for stability
  delay(5);
}
