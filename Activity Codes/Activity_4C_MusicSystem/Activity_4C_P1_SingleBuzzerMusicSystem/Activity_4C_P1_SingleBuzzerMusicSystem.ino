
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

// Define Buzzer pin
#define BUZZER 1

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
  
  // Initialize buzzer pin
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
  // Turn off all LEDs initially
  digitalWrite(RED_LED, LOW);   // Add Red LED
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Enhanced Interactive Musical Light System");
  Serial.println("Two modes based on IR sensor detection:");
  Serial.println("1) IR detected: Pulsing tones and disco light sequence");
  Serial.println("2) IR not detected: Steady tones with corresponding LEDs");
  
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
  
  // Test buzzer with quick ascending notes
  for (int i = 100; i <= 300; i += 100) {
    digitalWrite(BUZZER, HIGH);
    delay(i);
    digitalWrite(BUZZER, LOW);
    delay(50);
  }
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

// Function to play a tone with specific frequency
void playTone(int frequency, int duration) {
  // Simple tone generation (approximation)
  // More precise frequencies would require a library or PWM
  int period = 1000000 / frequency; // Period in microseconds
  for (long i = 0; i < duration * 1000L / period; i++) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(period / 2);
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

// Function to generate pulsed tones based on active sensor(s)
void playPulsedTones() {
  unsigned long currentMillis = millis();
  
  // Determine which tone to play based on detected sensors
  if (distance2 < threshold && distance2 >= minValidDistance && 
      distance3 < threshold && distance3 >= minValidDistance) {
    // Both sensors - play alternating tones
    if (currentMillis - lastPulseChange > pulseOffTime) {
      if (!pulseState) {
        playTone(freq2, 50);  // Play short tone at frequency 2
        pulseState = true;
        lastPulseChange = currentMillis;
      } else {
        playTone(freq3, 50);  // Play short tone at frequency 3
        pulseState = false;
        lastPulseChange = currentMillis;
      }
    }
  }
  else if (distance2 < threshold && distance2 >= minValidDistance) {
    // Sensor 2 - pulse at lower frequency
    if (currentMillis - lastPulseChange > (pulseState ? pulseOnTime : pulseOffTime)) {
      pulseState = !pulseState;
      lastPulseChange = currentMillis;
      
      if (pulseState) {
        playTone(freq2, 50);  // Play short tone at frequency 2
      }
    }
  }
  else if (distance3 < threshold && distance3 >= minValidDistance) {
    // Sensor 3 - pulse at higher frequency
    if (currentMillis - lastPulseChange > (pulseState ? pulseOnTime : pulseOffTime)) {
      pulseState = !pulseState;
      lastPulseChange = currentMillis;
      
      if (pulseState) {
        playTone(freq3, 50);  // Play short tone at frequency 3
      }
    }
  }
  else {
    // No sensors - silence
    digitalWrite(BUZZER, LOW);
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
  
  // MODE 1: IR object detected - Disco lights and pulsed tones
  if (irDetected) {
    // Run disco light sequence
    runDiscoSequence();
    
    // Play pulsed tones based on which sensors are triggered
    playPulsedTones();
    
    Serial.println("MODE 1: Disco lights and pulsed tones");
  }
  // MODE 2: No IR object - Steady lights and continuous tones
  else {
    // Check sensors and play appropriate continuous tones
    if (distance2 < threshold && distance2 >= minValidDistance &&
        distance3 < threshold && distance3 >= minValidDistance) {
      // Both sensors - turn on both LEDs, play higher frequency
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      playTone(freq3, 50);  // Play higher frequency
      Serial.println("MODE 2: Both LEDs ON - Playing higher tone");
    }
    else if (distance2 < threshold && distance2 >= minValidDistance) {
      // Sensor 2 only - turn on Yellow LED, play lower frequency
      digitalWrite(YELLOW_LED, HIGH);
      playTone(freq2, 50);  // Play lower frequency
      Serial.println("MODE 2: YELLOW LED ON - Playing lower tone");
    }
    else if (distance3 < threshold && distance3 >= minValidDistance) {
      // Sensor 3 only - turn on Green LED, play higher frequency
      digitalWrite(GREEN_LED, HIGH);
      playTone(freq3, 50);  // Play higher frequency
      Serial.println("MODE 2: GREEN LED ON - Playing higher tone");
    }
    else {
      // No sensors triggered - silence
      digitalWrite(BUZZER, LOW);
    }
  }
  
  // Short delay for stability
  delay(5);
}
