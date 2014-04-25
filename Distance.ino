#include <Servo.h>                                  // Include Servo library

Servo steering;                                      // create servo object to control a servo 
 
int pos=0;                                          // variable to store the servo position

int URPWM=3;                                        // PWM Output 0-25000us,every 50us represent 1cm
int URTRIG=5;                                       // PWM trigger pin

int URPWM2=4;                                        // PWM Output 0-25000us,every 50us represent 1cm
int URTRIG2=6;                                       // PWM trigger pin

boolean up=true;                                    // create a boolean variable
boolean up2=true;                                    // create a boolean variable
boolean steer=false;
boolean steerRight=false;

unsigned long time;                                 // create a time variable
unsigned long urmTimer = 0;                          // timer for managing the sensor reading flash rate
unsigned int Distance=0;

unsigned long urmTimer2 = 0;                          // timer for managing the sensor reading flash rate
unsigned int Distance2 = 0;

uint8_t EnPwmCmd[4]={0x44,0x22,0xbb,0x01};          // distance measure command
 
void setup(){                                      // Serial initialization
  Serial.begin(9600);                              // Sets the baud rate to 9600
  steering.attach(9);                              // Pin 9 to control servo
  steering.write(43);                              // starts servo at 43 degrees
  
  PWM_Mode_Setup();
  PWM_Mode_Setup_Right();
}
 
void loop(){
   if(millis()-urmTimer > 50){
     urmTimer=millis(); 

     PWM_Mode();
     PWM_Mode_Right();
   }
}
 
void PWM_Mode_Setup(){ 
  pinMode(URTRIG,OUTPUT);                            // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                         // Set to HIGH
  
  pinMode(URPWM, INPUT);                             // Sending Enable PWM mode command
  
  for(int i=0;i<4;i++){
      Serial.write(EnPwmCmd[i]);
   } 
}

void PWM_Mode_Setup_Right(){ 
  pinMode(URTRIG2,OUTPUT);                            // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG2,HIGH);                         // Set to HIGH
  
  pinMode(URPWM2, INPUT);                             // Sending Enable PWM mode command
  
  for(int i=0;i<4;i++){
      Serial.write(EnPwmCmd[i]);
   } 
}
 
void PWM_Mode(){                                     // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW); 
    digitalWrite(URTRIG, HIGH);                      // reading Pin PWM will output pulses
     
    unsigned long DistanceMeasured=pulseIn(URPWM,LOW);
     
    if(DistanceMeasured==50000){                     // the reading is invalid.
      Serial.print("Invalid");    
   }
    else{
      Distance=DistanceMeasured/50;                  // every 50us low level stands for 1cm
      
      if(steer == false){
        if(Distance <= 10){
          goLeft();
          steer = true;
        }
      }
      else{
        if(Distance >= 10){
          forwardFromLeft();
          steer = false;
        }  
      }
   }
}

void PWM_Mode_Right(){                                     // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG2, LOW); 
    digitalWrite(URTRIG2, HIGH);                      // reading Pin PWM will output pulses
     
    unsigned long DistanceMeasured2 = pulseIn(URPWM2,LOW);
     
    if(DistanceMeasured2==50000){                     // the reading is invalid.
      Serial.print("Invalid");    
   }
    else{
      Distance2=DistanceMeasured2/50;                  // every 50us low level stands for 1cm
      
      if(steerRight == false){
        if(Distance2 <= 10){
          goRight();
          steerRight = true;
        }
      }
      else{
        if(Distance2 >= 10){
          forwardFromRight();
          steerRight = false;
        }  
      }
   }
}

void goLeft(){
  for(pos = 43; pos >= 5; pos -= 1) // goes from 110 degrees to 90 degrees
  {
    steering.write(pos); // tell servo to go to position in variable ‘pos’
  }
}

void forwardFromLeft(){
  if(pos != 43){
    for(pos = 5; pos < 43; pos += 1) // goes from 90 degrees to 110 degrees
    {
       steering.write(pos); // tell servo to go to position in variable ‘pos’
    }
  }
}

void goRight(){
  for(pos = 43; pos <= 83; pos += 1) // goes from 110 degrees to 90 degrees
  {
    steering.write(pos); // tell servo to go to position in variable ‘pos’
  }
}

void forwardFromRight(){
  if(pos != 43){
    for(pos = 83; pos > 43; pos -= 1) // goes from 90 degrees to 110 degrees
    {
       steering.write(pos); // tell servo to go to position in variable ‘pos’
    }
  }
}
