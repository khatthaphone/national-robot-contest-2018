//CEIT_Robot_2018 In Loas

// in line speed 
int pwm_l = 100;
int pwm_r = 100;

int count_sensor = 0;// Count Line 
int last_state_sensor = 0; //Check State before Count Line
    
//set PIN data of line direction sensor
#define  SENSOR_T1 22
#define  SENSOR_T2 23
#define  SENSOR_T3 24
#define  SENSOR_T4 25
#define  SENSOR_T5 26
#define  SENSOR_T6 6
#define  SENSOR_T7 7 

//save to read the reperent from line sensor
int data_sensor_1 = 0;
int data_sensor_2 = 0;
int data_sensor_3 = 0;
int data_sensor_4 = 0;
int data_sensor_5 = 0;
int data_sensor_6 = 0;
int data_sensor_7 = 0;

// set PIN motor and power rolling
#define DIR1 12
#define DIR2 10
#define PWM1 11
#define PWM2 9

// turn in line speed 
int pwmTurn_l = 45;
int pwmTurn_r = 45;

// uturn zone speed 
int pwmUturn_l = 60;
int pwmUturn_r = 60;

// states of the robot
#define START 0
#define WAIT 1
#define GO 2
#define DROP_IN 3
#define BACK 4

// colors
#define GREEN 1
#define RED 0
#define BLUE 2

// initial state
int state = START;
// initla box color
int color = -1;
// initial count for checkpoint count
int count = 0;

// IR Box detect
#define BOX_IR 27

// slider
#define PWM_SLDR 4
#define SLDR 5

// Switch
#define SLDR_SW_TOP 48
#define SLDR_SW_BTM 46

// Start Color Swtiches
#define RED_SWITCH 49
#define BLUE_SWITCH 51
bool using_plan_b = false;

void setup() {
  Serial.begin(115200);
  //line direction sensor
  pinMode(SENSOR_T1, INPUT);
  pinMode(SENSOR_T2, INPUT);
  pinMode(SENSOR_T3, INPUT);
  pinMode(SENSOR_T4, INPUT);
  pinMode(SENSOR_T5, INPUT);
  pinMode(SENSOR_T6, INPUT);
  pinMode(SENSOR_T7, INPUT);

  //motor and power rolling
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);

  // Slider
  pinMode(PWM_SLDR, OUTPUT);
  pinMode(SLDR, OUTPUT);

  // Switches
  pinMode(SLDR_SW_TOP, INPUT);
  pinMode(SLDR_SW_BTM, INPUT);

  // Starting color switches
  pinMode(BLUE_SWITCH, INPUT);
  pinMode(RED_SWITCH, INPUT);
  
} //Ending setup fucntion

void loop() {
  Serial.print("state = ");
  Serial.print(state);
  Serial.print("   count = ");
  Serial.println(count);

  switch(state) {
    case START:

      forward(0, 0);

      if (color == -1)
      {
        delay(1000);
        if (digitalRead(BLUE_SWITCH))
        {
          color = BLUE;
          using_plan_b = true;
        }
        else if (digitalRead(RED_SWITCH))
        {
          color = RED;
          using_plan_b = true;
        }
        else {
          color = GREEN;
        }
  
        delay(1000);
      }
      
      // wait for plab B (use switches to specify starting box color
    
      controlFollowLine(); //FollowLine
      if (detectCheckpoint())
      {
        forward(0, 0);
        delay(1000);
        state = WAIT;
      }
      break;
      
    case WAIT:

      forward(0, 0);

      // wait for the first box
      do {
        delay(100);
      } while (digitalRead(BOX_IR));
      // wait for the second box
      delay(12000);

      forward(pwm_l, pwm_r);
      delay(300);
      
      state = GO;
      
      // color = getBoxColor();

      // if (color != -1) {
//          state = GO;
      // } else {
      //   color = getBoxColor();
      // }
      
      break;
    case GO:
      controlFollowLine(); //FollowLine
      if (detectCheckpoint()) {
        delay(100);
        ++count;
      }
      // count == counts to go for color
      if (count == goCheckpointCount(color)) {
        count = 0;
        state = DROP_IN;
      }
      break;
      
    case DROP_IN:
      forward(0, 0);
      
      // Drop the box(es)
      while (digitalRead(SLDR_SW_TOP)) {
        analogWrite(PWM_SLDR, 200);
        digitalWrite(SLDR, HIGH);
      }

      analogWrite(PWM_SLDR, 0);
      
      if (!digitalRead(SLDR_SW_TOP)) {
        analogWrite(PWM_SLDR, 0);
        delay(1000);
        forward(pwm_l, pwm_r);
        delay(300);
        state = BACK;
      }
      
      break;
      
    case BACK:

      while (digitalRead(SLDR_SW_BTM)) {
        controlFollowLine(); //FollowLine
        if (detectCheckpoint()) {
          delay(100);
          ++count;
        }
        
        analogWrite(PWM_SLDR, 200);
        digitalWrite(SLDR, LOW);

        if (count == backCheckpointCount(color)) {
          forward(0, 0);
          analogWrite(PWM_SLDR, 0);
          state = WAIT;
          color = changeToNextColor(color);
          count = 0;
          break;
        }
      }
      analogWrite(PWM_SLDR, 0);

      controlFollowLine(); //FollowLine
      if (detectCheckpoint()) {
        delay(100);
        ++count;
      }
      if (count == backCheckpointCount(color)) {
        forward(0, 0);
        analogWrite(PWM_SLDR, 0);
        color = changeToNextColor(color);
        state = WAIT;
        count = 0;
      }

      break;
  }
  
//    controlFollowLine(); //FollowLine
//    control_Sensor(); //Read Data From Sensor
    
  Serial.print("SENSOR_T1 : ");
  Serial.println(data_sensor_1);
  Serial.print("SENSOR_T2 : ");
  Serial.println(data_sensor_2);
  Serial.print("SENSOR_T3 : ");
  Serial.println(data_sensor_3);
  Serial.print("SENSOR_T4 : ");
  Serial.println(data_sensor_4);
  Serial.print("SENSOR_T5 : ");
  Serial.println(data_sensor_5);
  Serial.print("SENSOR_T6 : ");
  Serial.println((!data_sensor_6));
  Serial.print("SENSOR_T7 : ");
  Serial.println(data_sensor_7);
  Serial.println("-------------");

} //End_Loop

