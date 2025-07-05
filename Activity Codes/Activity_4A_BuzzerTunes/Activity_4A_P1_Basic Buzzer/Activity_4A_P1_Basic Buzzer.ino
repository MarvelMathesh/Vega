#define BUZZER 1 // connect INPUT pin of buzzer to GPIO-0

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 0 as an output.
  pinMode(BUZZER, OUTPUT); 
}

// the loop function runs over and over again forever
void loop() {
  // switching buzzer on and off rapidly
  digitalWrite(BUZZER, HIGH); //turn on the buzzer
  delay(250);
  digitalWrite(BUZZER, LOW);  //turn off the buzzer
  delay(750);
  digitalWrite(BUZZER, HIGH); //turn on the buzzer
  delay(125);
  digitalWrite(BUZZER, LOW);  //turn off the buzzer
  delay(125);
  digitalWrite(BUZZER, HIGH); //turn on the buzzer
  delay(125);
  digitalWrite(BUZZER, LOW);  //turn off the buzzer
  delay(125);
}
