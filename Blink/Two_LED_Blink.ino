/*
 * Two-LED Blink Pattern
 * 
 * This Two-LED Blink alternates two LEDs in a sequential pattern.
 * Each LED turns on and off with a brief delay between states.
 */

int ledPin1 = 4;  // First LED connected to GPIO pin 4
int ledPin2 = 5;  // Second LED connected to GPIP pin 5

// The setup function runs once when the board powers on or is reset
void setup() {
    pinMode(ledPin1, OUTPUT);  // Configure pin 4 as an output for LED1
    pinMode(ledPin2, OUTPUT);  // Configure pin 5 as an output for LED2
}

// The loop function runs continuously after setup completes
void loop() {  
    digitalWrite(ledPin1, HIGH);  // Turn on first LED
    delay(100);                   // Wait for 100 milliseconds
    digitalWrite(ledPin1, LOW);   // Turn off first LED
    delay(100);                   // Wait for 100 milliseconds
    digitalWrite(ledPin2, HIGH);  // Turn on second LED
    delay(100);                   // Wait for 100 milliseconds
    digitalWrite(ledPin2, LOW);   // Turn off second LED
    delay(100);                   // Wait for 100 milliseconds
}
