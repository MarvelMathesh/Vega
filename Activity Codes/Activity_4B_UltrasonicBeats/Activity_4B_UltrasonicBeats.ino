// Define Ultrasonic Sensor pins
#define TRIG_PIN2 3
#define ECHO_PIN2 4
#define TRIG_PIN3 5
#define ECHO_PIN3 6

// Define LED pins
#define YELLOW_LED 8
#define GREEN_LED 7

// Define Buzzer pin
#define BUZZER 12

// Variables for distance measurement
long duration2, duration3;
int distance2, distance3;
int threshold = 9; // Threshold distance in cm
int minValidDistance = 2; // Minimum valid distance (cm) to filter out false readings
boolean specialMode = false; // For special patterns when multiple sensors are triggered

// Variables for buzzer control
unsigned long lastBuzzerTime = 0;
int currentTune = 0; // 0: no sound, 2: tune2, 3: tune3, 4: special tune

void setup() {
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
  
  // Initialize LED pins
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Initialize buzzer pin
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
  // Turn off all LEDs initially
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  
  // Test the buzzer
  playTone(100, 3);
  
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Two Ultrasonic Sensors Controlling LEDs with Buzzer Feedback");
  Serial.println("Place hand in front of any sensor within 10cm to trigger its LED and buzzer");
  Serial.println("Buzzer test complete");
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

void allLedsOff() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

// Function to play a single tone
void playTone(int duration, int repetitions) {
  for (int i = 0; i < repetitions; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(duration);
    digitalWrite(BUZZER, LOW);
    delay(duration);
  }
}

// Function for Sensor 2 tune (Yellow LED) - Double beep pattern
void playTune2() {
  unsigned long currentTime = millis();
  if (currentTime - lastBuzzerTime > 500) {
    lastBuzzerTime = currentTime;
    digitalWrite(BUZZER, HIGH);
    delay(80);
    digitalWrite(BUZZER, LOW);
    delay(80);
    digitalWrite(BUZZER, HIGH);
    delay(80);
    digitalWrite(BUZZER, LOW);
  }
}

// Function for Sensor 3 tune (Green LED) - Long single beep
void playTune3() {
  unsigned long currentTime = millis();
  if (currentTime - lastBuzzerTime > 800) {
    lastBuzzerTime = currentTime;
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
  }
}

// Function for special tune (All sensors) - Victory melody
void playSpecialTune() {
  unsigned long currentTime = millis();
  if (currentTime - lastBuzzerTime > 1000) {
    lastBuzzerTime = currentTime;
    // Play ascending notes
    for (int i = 50; i <= 150; i += 25) {
      digitalWrite(BUZZER, HIGH);
      delay(i);
      digitalWrite(BUZZER, LOW);
      delay(50);
    }
  }
}

void loop() {
  // Measure distance from each sensor
  distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);
  distance3 = measureDistance(TRIG_PIN3, ECHO_PIN3);
  
  // Print distances to serial monitor for debugging
  Serial.print("Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm | Distance 3: ");
  Serial.print(distance3);
  Serial.println(" cm");
  
  // Turn off all LEDs first
  allLedsOff();
  
  // Reset buzzer state
  currentTune = 0;
  
  // Check if both sensors are triggered simultaneously
  if ((distance2 < threshold && distance2 >= minValidDistance) && 
      (distance3 < threshold && distance3 >= minValidDistance)) {
    specialMode = true;
    // Special mode: all LEDs on steadily
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    // Play special tune
    currentTune = 4;
    Serial.println("SPECIAL MODE: Both sensors triggered!");
  }
  else {
    specialMode = false;
    
    // Check sensor 2 (Yellow LED)
    if (distance2 < threshold && distance2 >= minValidDistance) {
      digitalWrite(YELLOW_LED, HIGH);
      currentTune = 2; // Set tune for sensor 2
      Serial.println("YELLOW LED ON - Sensor 2 triggered");
    }
    // Check sensor 3 (Green LED)
    else if (distance3 < threshold && distance3 >= minValidDistance) {
      digitalWrite(GREEN_LED, HIGH);
      currentTune = 3; // Set tune for sensor 3
      Serial.println("GREEN LED ON - Sensor 3 triggered");
    }
  }
  // Play the selected tune
  switch(currentTune) {
    case 2:
      playTune2();
      break;
    case 3:
      playTune3();
      break;
    case 4:
      playSpecialTune();
      break;
    default:
      // No sound
      digitalWrite(BUZZER, LOW);
      break;
  }
  
  // Short delay before next reading
  delay(20); // Reduced delay for better tune responsiveness
}
