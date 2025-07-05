#include <TM1637.h>

int IRSensor = 4;
int LED = 23; 
int CLK = 1;        // TM1637 Clock Pin
int DIO = 0;        // TM1637 Data Pin

TM1637 tm(CLK, DIO);

int lastState = HIGH;

void setup() {
  pinMode(IRSensor, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  tm.init();
  tm.set(5);
  updateDisplay(0000);

}

void loop() {
  int statusSensor = digitalRead(IRSensor);

  if (statusSensor == LOW && lastState == HIGH) { 
    int slotNumber = random(0, 11);
    updateDisplay(slotNumber);

    if (slotNumber == 10) {
      flashWinLED();
    }
  }

  lastState = statusSensor;
}

void updateDisplay(int num) {
  tm.display(3, num % 10);          // Ones place
  tm.display(2, (num / 10) % 10);   // Tens place
  tm.display(1, (num / 100) % 10);  // Hundreds place
  tm.display(0, (num / 1000) % 10); // Thousands place
}

void flashWinLED() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    delay(300);
  }
}
