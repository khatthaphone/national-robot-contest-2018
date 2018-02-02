/**
 * @file real_code_of_auto_2018.ino
 * @brief Arduino auto robot algorithm hard core coding.
 * @uathor Tee Hunter
 */
 
//count line 
  int countSensor = 0; // count line position
  int lastStateSensor = 0; // Check state before count line position

//set PIN data of line direction sensor 
  #define  sensorT1 44
  #define  sensorT2 46
  #define  sensorT3 48
  #define  sensorT4 50
  #define  sensorT5 52

 //data condition of auto robot must do 
  int start = 6;
  static int getLine = -1;
  static int dropLine = -1;
  static int backLocation = 0;
  int startSlide = 1;

 //save to read the reperent from line sensor
  int dataSensorT1 = 0;
  int dataSensorT2 = 0;
  int dataSensorT3 = 0;
  int dataSensorT4 = 0;
  int dataSensorT5 = 0;

 // set PIN motor and power rolling
  #define dir1 9
  #define dir2 11
  #define pwm1 10
  #define pwm2 12

 // set PIN motor and power sline 
  #define dir3 7
  #define pwm3 8

 // top switch and bottom switch are stopping the slide motor
  #define topSW 18
  #define bottomSW 20

 // set switch control
   int dataSWTop;
   int dataSWBottom;
   int SWLastStateTop;
   int SWLastStateBottom;

void setup()
{
  Serial.begin(250000);

  //motor and power rolling
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);

  //motor and power sline
  pinMode(dir3, OUTPUT);
  pinMode(pwm3, OUTPUT);

  //line direction sensor
  pinMode(sensorT1, INPUT);
  pinMode(sensorT2, INPUT);
  pinMode(sensorT3, INPUT);
  pinMode(sensorT4, INPUT);
  pinMode(sensorT5, INPUT);

  //top switch and bottom stopping the slide motor
  pinMode(topSW, INPUT);
  pinMode(bottomSW, INPUT);
  
} // setup ending

void loop()
{
  controlSensor(); //read data from sensor
  countLine(); //count line with sensorT1 ==> sensorT5 == HIGH
  switchControl();

 //indentifine zone
 static int stateZone = 0;
 static int checkZone = 0;

 switch (stateZone)
 {
  case 0:
    checkZone = startZone();
    if (checkZone == 1) stateZone = 1; // position at the get zone
    break;
  case 1:
    checkZone = getZone();  //TODO1 look at 141
    if (checkZone == 3) stateZone = 3; // position at the back zone
    break;
  case 2:
    checkZone = dropZone(); //TODO3 look at 145
    if (checkZone == 0) stateZone = 0; // position at the start zone
    break;
  case 3: 
    checkZone = backZone(); //TODO4 look at 149
    if (checkZone == 2) stateZone = 2; // position at the drop zone
    break; 
 }
} // ending loop function

int startZone()
{
  switchControl();// get the value from swicth status
  switchState(); // check status state of switch

  //TODO2 look at line 163 and 1042 
  while (startSlide <= 1) // start check slide motor
  {
    armSlideSetup();
    
  }
}

int getZone()
{
  //TODO1.1, then call int fucntion loop
}

int dropZone()
{
  //TODO3.1, then call int fucntion loop
}

int backZone()
{
  //TODO4.1, then call int fucntion loop
}

//count line this is the heard of this code
void countLine()
{
  bool checkStateStatus = ((dataSensorT1 != lastStateSensor) && (dataSensorT2 != lastStateSensor) && (dataSensorT3 != lastStateSensor) && (dataSensorT4 != lastStateSensor) && (dataSensorT5 != lastStateSensor));
  bool updateLineCount = ((dataSensorT1 == HIGH) && (dataSensorT2 == HIGH) && (dataSensorT3 == HIGH) && (dataSensorT4 == HIGH) && (dataSensorT5 == HIGH));
  if (checkStateStatus)
  {
    if (updateLineCount)
    {
      countSensor++;
      Serial.println("Line: " + countSensor);
      delay(10);
    }
    delay(20);
  }
  lastStateSensor = dataSensorT1 && dataSensorT2 && dataSensorT3 && dataSensorT4 && dataSensorT5;
  }

// read data sensor function
void controlSensor()
{
  dataSensorT1 = digitalRead(sensorT1);
  dataSensorT2 = digitalRead(sensorT2);
  dataSensorT3 = digitalRead(sensorT3);
  dataSensorT4 = digitalRead(sensorT4);
  dataSensorT5 = digitalRead(sensorT5);
}

// slide up and down function control
void switchControl()
{
  //stop slide
  dataSWTop = digitalRead(topSW);
  dataSWBottom = digitalRead(bottomSW);
}

void switchStateTop()
{
  if (dataSWTop != SWLastStateTop)
  {
    if (dataSWTop == HIGH)
    {
      motorArmSlideBack(0);
      Serial.print("switch top is clicking : ");
      delay(10);
    }
    delay(20);
  }
  SWLastStateTop = dataSWTop;
}

void switchStateBottom()
{
    if (dataSWBottom != SWLastStateBottom)
  {
    if (dataSWBottom == HIGH)
    {
      motorArmSlideStart(0);
      Serial.print("switch bottom is clicking : ");
      delay(10);
    }
    delay(20);
  }
  SWLastStateBottom = dataSWBottom;
}

void switchState(){
  switchStateTop();
  switchStateBottom();
}

void motorArmSlideStart(int setPWM3)
{
  // start motor slide
  digitalWrite(dir3, HIGH);   // PIN 7
  analogWrite(pwm3, setPWM3); // PIN 8
}

void motorArmSlideBack(int setPWM3)
{
  //  slide motor back
  digitalWrite(dir3, LOW);    // PIN 7
  analogWrite(pwm3, setPWM3); // PIN 8
}

void armSlideSetup()
{
  
}



