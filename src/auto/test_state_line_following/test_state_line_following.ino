//CEIT_Robot_2018 In Loas

int count_sensor = 0;// Count Line 
int last_state_sensor = 0; //Check State before Count Line
    
//set PIN data of line direction sensor
#define  sensorT1 22
#define  sensorT2 23
#define  sensorT3 24
#define  sensorT4 25
#define  sensorT5 26
#define  sensorT6 6
#define  sensorT7 7 

//save to read the reperent from line sensor
int data_sensor_1 = 0;
int data_sensor_2 = 0;
int data_sensor_3 = 0;
int data_sensor_4 = 0;
int data_sensor_5 = 0;
int data_sensor_6 = 0;
int data_sensor_7 = 0;

// set PIN motor and power rolling
#define dir1 12
#define dir2 10
#define pwm1 11
#define pwm2 9

// in line speed 
int pwm_l = 70;
int pwm_r = 70;

// turn in line speed 
int pwmTurn_l = 35;
int pwmTurn_r = 35;

// uturn zone speed 
int pwmUturn_l = 60;
int pwmUturn_r = 60;

// states of the robot
#define START 0
#define WAIT 1
#define GO 2
#define DROP_IN 3
#define DROP_OUT 4
#define BACK 5

// colors
#define GREEN 1
#define RED 0
#define BLUE 2

// state
int state = START;
// color
int color = GREEN;
// count for checkpoint count
int count = 0;

// TODO: Infared sensor to know if the robot have a box
int infared = 0;

void setup() {
  Serial.begin(250000);
  //line direction sensor
  pinMode(sensorT1, INPUT);
  pinMode(sensorT2, INPUT);
  pinMode(sensorT3, INPUT);
  pinMode(sensorT4, INPUT);
  pinMode(sensorT5, INPUT);
  pinMode(sensorT6, INPUT);
  pinMode(sensorT7, INPUT);

  //motor and power rolling
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);

} //Ending setup fucntion

void loop() {

  switch(state) {
    case START:
      controlFollowLine(); //FollowLine
      if (detectCheckpoint()) {
        delay(225);
        state = WAIT;
      }
      break;
    case WAIT:
      forward(0, 0);
      
      // replace with wait for infared sonsor
        delay(1000);
        
      forward(pwm_l, pwm_r);
      delay(200);
      state = GO;
      
      break;
    case GO:
      controlFollowLine(); //FollowLine
      if (detectCheckpoint()) {
        delay(100);
        ++count;
      }
      // count == count to go for ccolor
      if (count == goCheckpointCount(color)) {
        delay(500);
        count = 0;
        state = DROP_IN;
      }
      break;
      
    case DROP_IN:
      forward(0, 0);
      delay(200);

      // turn left to drop
      uturnLeftZone(pwmUturn_l, pwmUturn_r);
      delay(200);

      // stop when turn succeed
      if (dropInTurnSucceed())
      {
        state = DROP_OUT;
      }
      
      break;
      
    case DROP_OUT:
      forward(0, 0);
      delay(200);

      // turn right to get out
      uturnRightZone(pwmUturn_l, pwmUturn_r);
      delay(200);
      
      // stop when turn succeed
      if (!dropInTurnSucceed())
      {
        forward(pwm_l, pwm_r);
        delay(200);
        state = BACK;
      }
      break;
      
    case BACK:

      controlFollowLine(); //FollowLine
      if (detectCheckpoint()) {
        delay(100);
        ++count;
      }
      if (count == backCheckpointCount(color)) {
        state = WAIT;
        color = changeToNextColor(color);
        count = 0; 
      }

      break;
  }
  
//    controlFollowLine(); //FollowLine
//    control_Sensor(); //Read Data From Sensor
    
//  Serial.print("sensorT1 : ");
//  Serial.println(data_sensor_1);
//  Serial.print("sensorT2 : ");
//  Serial.println(data_sensor_2);
//  Serial.print("sensorT3 : ");
//  Serial.println(data_sensor_3);
//  Serial.print("sensorT4 : ");
//  Serial.println(data_sensor_4);
//  Serial.print("sensorT5 : ");
//  Serial.println(data_sensor_5);
//  Serial.print("sensorT6 : ");
//  Serial.println(data_sensor_6);
//  Serial.print("sensorT7 : ");
//  Serial.println(data_sensor_7);
//  Serial.println("-------------");
//  delay(5000);

} //End_Loop

