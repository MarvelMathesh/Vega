/*
  Blink for Aries Board
  
  This Blink demonstrates the basic functionality of blinking an onboard LED.
  
  Connections:
    No external connections required. Connect Aries board via USB and upload the program.
  
  LED Pin Mapping for Aries Board:
    Pin 22 - Green LED (Used in this example)
    Pin 23 - Blue LED
    Pin 24 - Red LED
  
  LED Behavior Notes:
    BLUE, RED, GREEN LEDs: ON when pin is LOW, OFF when pin is HIGH (active LOW)
*/

int ledPin=22; // Green LED on Aries board (active LOW)

// The setup function runs once when the board powers on or is reset
void setup() {
    // Configure digital pin as output to control the LED
    pinMode(ledPin,OUTPUT);
}

// The loop function runs continuously after setup completes
void loop() {  
    // LED blink sequence with 1-second intervals
    digitalWrite(ledPin,HIGH); // Turn LED OFF
    delay(1000);               // Pause execution for 1000ms (1 second)
    digitalWrite(ledPin,LOW);  // Turn LED ON
    delay(1000);               // Pause execution for 1000ms (1 second)
}
