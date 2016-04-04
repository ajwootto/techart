#define E1 10  // Enable Pin for motor 1
#define E2 11  // Enable Pin for motor 2
 
#define I1 8  // Control pin 1 for motor 1
#define I2 9  // Control pin 2 for motor 1
#define I3 12  // Control pin 1 for motor 2
#define I4 13  // Control pin 2 for motor 2
int motorSpeed = 0; 
String currentCity=""; 
String inBuffer="";
String motorString="";
void setup() {
 
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
 
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);
    Serial.begin(9600);
     
}
void loop() {

  while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inBuffer += recieved; 

       // Serial.print(recieved); 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            if (inBuffer.substring(0,1) == "C") {
              currentCity = inBuffer.substring(2); 
            }

            if (inBuffer.substring(0,1) == "V"){
              motorString=inBuffer.substring(2);
              motorSpeed = motorString.toInt();
            }
            Serial.print(currentCity);
            Serial.print(motorString); 
            Serial.print(motorSpeed);
            Serial.println(""); 
            inBuffer = ""; // Clear recieved buffer
        }
    
  delay(1);        // delay in between reads for stability
}
    analogWrite(E2, motorSpeed); // Run in half speed
    //analogWrite(E1, motor2); // Run in full speed
 
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);  
}
