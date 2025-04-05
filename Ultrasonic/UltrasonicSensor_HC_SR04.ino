/*
  @file UltrasonicSensor_HC-SR04.ino
  @brief HC-SR04 Ultrasonic Sensor with the ARIES v3 Board
  @detail Measures the distance of a target object and displays it on serial monitor
  
  The HC-SR04 works by sending an ultrasonic pulse and measuring the time
  it takes for the pulse to bounce back after hitting an object.
  
  Connections:
  HC-SR04    Aries Board
  VCC      -   5V
  GND      -   GND
  Echo     -   GPIO1
  Trig     -   GPIO0
  
  Working Principle:
  1. Send a 10μs pulse on the Trigger pin
  2. Sensor emits 8 cycle ultrasonic burst at 40kHz
  3. Listen for echo pulse on Echo pin
  4. Calculate distance using time-of-flight principle
     (Speed of sound in air ~343m/s at room temperature)
*/

#define echoPin 1  // Attach pin GPIO-1 on Aries to pin Echo of HC-SR04
#define trigPin 0  // Attach pin GPIO-0 on Aries to pin Trig of HC-SR04

// Variables to store sensor measurements
long duration;     // Duration of sound wave travel (in microseconds)
int distance;      // Calculated distance (in centimeters)

void setup() {
  pinMode(trigPin, OUTPUT);     // Configure trigger pin as OUTPUT to send pulses
  pinMode(echoPin, INPUT);      // Configure echo pin as INPUT to receive return signal
  
  Serial.begin(115200);         // Initialize serial communication at 115200 baud rate
  Serial.println("Ultrasonic Sensor HC-SR04 Test");  // Print header information
  Serial.println("with Aries V2.0");
}

void loop() {
  // Clear the trigger pin to ensure a clean pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Generate the ultrasonic pulse (10μs HIGH pulse)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure how long the echo pin stays HIGH (round-trip time of ultrasonic pulse)
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance using the formula: distance = (time × speed of sound) / 2
  // Speed of sound is ~343m/s = 0.0343cm/μs, divide by 2 for round trip
  distance = duration * 0.034 / 2;
  
  // Output the measured distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Brief delay before next measurement
  delay(100);  // Refresh rate of approximately 10 readings per second
}
