/*
 * IR Object Counter with TM1637 Display
 *
 * This sketch counts objects passing by an IR sensor (HW-201) and 
 * displays the total count on a 4-digit 7-segment TM1637 display.
 * 
 * Hardware Connections:
 *   IR Sensor (HW-201) | Aries Board
 *   --------------------------------
 *   VCC                | 3.3V
 *   GND                | GND
 *   OUT                | GPIO4
 *
 *   TM1637 Display     | Aries Board
 *   --------------------------------
 *   CLK                | GPIO1
 *   DIO                | GPIO0
 *   VCC                | 5V
 *   GND                | GND
 */

#include <TM1637.h>  // Library for TM1637 4-digit 7-segment display

// Pin definitions
int IRSensor = 4;    // IR sensor output connected to GPIO-4
int CLK = 1;         // CLK of TM1637 display connected to GPIO-1
int DIO = 0;         // DIO of TM1637 display connected to GPIO-0

TM1637 tm(CLK, DIO); // Initialize TM1637 display object

int count = 0;       // Counter for objects detected
int lastState = HIGH; // Track previous IR sensor state (HIGH = no object detected)

void setup() {
  pinMode(IRSensor, INPUT);  // Configure IR sensor pin as input

  tm.init();                 // Initialize the display
  tm.set(2);                 // Set display brightness (range: 0-7)
  updateDisplay(count);      // Show initial count (zero) on display
}

void loop() {
  delay(100);                // Small delay to avoid multiple counts from same object

  // Read current state of IR sensor
  // LOW = object detected, HIGH = no object detected
  int statusSensor = digitalRead(IRSensor);

  // Detect falling edge (transition from no object to object detected)
  if (statusSensor == LOW && lastState == HIGH) {
    count++;                 // Increment object counter
    updateDisplay(count);    // Update display with new count
  }
  lastState = statusSensor;  // Save current state for next comparison
}

/**
 * Function to update the 4-digit 7-segment display
 * Splits a number into individual digits and displays each digit
 * in the appropriate position (right-aligned)
 * 
 * @param num - The number to display (0-9999)
 */
void updateDisplay(int num) {
  tm.display(3, num % 10);          // Units digit (rightmost)
  tm.display(2, (num / 10) % 10);   // Tens digit
  tm.display(1, (num / 100) % 10);  // Hundreds digit
  tm.display(0, (num / 1000) % 10); // Thousands digit (leftmost)
}
