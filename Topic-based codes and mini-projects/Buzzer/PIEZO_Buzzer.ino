/*
 * Piezo Buzzer
 *
 * This Piezo Buzzer generates a simple beeping sound pattern using a piezoelectric buzzer.
 * The buzzer is toggled between on and off states at regular intervals.
 *
 * Hardware Connections:
 *   Piezo Buzzer   | Aries Board
 *   --------------------------------
 *   VCC            | 3.3V
 *   GND            | GND
 *   IN (Signal)    | GPIO0
 *
 * Working Principle:
 * When the digital pin is set HIGH, the buzzer receives voltage and produces sound.
 * When the digital pin is set LOW, the buzzer stops producing sound.
 * Rapidly alternating between these states creates a beeping pattern.
 */

#define BUZZER 0   // Connect INPUT pin of buzzer to GPIO-0

// The setup function runs once when the board powers on or is reset
void setup() {
  pinMode(BUZZER, OUTPUT);   // Configure buzzer pin as an output
}

// The loop function runs continuously after setup completes
void loop() {
  digitalWrite(BUZZER, HIGH);   // Turn ON the buzzer (sound starts)
  delay(100);                   // Keep buzzer ON for 100 milliseconds
  
  digitalWrite(BUZZER, LOW);    // Turn OFF the buzzer (sound stops)
  delay(100);                   // Keep buzzer OFF for 100 milliseconds
                                // This creates a 5Hz beeping pattern (on for 0.1s, off for 0.1s)
}
