int base_PWM = 150;
int maxSpeed = 75;
int minSpeed = -75;

float KP = 10.0;
float KI = 0.0;
float KD = 0.3;
float PID;
float setpoint = 20; //robot mengikuti dinding dengan jarak 20 cm dari dinding kiri
float error;
float lastError = 0;
float erroecho= 0;
float errorSum = 0;
float error_threshold = 50; //batas toleransi error untuk integral
long waktuLampau_PID = 0; 
long interval_PID = 35;

float jarak1, jarak2 ;
#define trigger1 10
#define echo1 11
#define trigger2 8
#define echo2 9

int motor_pin[6] = {2,3,4,5,6,7};
long waktuLampau_motor = 0;
long interval_motor = 40;

long waktuSekarang; 
long waktuLampau_sensor = 0;
long interval_sensor = 30; 
 
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

void roda_kanan(int speed){
  if(speed < 0){
    digitalWrite(motor_pin[1], HIGH);
    digitalWrite(motor_pin[2], LOW);
  }
  else if(speed > 0){
    digitalWrite(motor_pin[1], LOW);
    digitalWrite(motor_pin[2], HIGH);
  }
  else{
    digitalWrite(motor_pin[1], LOW);
    digitalWrite(motor_pin[2], LOW);
  } 
  analogWrite(motor_pin[0], abs(speed));
}

void roda_kiri(int speed){
  if(speed < 0){
    digitalWrite(motor_pin[3], HIGH);
    digitalWrite(motor_pin[4], LOW);
  }
  else if(speed > 0){
    digitalWrite(motor_pin[3], LOW);
    digitalWrite(motor_pin[4], HIGH);
  }
  else{
    digitalWrite(motor_pin[3], LOW);
    digitalWrite(motor_pin[4], LOW);
  }
  analogWrite(motor_pin[5], abs(speed));
}

void hitungPID(){
  float input = bacajarak(trigger2, echo2);
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
  Serial.print(speedR);
  Serial.print(" ");
  Serial.print(speedL);
}
void setup() {
  for (int i =0; i <= 5; i++){
  pinMode(motor_pin[i], OUTPUT);
  }
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger2, OUTPUT);
  Serial.begin(9600);
}
void loop() {
waktuSekarang = millis();
  if(waktuSekarang - waktuLampau_sensor >= interval_sensor){
    jarak1 = bacajarak(trigger1, echo1);
    jarak2 = bacajarak(trigger2, echo2);
    waktuLampau_sensor = waktuSekarang;
  }
  if(waktuSekarang - waktuLampau_PID >= interval_PID){
    hitungPID();
    waktuLampau_PID = waktuSekarang;
  }
  if(waktuSekarang - waktuLampau_motor >= interval_motor){
    if(jarak1 >= 30){
      ikutiDinding();
    }
    else if (jarak1 <30 ){
      roda_kanan(-255);
      roda_kiri(255);
    }
    waktuLampau_motor=waktuSekarang;
  }
    Serial.print(jarak1);
    Serial.print(" cm | ");
    Serial.println(jarak2);
}
