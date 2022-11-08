#include <Arduino.h>

int base_PWM = 150;
int maxSpeed = 255;
int minSpeed = -255;

//parameter untuk PID
float KP = 10.0;
float KI = 0.0;
float KD = 0.0;
float PID;
float setpoint = 20; //robot mengikuti dinding dengan jarak 20 cm dari dinding kiri
float error;
float lastError = 0;
float errorSum = 0;
float error_threshold = 50; //batas toleransi error untuk integral
long waktuLampau_PID = 0;
long interval_PID = 35;

//parameter sensor ultrasonik
float jarak1, jarak2, jarak3;
#define trigger1 8
#define echo1 9
#define trigger2 10
#define echo2 11
#define trigger3 12
#define echo3 13

//pin motor
int motor_pin[6] = {52,50,48,46,44,42};
long waktuLampau_motor = 0;
long interval_motor = 40;

long waktuSekarang; 
long waktuLampau_sensor = 0;
long interval_sensor = 30; 


//fungsi untuk membaca jarak dari ultrasonik 
float bacajarak (int trigPin, int echoPin){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin,HIGH);
    float distance = (duration*0.034/2);
    return distance;
}

//fungsi untuk menjalankan roda kanan
void roda_kanan(int speed){
  if(speed > 0){
    digitalWrite(motor_pin[1], HIGH);
    digitalWrite(motor_pin[2], LOW);
  }
  else if(speed < 0){
    digitalWrite(motor_pin[1], LOW);
    digitalWrite(motor_pin[2], HIGH);
  }
  else{
    digitalWrite(motor_pin[1], LOW);
    digitalWrite(motor_pin[2], LOW);
  }
  analogWrite(motor_pin[0], abs(speed));
}

//fungsi untuk menjalankan roda kiri
void roda_kiri(int speed){
  if(speed > 0){
    digitalWrite(motor_pin[3], HIGH);
    digitalWrite(motor_pin[4], LOW);
  }
  else if(speed < 0){
    digitalWrite(motor_pin[3], LOW);
    digitalWrite(motor_pin[4], HIGH);
  }
  else{
    digitalWrite(motor_pin[3], LOW);
    digitalWrite(motor_pin[4], LOW);
  }
  analogWrite(motor_pin[5], abs(speed));
}

//fungsi untuk komputasi nilai PID
void hitungPID(){
  float input = bacajarak(trigger1, echo1);
  error = setpoint - input;

  if (abs(error) > error_threshold){
    errorSum += error;
  }
  else {
    errorSum = 0;
  }

  float d_error = error - lastError;
  lastError = error;
  PID = KP*error + KI*errorSum + KD*d_error;

  if(PID > maxSpeed){
    PID = maxSpeed;
  }

  if (PID < minSpeed){
    PID = minSpeed;
  }
}

void ikutiDinding() {
  int speedL = base_PWM+PID;
  int speedR = base_PWM-PID;
  roda_kanan(speedR);
  roda_kiri(speedL);
}

//setup
void setup() {
  for (int i =0; i <= 5; i++){
    pinMode(motor_pin[i], OUTPUT);
  }
  pinMode(echo2, INPUT);
  pinMode(trigger2, OUTPUT);
  Serial.begin(9600);
}

//loop utama
void loop() {
//loop untuk menghitung jarak masukan sensor
waktuSekarang = millis();
  if(waktuSekarang - waktuLampau_sensor >= interval_sensor){
    jarak1 = bacajarak(trigger1, echo1);
    jarak2 = bacajarak(trigger2, echo2);
    jarak3 = bacajarak(trigger3, echo3);
    waktuLampau_sensor = waktuSekarang;
  }
  //loop untuk menghitung PID
  if(waktuSekarang - waktuLampau_PID >= interval_PID){
    hitungPID();
    waktuLampau_PID = waktuSekarang;
  }

  //loop untuk kerja motor
  if(waktuSekarang - waktuLampau_motor >= interval_motor){
    if(jarak2 >= 20){
      ikutiDinding();
    }
    else if(jarak2 < 20 && jarak1 < 20){
      roda_kanan(minSpeed);
      roda_kiri(maxSpeed);
    }
  }
}