//control_Sensor
  void control_Sensor()
  {
    //Line Sensor
  data_sensor_1 = digitalRead(SENSOR_T1); // PIN 22
  data_sensor_2 = digitalRead(SENSOR_T2); // PIN 23
  data_sensor_3 = digitalRead(SENSOR_T3); // PIN 24
  data_sensor_4 = digitalRead(SENSOR_T4); // PIN 25
  data_sensor_5 = digitalRead(SENSOR_T5); // PIN 26
  data_sensor_6 = digitalRead(SENSOR_T6); // PIN 6
  data_sensor_7 = digitalRead(SENSOR_T7); // PIN 7   
  }
  
// TODO: SENSOR_T3 is not working must change the new once
void controlFollowLine() 
{
      control_Sensor();
      
      // always moving
      if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      { 
              forward(pwm_l, pwm_r);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
              forward(pwm_l, pwm_r);
      }
      else if  ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      { 
              forward(pwm_l, pwm_r);
      }
      else if  ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      { 
              forward(pwm_l, pwm_r);
      }
      // turn right in line 
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmTurn_l, pwmTurn_r);
      }
      // turn left in line 
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmTurn_l, pwmTurn_r);
      }
      // turn right at the uturn zone
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l - 5, pwmUturn_r - 5);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(5);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(10);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 1)&& (data_sensor_5 == 1) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l - 5, pwmUturn_r - 5);
        delay(20);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 1) && (data_sensor_7 == 0))
      {
        uturnRightZone(pwmUturn_l, pwmUturn_r);
        delay(18);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 1) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l + 13, pwmUturn_r + 25);
        delay(35); 
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 1))
      {
        uturnRightZone(pwmUturn_l + 15, pwmUturn_r + 5);
        delay(40);
      }
      //turn left at the uturn zone
       else if ((!data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l - 5, pwmUturn_r - 5);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l - 5, pwmUturn_r - 5);
        delay(5);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(10);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 1) && (data_sensor_2 == 1) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(20);
      }
      else if ((!data_sensor_6 == 0) && (data_sensor_1 == 1) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l, pwmUturn_r);
        delay(18);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 1) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
      {
        uturnLeftZone(pwmUturn_l + 25, pwmUturn_r + 13);
        delay(35);
      }
      else if ((!data_sensor_6 == 1) && (data_sensor_1 == 0) && (data_sensor_2 == 0) && (data_sensor_3 == 0) && (data_sensor_4 == 0)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
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
//        Serial.print("Line: ");
//        Serial.println(count_sensor);
      }
    }
    last_state_sensor = data_sensor_1 && data_sensor_2 && data_sensor_3 &&  data_sensor_4 && data_sensor_5;

    return isAtCheckpoint;
  }

bool dropInTurnSucceed() {
  control_Sensor();

  if ((!data_sensor_6 == 0) && (data_sensor_1 == 0) && (data_sensor_2 == 1) && (data_sensor_3 == 1) && (data_sensor_4 == 1)&& (data_sensor_5 == 0) && (data_sensor_7 == 0))
  {
    return true;
  }

  return false;
}

void forward(int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, pwm_l);
  //right motor
  digitalWrite(DIR2, LOW);
  analogWrite(PWM2, pwm_r);
} // End forward 

void backward(int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, pwm_l);
  //right motor
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, pwm_r);
} // end backward


void uturnLeftZone (int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, pwm_l);
  //right motor
  digitalWrite(DIR2, LOW);
  analogWrite(PWM2, pwm_r);
}

void uturnRightZone (int pwm_l, int pwm_r)
{
  //left motor
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, pwm_l);
  //right motor
  digitalWrite(DIR2, HIGH);
  analogWrite(PWM2, pwm_r);
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
// change current box color
int changeToNextColor(int color) {
    switch(color) {
    case GREEN: return RED;
    case RED:   return BLUE;
    case BLUE:  return GREEN;
  }
}

bool sliderOnTop() {
  bool onTop = false;
  int sldr_count = 0;

  int i;
  for (i = 0; i < 5; i++) {
    if (onTop == !digitalRead(SLDR_SW_TOP)) {
      sldr_count++;
    } else {
      onTop = !digitalRead(SLDR_SW_TOP);
    }
    delay(15);
  }

  if (count >= 3) {
    return true;
  }

  return false;

}

bool sliderOnBottom() {
  bool onBottom = false;
  int sldr_count = 0;

  int i;
  for (i = 0; i < 5; i++) {
      if (onBottom == !digitalRead(SLDR_SW_TOP)) {
      sldr_count++;
    } else {
      onBottom = !digitalRead(SLDR_SW_TOP);
    }
    delay(15);
  }

  if (count >= 3) {
    return true;
  }

  return false;

}
