/*
 * TM1637 Four-Digit Display
 *
 * This TM1637 Four-Digit Display demonstrates how to control a TM1637 4-digit 7-segment LED display.
 * The example shows two display modes:
 * 1. Displaying characters (including the colon separator)
 * 2. Displaying a 4-digit number
 *
 * Hardware Connections:
 *   TM1637 Display | Aries Board
 *   --------------------------------
 *   VCC            | 3.3V
 *   GND            | GND
 *   CLK            | GPIO1
 *   DIO            | GPIO0
 *
 * Library: Grove 4-Digit Display (by Seeed Studio)
 */

#include <TM1637.h>  // Include the TM1637 display library

// Pin definitions
int CLK = 1;   // CLK of TM1637 connected to GPIO-1 pin of Aries Board
int DIO = 0;   // DIO of TM1637 connected to GPIO-0 pin of Aries Board

TM1637 tm(CLK, DIO);  // Initialize TM1637 display object with pin assignments

void setup() {
  tm.init();    // Initialize the display
  tm.set(2);    // Set display brightness (range: 0-7)
}

void loop() {
  // Example 1: Display special characters "12:ab"
  // Each position (0-3) can display a character (0-15 for digits and letters)
  tm.display(0, 10);    // Position 0 (leftmost): Display 'a'
  tm.display(1, 11);    // Position 1: Display 'b'
  tm.point(1);          // Enable the colon between positions 1 and 2
  tm.display(2, 12);    // Position 2: Display 'c'
  tm.display(3, 13);    // Position 3 (rightmost): Display 'd'

  delay(1000);          // Wait for 1 second before changing display

  // Example 2: Display a 4-digit number "1234"
  displayNumber(1234);  // Call function to display the number

  delay(1000);          // Wait for 1 second before repeating
}

/**
 * Function to display a 4-digit number on the TM1637 display
 * 
 * @param num - The number to display (0-9999)
 * 
 * The function separates individual digits and displays them
 * in the correct positions (right-aligned)
 */
void displayNumber(int num) {
  tm.display(3, num % 10);           // Units digit (rightmost position)
  tm.display(2, (num / 10) % 10);    // Tens digit
  tm.display(1, (num / 100) % 10);   // Hundreds digit
  tm.display(0, (num / 1000) % 10);  // Thousands digit (leftmost position)
}
