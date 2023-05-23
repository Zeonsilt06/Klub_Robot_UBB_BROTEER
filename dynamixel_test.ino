#include <DynamixelSerial.h>

int MOVE_SPEED = 200;
int POSITION = 520;
int POSITION2 = 220;
int Temperature,Voltage,Position; 

void setup(){
  Dynamixel.setSerial(&Serial1); // &Serial - Arduino UNO/NANO/MICRO, &Serial1, &Serial2, &Serial3 - Arduino Mega
  Dynamixel.begin(1000000,5);  // Inicialize the servo at 1 Mbps and Pin Control 2
  Serial.begin(115200);              // Begin Serial Comunication
}

void loop(){
  /*
  POSITION = analogRead(A0);

  Dynamixel.moveSpeed(18,POSITION,MOVE_SPEED);
  Dynamixel.moveSpeed(12,POSITION,MOVE_SPEED);
  Dynamixel.moveSpeed(11,POSITION,MOVE_SPEED);
  Dynamixel.moveSpeed(14,POSITION,MOVE_SPEED);
  */
  Temperature = Dynamixel.readTemperature(1); // Request and Print the Temperature
  Voltage = Dynamixel.readVoltage(1);         // Request and Print the Voltage
  Position = Dynamixel.readPosition(1);       // Request and Print the Position 
  

  Dynamixel.moveSpeed(5,random(520,700),MOVE_SPEED);
  Dynamixel.moveSpeed(3,random(520,700),MOVE_SPEED);
  Dynamixel.moveSpeed(1,random(512,700),MOVE_SPEED);
  
//lengan kiri
  Dynamixel.moveSpeed(2,random(200,512),MOVE_SPEED);
  Dynamixel.moveSpeed(4,random(220,512),MOVE_SPEED);
  Dynamixel.moveSpeed(6,random(220,512),MOVE_SPEED);
  
  
  Serial.print(" *** Temperature: ");   // Print the variables in the Serial Monitor
  Serial.print(Temperature);
  Serial.print(" Celcius  Voltage: ");
  Serial.print(Voltage);
  Serial.print("  Volts   Position: ");
  Serial.print(Position);
  Serial.println(" of 1023 resolution");  
  delay(1000); 
}
