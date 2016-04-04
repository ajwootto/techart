#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#define enablePin 10  // Enable Pin for motor 1

#define airPin 9  // Control pin 1 for motor 1
#define windPin 10  // Control pin 2 for motor 1
#define rainPin 11  // Control pin 1 for motor 2

#define LIGHT1 3
#define LIGHT2 5
#define LIGHT3 6

double elementPercent = 0; 
String currentCity = ""; 
String inBuffer = "";
String motorString="";

//Denotes element assigned to this Arduino, determing how it behaves
String element="";

void setup() {

    lcd.begin(16, 2);

    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
 
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);

    pinMode(LIGHT1, OUTPUT);
    pinMode(LIGHT2, OUTPUT);
    pinMode(LIGHT3, OUTPUT);

    Serial.begin(9600);
     
}
void loop() {

  while (Serial.available() > 0) {
    char recieved = Serial.read();
    inBuffer += recieved; 
        
    // Process message when new line character is recieved
    if (recieved == '\n') {
      processCommand(inBuffer);
      inBuffer = "";
    }
    switch(element) {
      case("wind"):
        performWind();
        break;
      case("air"):
        performAir();
        break;
      case("rain"):
        performRain();
        break;
      case("temp"):
        performTemp();
        break;
    }
    delay(1);        // delay in between reads for stability
  }
  
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, HIGH);
  digitalWrite(I4, LOW);  
}


void processCommand(String cmd) {
   if (cmd.substring(0,1) == "C") {
      currentCity = inBuffer.substring(2); 
    }

    if (cmd.substring(0,1) == "V"){
      motorString=cmd.substring(2);
      motorSpeed = motorString.toInt();
    }
}

void performTemp() {
  int tempDelay = 500;
  int eachDelay = tempDelay/4*elementPercentage
  int allDelay = tempDelay*elementPercentage
  digitalWrite(LIGHT1, HIGH);
  digitalWrite(LIGHT2, LOW);
  digitalWrite(LIGHT3, LOW);

  delay(eachDelay);
  
  digitalWrite(LIGHT1, LOW);
  digitalWrite(LIGHT2, HIGH);
  digitalWrite(LIGHT3, LOW);

  delay(eachDelay);

  digitalWrite(LIGHT1, LOW);
  digitalWrite(LIGHT2, LOW);
  digitalWrite(LIGHT3, HIGH);

  delay(eachDelay);
  
  digitalWrite(LIGHT1, LOW);
  digitalWrite(LIGHT2, LOW);
  digitalWrite(LIGHT3, LOW);
  delay(allDelay);
}

void performWind() {
  analogWrite(E2, elementPercent*255); // Run in half speed
}

void performAir() {
  analogWrite(
}

void performRain() {
  
}


