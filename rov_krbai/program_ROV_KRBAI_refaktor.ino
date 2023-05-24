/* deklarasi library */
#include <PPMReader.h>


/* deklarasi variabel global disini */
int duty_cycle = 80;
const int reciever = 40;
int channelAmount = 8;
const int deadzone = 20;
//motor kiri
#define PWM_R1 2
#define PWM_L1 3
#define ENABLE_R1 22
#define ENABLE_L1 24
//motor depan surface
#define PWM_R2 4
#define PWM_L2 5
#define ENABLE_R2 26
#define ENABLE_L2 28
// motor kanan
#define PWM_R3 6
#define PWM_L3 7
#define ENABLE_R3 30
#define ENABLE_L3 32
//motor belakang surface
#define PWM_R4 8
#define PWM_L4 9
#define ENABLE_R4 34
#define ENABLE_L4 36

/* deklarasi objek library */
PPMReader ppm(reciever , channelAmount);

int pulseToPWM(int pulse){
  if (pulse >= 1000){
    pulse = map(pulse, 1000, 2000, -500, 500);
    pulse = constrain(pulse, -200, 200);
  } else{
    pulse = 0;
  }

  if (abs(pulse) <= deadzone) {
    pulse = 0;
  }

  return pulse;
}

//setup
void setup() {
  pinMode(PWM_R1, OUTPUT);
  pinMode(PWM_L1, OUTPUT);
  pinMode(ENABLE_R1, OUTPUT);
  pinMode(ENABLE_L1, OUTPUT);

  pinMode(PWM_R2, OUTPUT);
  pinMode(PWM_L2, OUTPUT);
  pinMode(ENABLE_R2, OUTPUT);
  pinMode(ENABLE_L2, OUTPUT);

  pinMode(PWM_R3, OUTPUT);
  pinMode(PWM_L3, OUTPUT);
  pinMode(ENABLE_R3, OUTPUT);
  pinMode(ENABLE_L3, OUTPUT);

  pinMode(PWM_R4, OUTPUT);
  pinMode(PWM_L4, OUTPUT);
  pinMode(ENABLE_R4, OUTPUT);
  pinMode(ENABLE_L4, OUTPUT);

  Serial.begin(9600);

}

//main loop
void loop() {
  unsigned long x_dir = ppm.latestValidChannelValue(1, 0);
  unsigned long y_dir = ppm.latestValidChannelValue(2, 0);
  unsigned long z_dir = ppm.latestValidChannelValue(3, 0);
  unsigned long arming_switch = ppm.latestValidChannelValue(5, 0);

  int x_pwm = pulseToPWM(x_dir);
  int y_pwm = pulseToPWM(y_dir);
  int z_pwm = pulseToPWM(z_dir);

  int left = y_pwm + x_pwm; left = constrain(left, -200, 200);
  int right = y_pwm - x_pwm; right = constrain(right, -200, 200);

  Serial.print(x_pwm);
  Serial.print(" ");
  Serial.print(y_pwm);
  Serial.print(" ");
  Serial.print(z_pwm);
  Serial.print(" ");
  Serial.println(arming_switch);

  //posisi disarm
  if (arming_switch < 1500){
    // disable semua motor
    digitalWrite(ENABLE_R1, LOW);
    digitalWrite(ENABLE_L1, LOW);
    digitalWrite(ENABLE_R2, LOW);
    digitalWrite(ENABLE_L2, LOW);
    digitalWrite(ENABLE_R3, LOW);
    digitalWrite(ENABLE_L3, LOW);
    digitalWrite(ENABLE_R4, LOW);
    digitalWrite(ENABLE_L4, LOW);
  }
  
  //posisi arming
  else if (arming_switch >= 1500){
    // enable semua motor
    digitalWrite(ENABLE_R1, HIGH);
    digitalWrite(ENABLE_L1, HIGH);
    digitalWrite(ENABLE_R2, HIGH);
    digitalWrite(ENABLE_L2, HIGH);
    digitalWrite(ENABLE_R3, HIGH);
    digitalWrite(ENABLE_L3, HIGH);
    digitalWrite(ENABLE_R4, HIGH);
    digitalWrite(ENABLE_L4, HIGH);
    
    motor_L(left);
    motor_R(right);
    motor_F(z_pwm);
    motor_B(z_pwm);

  }
  
}

//motor bagian kanan
void motor_R(int speed){
  if(speed > deadzone){
    analogWrite(PWM_L1, 0);
    analogWrite(PWM_R1, abs(speed));
  }
  else if(speed < deadzone){
    analogWrite(PWM_L1, abs(speed));
    analogWrite(PWM_R1, 0);
  }
  else{
    analogWrite(PWM_L1, 0);
    analogWrite(PWM_R1, 0);
  }
}

//motor bagian kiri
void motor_L(int speed){
  if(speed > deadzone){
    analogWrite(PWM_L3, 0);
    analogWrite(PWM_R3, abs(speed));
  }
  else if(speed < deadzone){
    analogWrite(PWM_L3, abs(speed));
    analogWrite(PWM_R3, 0);
  }
  else{
    analogWrite(PWM_L3, 0);
    analogWrite(PWM_R3, 0);
  }
}

//motor bagian depan
void motor_F(int speed){
  if(speed > deadzone){
    analogWrite(PWM_L2, abs(speed));
    analogWrite(PWM_R2, 0);
  }
  else if(speed < deadzone){
    analogWrite(PWM_L2, 0);
    analogWrite(PWM_R2, abs(speed));
  }
  else{
    analogWrite(PWM_L2, 0);
    analogWrite(PWM_R2, 0);
  }
  
 //motor bagian belakang
 void motor_B(int speed){
  if(speed > deadzone){
    analogWrite(PWM_L4, 0);
    analogWrite(PWM_R4, abs(speed));
  }
  else if(speed < deadzone){
    analogWrite(PWM_L4, abs(speed));
    analogWrite(PWM_R4, 0);
  }
  else{
    analogWrite(PWM_L4, 0);
    analogWrite(PWM_R4, 0);
  }
}
