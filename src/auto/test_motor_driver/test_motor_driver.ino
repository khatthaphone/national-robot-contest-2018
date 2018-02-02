//set PIN data of line direction sensor
#define  sensorT1 22
#define  sensorT2 23
#define  sensorT3 24
#define  sensorT4 25
#define  sensorT5 26

//save to read the reperent from line sensor
int dataSensorT1 = 0;
int dataSensorT2 = 0;
int dataSensorT3 = 0;
int dataSensorT4 = 0;
int dataSensorT5 = 0;

// set PIN motor and power rolling
#define dir1 12
#define dir2 10
#define pwm1 11
#define pwm2 9

void setup() {
  Serial.begin(250000);
  //line direction sensor
  pinMode(sensorT1, INPUT);
  pinMode(sensorT2, INPUT);
  pinMode(sensorT3, INPUT);
  pinMode(sensorT4, INPUT);
  pinMode(sensorT5, INPUT);

  //motor and power rolling
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);

} //Ending setup fucntion

void loop() {

  dataSensorT1 = digitalRead(sensorT1); // PIN 22
  dataSensorT2 = digitalRead(sensorT2); // PIN 23
  dataSensorT3 = digitalRead(sensorT3); // PIN 24
  dataSensorT4 = digitalRead(sensorT4); // PIN 25
  dataSensorT5 = digitalRead(sensorT5); // PIN 26

  Serial.print("sensorT1 : ");
  Serial.println(dataSensorT1);
  Serial.print("sensorT2 : ");
  Serial.println(dataSensorT2);
  Serial.print("sensorT3 : ");
  Serial.println(dataSensorT3);
  Serial.print("sensorT4 : ");
  Serial.println(dataSensorT4);
  Serial.print("sensorT5 : ");
  Serial.println(dataSensorT5);
  Serial.println("-------------");

      if ((dataSensorT1 == 0) && (dataSensorT2 == 1) && (dataSensorT3 == 1) && (dataSensorT4 == 1)&& (dataSensorT5 == 0))
      {
              forward(50, 49);
      }
      // sensorT4 out == move right faster than left
      // 1 110 0
      if ((dataSensorT1 == 1) && (dataSensorT2 == 1) && (dataSensorT3 == 1) && (dataSensorT4 == 0)&& (dataSensorT5 == 0))
      {
        rightFast(100, 50);
      }
      // sensorT2 out == move left faster than right
      // 0 011 1
      if ((dataSensorT1 == 0) && (dataSensorT2 == 0) && (dataSensorT3 == 1) && (dataSensorT4 == 1)&& (dataSensorT5 == 1))
      {
        leftFast(50, 100);
      }
      // 
      // 0 001 1
      if ((dataSensorT1 == 0) && (dataSensorT2 == 0) && (dataSensorT3 == 0) && (dataSensorT4 == 1)&& (dataSensorT5 == 1))
      {
        hardLeft(150, 75);
        delay(300);
        forward(50, 49);
        delay(800);
      }
            // 
      // 1 100 0
      if ((dataSensorT1 == 1) && (dataSensorT2 == 1) && (dataSensorT3 == 0) && (dataSensorT4 == 0)&& (dataSensorT5 == 0))
      {
        hardRight(75, 120);
        delay(100);
      }
      // 0 000 0
      if ((dataSensorT1 == 1) && (dataSensorT2 == 1) && (dataSensorT3 == 0) && (dataSensorT4 == 0)&& (dataSensorT5 == 0))
      {
        forward(0, 0);
        delay(100);
      }
    
  delay(100);

}

void forward(int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, setPWM2);
}

void rightFast(int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, setPWM2);
}

void leftFast(int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, setPWM2);
}

void hardLeft(int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, setPWM2);
}

void hardRight(int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, LOW);
  analogWrite(pwm2, setPWM2);
}

void uturnRightZone (int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, setPWM2);
}

void uturnLeftZone (int setPWM1, int setPWM2)
{
  //right motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, setPWM1);
  //left motor
  digitalWrite(dir2, LOW);
  analogWrite(pwm2, setPWM2);
}


