#include <TM1637.h>

int IRSensor = 4;
int LED = 22;
int CLK = 1; //CLK of TM1637 is connected to GPIO-1 pin of Aries Board
int DIO = 0;  //DI0 of TM1637 is connected to GPIO-0 pin of Aries Board

TM1637 tm(CLK,DIO);

void setup() {
  pinMode (IRSensor, INPUT);

  tm.init();
  tm.set(2); //set brightness; 0-7
}

void loop() {
  int statusSensor = digitalRead (IRSensor); 
  
  if (statusSensor == 1){
    digitalWrite(LED, HIGH); // LED OFF 
    tm.display(0,5);
    tm.display(1,10);
    tm.display(2,15);
    tm.display(3,14);
  }
  else {
    digitalWrite(LED, LOW); // LED ON
    tm.display(0,13);
    tm.display(1,14);
    tm.display(2,10);
    tm.display(3,13);
  }
}
