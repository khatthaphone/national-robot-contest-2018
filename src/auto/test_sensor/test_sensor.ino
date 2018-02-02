  
  /**
 * @file SensorTesting.ino
 * @brief Arduino auto robot algorithm hard core coding.
 * @uathor Tee Hunter
 */
 
//count line 
  int countSensor = 0; // count line position
  int lastStateSensor = 0; // Check state before count line position

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

// set PIN motor and power sline 
  #define dir3 5
  #define pwm3 4

// top switch and bottom switch are stopping the slide motor
  #define topSW 18    // TODO: define new pin depending on the curcuit
  #define bottomSW 20 // TODO: define new pin depending on the curcuit

void setup() {
  Serial.begin(250000);
  //line direction sensor
  pinMode(sensorT1, INPUT);
  pinMode(sensorT2, INPUT);
  pinMode(sensorT3, INPUT);
  pinMode(sensorT4, INPUT);

  //motor and power rolling
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);

  //motor and power sline
  pinMode(dir3, OUTPUT);
  pinMode(pwm3, OUTPUT);
  
} //Ending setup fucntion



void loop() 
{
  controlSenser(); //read data from sensor
  countLine(); //count line with sensorT1 ==> sensorT5 == HIGH

  static int stateZone = 1;
  static int checkZone = 0;

  switch (stateZone) // TODO: just wanna move forwadr only to try understanding what should do next 
  {
    //startZone
    case 0:
      if (checkZone == 0) stateZone =1;
      stopMotor(0, 0);
      break;
    
    //move foreward only
    case 1:
      forwardControl (100, 100);
      break;

    //
     
    
  }
  
  
} //Ending loop function

// count line to know when and where the robot can stop - turn or release
void countLine()
{
  bool checkStateStatus = ((dataSensorT1 != lastStateSensor) && (dataSensorT2 != lastStateSensor) && (dataSensorT3 != lastStateSensor) && (dataSensorT4 != lastStateSensor) && (dataSensorT5 != lastStateSensor));
  bool updateLineCount = ((dataSensorT1 == HIGH) && (dataSensorT2 == HIGH) && (dataSensorT3 == HIGH) && (dataSensorT4 == HIGH) && (dataSensorT5 == HIGH));
  if (checkStateStatus)
  {
    if (updateLineCount)
    {
      countSensor++;
      Serial.print("Line: ");
      Serial.println(countSensor);
      delay(10);
    }
    delay(20);
  }
  lastStateSensor = dataSensorT1 && dataSensorT2 && dataSensorT3 && dataSensorT4 && dataSensorT5;
  delay(1000); //TODO Dellet when down coding
  }


// read data sensor function
void controlSenser()
{
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
}

 /*
  * control robot moving stable 
  */
// TODO: Must set the pwmRight, pwmLeft and HIGH or LOW realize 
void stopMotor(int pwmRight, int pwmLeft)
{
  //right motor
  digitalWrite(dir1, HIGH);      // PIN 12
  analogWrite(pwm1, pwmRight);         // PIN 11
  //left motor
  digitalWrite(dir2, HIGH);      // PIN 10
  analogWrite(pwm2, pwmLeft);         // PIN 9
}

//TODO: Must set the pwmRight, pwmLeft and HIGH or LOW realize
void forwardControl (int pwmRight, int pwmLeft)
{
  if (dataSensorT3 == 1)
  {
  //right motor
  digitalWrite(dir1, HIGH);     // PIN 12
  analogWrite(pwm1, pwmRight);           // PIN 11
  //left motor
  digitalWrite(dir2, HIGH);     // PIN 10
  analogWrite(pwm2, pwmLeft);            // PIN 9
  }
  
}

//TODO: Must set the pwmRight, pwmLeft and HIGH or LOW realize
void backwardControl (int pwmRight, int pwmLeft)
{
  //right motor
  digitalWrite(dir1, HIGH);     // PIN 12
  analogWrite(pwm1, pwmRight);  // PIN 11
  //left motor
  digitalWrite(dir2, HIGH);      // PIN 10
  analogWrite(pwm2, pwmLeft);   // PIN 9
}

// if sensorT1
void rightcontrol()
{
  forwardControl(150, 100);
}

//if sensorT4
void leftControl()
{
  forwardControl(100, 150);
}











