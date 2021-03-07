//inlcude relevant libraries and initialize pins

//IR remote setup
#include <IRremote.h>

const int IRpin = 10;

//stepper setup

#include <Stepper.h>

Stepper stepperL(200, 2, 3, 4, 5);
Stepper stepperR(200, 6, 7, 8, 9);

//ultrasonic pins set up
const int trigpin = 12;
const int echopin = 13;
long dist = 0;
const int empty = 4;

//servo setup
#include  <Servo.h>           
 
Servo servo_test;      
int servopin = 11;


//SETUP

void setup() {

  //set up input and output pins, speeds, etc
  Serial.begin(9600);
  
  IrReceiver.begin(IRpin, ENABLE_LED_FEEDBACK);

  stepperL.setSpeed(100);
  stepperR.setSpeed(100);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  servo_test.attach(servopin);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
}

long val = 0;
//decode_results val;


//stepper initial
int left = 0;
int right = 0;

//servo initial variables
int butter = 10;
int pos = 20;
int inc = 10;

//ultra sonic initial variable
int distlim = 500;
int tooclose = LOW;
int tooclosepos = 0;



//LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP

void loop() {
//set up stepper values for their relative side
  
  stepperL.step(left);
  stepperR.step(right);


  if (butter == 50){
    
  
  servo_test.write(pos);
  //Serial.println(pos);
  

  //ultrasonic check
        digitalWrite(trigpin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigpin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigpin, LOW);
        dist = pulseIn(echopin, HIGH);
        dist;
        Serial.print("distance: ");
        Serial.println(dist);

        if(pos == tooclosepos){
          if (dist > distlim){
            tooclose = LOW;
            digitalWrite(A0, LOW);
          }
        }

//failsafe for if the distance is too short, indicating an obstacle

        if (dist <= distlim){
          tooclose = HIGH;
          digitalWrite(A0, HIGH);
          tooclosepos = pos;

          if (left==1 && right==-1){
            left = 0;
            right = 0;
          }
        }


        //rotating the servo continuosly to look for obstacles
  
  if (pos != 170 && pos != 10){
    
  }
  else if (pos == 170){
    inc = -10;
  }
  else if (pos == 10){
    inc = 10;
  }

  pos = pos + inc;

  

  butter = 0;
  }
  
  if (left != 0 && right != 0){
    butter++;
  }
  


//check for input from the IR remote
  

  if(IrReceiver.decode()){ 
     
    long val = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(val);


    //based on the value (either turn left, forward or turn right), set the stepper speed

    switch(val){
      case(-1086259456):  //forward
      stepperL.setSpeed(100);
      stepperR.setSpeed(100);
      
      if (left == 1 && right == -1){
        left = 0;
        right = 0;
      }
      else{
        if (tooclose == LOW){
          left = 1;
          right = -1;
        }
      }
      break;

      case(-1153106176):  //left
      stepperL.setSpeed(100);
      stepperR.setSpeed(100);
      left = -1;
      right = -1;
      break;

      case(-1136394496):  //right
      stepperL.setSpeed(100);
      stepperR.setSpeed(100);
      left = 1;
      right = 1;
      break;


      
    }
    IrReceiver.resume();
}
}
