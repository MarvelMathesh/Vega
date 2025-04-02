/*
 * IR Proximity Sensor (HW-201)
 *
 * This IR Proximity Sensor demonstrates basic IR proximity detection using the HW-201 sensor.
 * When an object is detected near the sensor, the onboard green LED illuminates.
 * 
 * Hardware Connections:
 *   IR Sensor    |   Aries Board
 *   ---------------------
 *   VCC          |   3.3V
 *   GND          |   GND
 *   OUT          |   GPIO4
 *
 * Note: The onboard green LED on the Aries board is active LOW (ON when pin is LOW)
 */

int IRSensor = 4;  // IR sensor output connected to GPIO-4
int LED = 22;      // Onboard green LED connected to GPIO-22 (active LOW)

// The setup function runs once when the board powers on or is reset
void setup() {
  pinMode(IRSensor, INPUT);   // Configure IR sensor pin as input
  pinMode(LED, OUTPUT);       // Configure LED pin as output
}

// The loop function runs continuously after setup completes
void loop() {
  // Read the current state of the IR sensor
  // HIGH (1) = No object detected/far away
  // LOW (0) = Object detected/close
  int statusSensor = digitalRead(IRSensor);   
  
  if (statusSensor == 1) {
    digitalWrite(LED, HIGH);  // Turn LED OFF (active LOW)
  }
  else {
    digitalWrite(LED, LOW);   // Turn LED ON (active LOW)
  }
}
