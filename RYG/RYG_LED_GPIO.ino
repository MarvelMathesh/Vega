/*
 * RYG LED Traffic Light Pattern
 *
 * This RYG LED Traffic Light Pattern controls a Red-Yellow-Green LED strip in a sequential pattern
 * similar to traffic signals. LEDs turn on and off in series with delays between
 * each state change.
 *
 * Hardware Connections:
 *   RYG LED Strip  | Aries Board
 *   --------------------------------
 *   GND            | GND
 *   R (Red)        | GPIO0
 *   Y (Yellow)     | GPIO1
 *   G (Green)      | GPIO2
 *
 * Pattern Sequence:
 * 1. Red LED turns ON
 * 2. Yellow LED turns ON (Red+Yellow both ON)
 * 3. Green LED turns ON (All LEDs ON)
 * 4. Green LED turns OFF (Red+Yellow remain ON)
 * 5. Yellow LED turns OFF (Red remains ON)
 * 6. Red LED turns OFF (All LEDs OFF)
 * 7. Repeat sequence
 */

#define RED_LED     0     // Connect Red LED pin to GPIO-0
#define YELLOW_LED  1     // Connect Yellow LED pin to GPIO-1
#define GREEN_LED   2     // Connect Green LED pin to GPIO-2

// The setup function runs once when you press reset or power the board
void setup() {
  // Initialize digital pins as outputs for controlling LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

// The loop function runs continuously after setup completes
void loop() {
  // Turn RYG LEDs on and then off in series with 1 second delay between each action
  digitalWrite(RED_LED, HIGH);      // Turn Red LED ON
  delay(1000);                      // Wait for 1 second
  
  digitalWrite(YELLOW_LED, HIGH);   // Turn Yellow LED ON (Red+Yellow ON)
  delay(1000);                      // Wait for 1 second
  
  digitalWrite(GREEN_LED, HIGH);    // Turn Green LED ON (All LEDs ON)
  delay(1000);                      // Wait for 1 second
  
  digitalWrite(GREEN_LED, LOW);     // Turn Green LED OFF (Red+Yellow ON)
  delay(1000);                      // Wait for 1 second
  
  digitalWrite(YELLOW_LED, LOW);    // Turn Yellow LED OFF (Red ON)
  delay(1000);                      // Wait for 1 second
  
  digitalWrite(RED_LED, LOW);       // Turn Red LED OFF (All LEDs OFF)
  delay(1000);                      // Wait for 1 second before repeating
}