//control_Sensor
  void control_Sensor()
  {
    //Line Sensor
  data_sensor_1 = digitalRead(sensorT1); // PIN 22
  data_sensor_2 = digitalRead(sensorT2); // PIN 23
  data_sensor_3 = digitalRead(sensorT3); // PIN 24
  data_sensor_4 = digitalRead(sensorT4); // PIN 25
  data_sensor_5 = digitalRead(sensorT5); // PIN 26
  data_sensor_6 = digitalRead(sensorT6); // PIN 6
  data_sensor_7 = digitalRead(sensorT7); // PIN 7   
  }
  
// TODO: sensorT3 is not working must change the new once
void controlFollowLine() 
{
      control_Sensor();
      
      // always moving
      if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      { 
              forward(pwm_l, pwm_r);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      // turn right in line 
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmTurn_l, pwmTurn_r);
      }
      // turn left in line 
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmTurn_l, pwmTurn_r);
      }
      // turn right at the uturn zone
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l - 5, pwmUturn_r - 5);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(5);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(10);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l - 5, pwmUturn_r - 5);
        delay(20);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(18);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 1) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l + 13, pwmUturn_r + 25);
        delay(35); 
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l + 15, pwmUturn_r + 5);
        delay(40);
      }
      //turn left at the uturn zone
       else if ((data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l - 5, pwmUturn_r - 5);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l - 5, pwmUturn_r - 5);
        delay(5);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(10);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(20);
      }
      else if ((data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(18);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 1) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l + 25, pwmUturn_r + 13);
        delay(35);
      }
      else if ((data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l + 5, pwmUturn_r + 15);
        delay(40);
      } 
      else 
      {
        forward(0, 0);
      }
      //delay(100);
} //end control follow line


bool detectCheckpoint()
  {
    bool isAtCheckpoint = false;
    
    if(data_sensor_1 != last_state_sensor  && data_sensor_2 != last_state_sensor && data_sensor_3 != last_state_sensor && data_sensor_4 != last_state_sensor && data_sensor_5 != last_state_sensor)
    {
      if(data_sensor_1 == 1 && data_sensor_2 == 1 && data_sensor_3 == 1 && data_sensor_4 == 1 && data_sensor_5 == 1)
      {
        isAtCheckpoint = true;
        Serial.print("Line: ");
        Serial.println(count_sensor);
      }
    }
    last_state_sensor = data_sensor_1 && data_sensor_2 && data_sensor_3 &&  data_sensor_4 && data_sensor_5;

    return isAtCheckpoint;
  }

bool dropInTurnSucceed() {
  control_Sensor();

  if ((data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
  {
    return true;
  }

  return false;
}

void forward(int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, pwm_l);
  //right motor
  digitalWrite(dir2, LOW);
  analogWrite(pwm2, pwm_r);
} // End forward 

void backward(int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, pwm_l);
  //right motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, pwm_r);
} // end backward


void uturnLeftZone (int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(dir1, HIGH);
  analogWrite(pwm1, pwm_l);
  //right motor
  digitalWrite(dir2, LOW);
  analogWrite(pwm2, pwm_r);
}

void uturnRightZone (int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(dir1, LOW);
  analogWrite(pwm1, pwm_l);
  //right motor
  digitalWrite(dir2, HIGH);
  analogWrite(pwm2, pwm_r);
}

int goCheckpointCount(int color) {
  switch(color) {
    case GREEN: return 2;
    case BLUE: return 3;
    case RED: return 1;
  }
}

int backCheckpointCount(int color) {
  switch(color) {
    case GREEN: return 2;
    case BLUE: return 1;
    case RED: return 3;
  }
}
int changeToNextColor(int color) {
    switch(color) {
    case GREEN: return BLUE;
    case BLUE: return RED;
    case RED: return GREEN;
  }
}

