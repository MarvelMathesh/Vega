#include <TM1637.h>
int IRSensor = 4;
int CLK = 1;  // CLK of TM1637 is connected to GPIO-1 pin of Aries Board
int DIO = 0;  // DIO of TM1637 is connected to GPIO-0 pin of Aries Board

TM1637 tm(CLK, DIO);
int count = 0;
int lastState = HIGH;   // Assume no object detected at start
void setup() {
  pinMode(IRSensor, INPUT);
  tm.init();
  tm.set(2);
  updateDisplay(count);
}

void loop() {
  delay(100);
  int statusSensor = digitalRead(IRSensor);


  if (statusSensor == LOW && lastState == HIGH) {
    count++;
    updateDisplay(count);
  }
  lastState = statusSensor;
}
// Function to update 4-digit 7-segment display
void updateDisplay(int num) {
  tm.display(3, num % 10);
  tm.display(2, (num / 10) % 10);
  tm.display(1, (num / 100) % 10);
  tm.display(0, (num / 1000) % 10);
}
