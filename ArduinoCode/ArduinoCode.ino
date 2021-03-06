#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#define enablePin1 12  // Enable Pin for motor 1
#define enablePin2 13

#define airPin 9  // Control pin 1 for motor 1
#define windPin 10  // Control pin 2 for motor 1
#define rainPin 11  // Control pin 1 for motor 2

#define LIGHT1 3
#define LIGHT2 5
#define LIGHT3 6

double elementPercent = 0; 
int elementVal = 0;

String currentCity = ""; 
String inBuffer = "";
String motorString="";

//Denotes element assigned to this Arduino, determing how it behaves
String element="rain";

int delayCount = 0;

void setup() {

    lcd.begin(16, 2);
    lcd.setRGB(0, 100, 0);
    lcd.write("Initializing");
    pinMode(airPin, OUTPUT);
    pinMode(windPin, OUTPUT);
    pinMode(rainPin, OUTPUT);
    pinMode(enablePin1, OUTPUT);
    pinMode(enablePin2, OUTPUT);
 
    pinMode(LIGHT1, OUTPUT);
    pinMode(LIGHT2, OUTPUT);
    pinMode(LIGHT3, OUTPUT);

    Serial.begin(9600);
     
}

void loop() {
  digitalWrite(enablePin1, HIGH);
  digitalWrite(enablePin2, LOW);

  while (Serial.available() > 0) {
    char recieved = Serial.read();
    inBuffer += recieved; 
        
    // Process message when new line character is recieved
    if (recieved == '\n') {
      processCommand(inBuffer);
      inBuffer = "";
    }
    delay(1);        // delay in between reads for stability
  }
   if (element == "wind") {
    performWind();
    } else if (element == "aqi") {
    performAir();
  } else if (element == "rain") {
    performRain();
  } else if (element == "temp") {
    performTemp();
  }
  //Serial.println(element);
  //testMotors();
}

void testMotors(){
  analogWrite(rainPin, 200);
  analogWrite(windPin, 100);
  analogWrite(airPin, 200);
}

void processCommand(String cmd) {
  Serial.println(cmd);

   if (cmd.substring(0,1) == "C") {
      currentCity = inBuffer.substring(2); 
      displayCity();
    }

    if (cmd.substring(0,1) == "P"){
      elementPercent = cmd.substring(2).toInt() / 100.0;
    }

    if (cmd.substring(0,1) == "V") {
      elementVal = cmd.substring(2).toInt();
      displayWeather();
    }

    if (cmd.substring(0,1) == "E") {
       element = cmd.substring(2);
       element.trim();
    }
}

void performTemp() {
  delayCount++;
  int tempDelay = 20000;
  if (elementPercent == 1) {
    elementPercent == 0.99;
  }
  int eachDelay = round(tempDelay/4*(1-elementPercent));
  int allDelay = round(tempDelay*(1-elementPercent));

  if (delayCount < eachDelay) {
    digitalWrite(LIGHT1, HIGH);
    digitalWrite(LIGHT2, LOW);
    digitalWrite(LIGHT3, LOW);
  } else if (delayCount < eachDelay * 2) {
    digitalWrite(LIGHT1, LOW);
    digitalWrite(LIGHT2, HIGH);
    digitalWrite(LIGHT3, LOW);
  } else if (delayCount < eachDelay * 3) {
    digitalWrite(LIGHT1, LOW);
    digitalWrite(LIGHT2, LOW);
    digitalWrite(LIGHT3, HIGH);
  } else if (delayCount < eachDelay * 4) {
    digitalWrite(LIGHT1, LOW);
    digitalWrite(LIGHT2, LOW);
    digitalWrite(LIGHT3, LOW);
  } else if (delayCount < eachDelay*4 + allDelay) {
    delayCount = 0;
  }
}



void displayCity() {
  lcd.clear();
  lcd.setRGB(0, 100, 0);
  int len = currentCity.length();
  int startLoc = floor(8 - len / 2);
  char city[len];
  currentCity.toCharArray(city, len);
  lcd.setCursor(startLoc, 0);
  lcd.write(city);  
}

void displayWeather() {
  lcd.setCursor(0, 1);
  lcd.write("            ");
  lcd.setCursor(0, 1);
  char value[10];
  String valString = String(elementVal);
  valString.toCharArray(value, 10);
  int startLoc = floor(8 - valString.length() / 2);
  if (element.equals("wind")) {
      startLoc -= 2;
      lcd.setCursor(startLoc, 1);
      lcd.write(value);
      lcd.write(" km/h");
  } else if (element.equals("aqi")) {
      startLoc -= 2;
      lcd.setCursor(startLoc, 1);
      lcd.write(value);
      lcd.write(" ppm");
  } else if (element.equals("rain")) {
      startLoc -= 1;
      lcd.setCursor(startLoc, 1);
      lcd.write(value);
      lcd.write(" mm");
  } else if (element.equals("temp")) {
      lcd.setCursor(startLoc, 1);
      lcd.write(value);
      lcd.write(223);
  } else {
    lcd.write("No Element");
  }
}

void performWind() {
  analogWrite(windPin, elementPercent*175 + 80); // Run in half speed
}

void performAir() {
  analogWrite(airPin, elementPercent*185 + 70);
}

void performRain() {
  analogWrite(rainPin, elementPercent*205 + 50);
}


