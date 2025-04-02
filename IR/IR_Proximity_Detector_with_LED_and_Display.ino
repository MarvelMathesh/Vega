/*
 * IR Proximity Detector with LED and Display
 * 
 * This IR Proximity Detector uses an IR sensor to detect objects and displays proximity
 * status on a TM1637 4-digit display. An LED also indicates detection state.
 * 
 * Hardware connections:
 * - IR Sensor: Connected to digital pin 4
 * - Onboard LED: Pin 22 (active LOW)
 * - TM1637 Display: CLK to pin 1, DIO to pin 0
 * 
 * When an object is detected close to the IR sensor, the display shows "CLSE"
 * When no object is detected, the display shows "FAR "
 */

#include <TM1637.h> // Library for 4-digit 7-segment display

// Pin definitions
int IRSensor = 4;   // IR proximity sensor connected to pin 4
int LED = 22;       // Onboard LED on pin 22 (active LOW)
int CLK = 1;        // CLK of TM1637 display connected to GPIO-1
int DIO = 0;        // DIO of TM1637 display connected to GPIO-0

TM1637 tm(CLK,DIO); // Initialize TM1637 display object

void setup() {
  pinMode(IRSensor, INPUT); // Configure IR sensor pin as input
  
  tm.init();        // Initialize the display
  tm.set(2);        // Set display brightness (range: 0-7)
}

void loop() {
  // Read the current state of the IR sensor
  int statusSensor = digitalRead(IRSensor); 
  
  if (statusSensor == 1) {
    // When IR sensor reads HIGH (no object detected/far away)
    digitalWrite(LED, HIGH); // Turn LED OFF (active LOW)
    tm.display(0,'F');       // Display "FAR " on the 4-digit display
    tm.display(1,'A');
    tm.display(2,'R');
    tm.display(3,' ');
  }
  else {
    // When IR sensor reads LOW (object detected/close)
    digitalWrite(LED, LOW);  // Turn LED ON (active LOW)
    tm.display(0,'C');       // Display "CLSE" on the 4-digit display
    tm.display(1,'L');
    tm.display(2,'S');
    tm.display(3,'E');
  }
}
