/*
 * Five-LED Sequential Blink Pattern
 * 
 * This Five-LED Sequential Blink sequentially blinks five LEDs one after another.
 * Each LED turns on for 100ms, turns off, then the next LED in sequence activates.
 * 
 * Hardware notes:
 * - LEDs on pins 4 and 5 are external LEDs
 * - LEDs on pins 22, 23, and 24 are onboard LEDs on the Aries board
 *   (GREEN=22, BLUE=23, RED=24)
 */

// LED pin definitions
int ledPin1 = 4;    // External LED 1 connected to digital pin 4
int ledPin2 = 5;    // External LED 2 connected to digital pin 5
int ledPin3 = 22;   // Onboard GREEN LED on pin 22
int ledPin4 = 23;   // Onboard BLUE LED on pin 23
int ledPin5 = 24;   // Onboard RED LED on pin 24

// The setup function runs once when the board powers on or is reset
void setup() {
    // Initialize all LED pins as outputs
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT); 
    pinMode(ledPin5, OUTPUT); 
}

// The loop function runs continuously after setup completes
void loop() {  
    digitalWrite(ledPin1, HIGH); // Turn ON external LED 1
    delay(100);                  // Wait for 100 milliseconds
    digitalWrite(ledPin1, LOW);  // Turn OFF external LED 1
    delay(100);                  // Wait for 100 milliseconds
    
    digitalWrite(ledPin2, HIGH); // Turn ON external LED 2
    delay(100);                  // Wait for 100 milliseconds
    digitalWrite(ledPin2, LOW);  // Turn OFF external LED 2
    delay(100);                  // Wait for 100 milliseconds
    
    digitalWrite(ledPin3, HIGH); // Turn ON onboard GREEN LED 
    delay(100);                  // Wait for 100 milliseconds
    digitalWrite(ledPin3, LOW);  // Turn OFF onboard GREEN LED 
    delay(100);                  // Wait for 100 milliseconds
    
    digitalWrite(ledPin4, HIGH); // Turn ON onboard BLUE LED 
    delay(100);                  // Wait for 100 milliseconds
    digitalWrite(ledPin4, LOW);  // Turn OFF onboard BLUE LED 
    delay(100);                  // Wait for 100 milliseconds
    
    digitalWrite(ledPin5, HIGH); // Turn ON onboard RED LED 
    delay(100);                  // Wait for 100 milliseconds
    digitalWrite(ledPin5, LOW);  // Turn OFF onboard RED LED 
    delay(100);                  // Wait for 100 milliseconds
}
