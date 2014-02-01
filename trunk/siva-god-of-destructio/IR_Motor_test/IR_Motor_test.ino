/*************************************************************************
* File Name          : GoBackStop_Turn-RightLeft_ultraSensor.ino
* Author             : Evan
* Updated            : Evan
* Version            : V1.0.1
* Date               : 5/18/2013
* Description        : Example for Makeblock Electronic modules of Me - 
                       Infrared Receiver. 
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <Makeblock.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include <TimerOne.h>
#include <MsTimer2.h>
//#include <TimerThree.h>

//#define CONFIRM_KEY 68
#define STOP 68
#define GO 64
#define BACK 65
#define TURN_RIGHT 6
#define TURN_LEFT 7
#define TIME_DELAY 2000 //2000->10
#define ULTRA_DISTANCE 25
#define ULTRA_DANGER 10
#define EVENT_CHECK 0x30
#define STATE_CLEAR 0x0

MeInfraredReceiver infraredReceiverDecode(PORT_6); 
MeUltrasonicSensor ultraSensor(PORT_7); //Ultrasonic module can ONLY be connected to port 3, 4, 5, 6, 7, 8 of base shield.

int IRval = 0;
long ultra_val = 0;

MeDCMotor motor1(PORT_1);
MeDCMotor motor2(PORT_2);
MeDCMotor motor3(M1);
MeDCMotor motor4(M2);

uint8_t motorSpeed = 100;
uint8_t machineState = 0;
uint8_t motorHandler = 0;

//100 msec check
void ultraSensorCheck()
{

 ultra_val = ultraSensor.distanceCm();

  if(ultra_val < ULTRA_DISTANCE)  //25cm
  {
    Serial.print("ultra value = ");  
    Serial.print("\t");      
    Serial.println(ultra_val);
    
    machineState = ULTRA_DISTANCE;
    //turn_left();    
   }
   
  
  if(ultra_val < ULTRA_DANGER)    //5cm
  {
    Serial.print("ultra value = ");  
    Serial.print("\t");      
    Serial.println(ultra_val);
    
    machineState = ULTRA_DANGER;   
    //back_machine();
   }
   

}

//300 msec check
void remoconCheck()
{
 //  Serial.println("remoconCheck");      
    if(infraredReceiverDecode.available())
    {
      IRval = infraredReceiverDecode.read();
    }
}
void setup()
{
    infraredReceiverDecode.begin();
    Serial.begin(9600);
    Serial.println("InfraredReceiverDecode Start!");
    
    MsTimer2::set(100, ultraSensorCheck); // 500ms period   : 500, 1 sec 1000
    MsTimer2::start();
  
  // 0.5 sec->500000, 1 sec -> 1000000
    Timer1.initialize(300000);//500000         // initialize timer1, and set a 1/2 second period
    Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
    Timer1.attachInterrupt(remoconCheck);  // attaches callback() as a timer overflow interrupt

}

void go_machine()
{
	          motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);            
                  motor3.run(-motorSpeed);            
                  motor4.run(-motorSpeed);            
    	          delay(TIME_DELAY);  // 2000 -> 10
}

void stop_machine()
{
                  motor1.run(0); 
    	          motor2.run(0); 
                  motor3.run(0);
                  motor4.run(0);
                  delay(TIME_DELAY);
}

void back_machine()
{
                  motor1.run(motorSpeed); // value: between -255 and 255.
    	          motor2.run(motorSpeed); // value: between -255 and 255.
                  motor3.run(motorSpeed);
                  motor4.run(motorSpeed);
    	          delay(TIME_DELAY);
}

void turn_right()
{
                 Serial.println("turn right");
             	  motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);                               
                  motor4.run(-motorSpeed);      
                  
                  motor1.run(motorSpeed);            
        	  motor2.run(motorSpeed);    
                  motor3.run(motorSpeed);    
                  delay(1300);   //TIME_DELAY
        
}

void turn_left()
{
                  Serial.println("turn left");
                  motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);    
                  motor3.run(-motorSpeed);    
                  
                  motor1.run(motorSpeed);            
        	  motor2.run(motorSpeed);                               
                  motor4.run(motorSpeed);
                  delay(1400);       
}
void loop()
{
     if(machineState > 0)
     {
       if(machineState == ULTRA_DISTANCE)
       {
         motorHandler = 0x22; //turn left
       }
       else if(machineState == ULTRA_DANGER)
       {
         motorHandler = 0x11; //turn right
       }
     
     } 
  
    if( EVENT_CHECK & motorHandler )
    {
      /*
         0011 1111  : 0 bit -> turn right, 1 bit -> turn left, 2 bit -> back, 3 bit ->  Go
                     4 bit -> ULTRA DANGER ON/OFF, 5 bit -> ULTRA DISTANCE ON/OFF
      */
      Serial.println("IF check");      
      Serial.println(motorHandler);  
      if( motorHandler & 0x01) //0 bit -> turn right,
      {
        Serial.println("turn right");      
        turn_right();           
        motorHandler &= 0xFE;
        Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x02) //1 bit -> turn left,
      {
        Serial.println("turn left");      
        turn_left();           
        motorHandler &= 0xFD;
        Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x10) //4 bit -> ULTRA DANGER ON->OFF
      {
        Serial.println("ULTRA DANGER stop machine");
        //stop_machine();               
        go_machine();
        motorHandler &= 0xEF;
        machineState = STATE_CLEAR;
        Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x20) //5 bit -> ULTRA DISTANCE ON->OFF
      {
        Serial.println("ULTRA DISTANCE stop machine");
        //stop_machine();               
        go_machine();
        motorHandler &= 0xDF;
        machineState = STATE_CLEAR;
        Serial.println(motorHandler);     
      }
    }
  
  
    if(infraredReceiverDecode.available())
    {
      IRval = infraredReceiverDecode.read();
     
      switch(IRval)
      {
        case GO:Serial.println("GO");
             go_machine();
          //   stop_machine(); 
              break;
             
        case BACK:Serial.println("BACK");
             back_machine();
           //  stop_machine(); 
             break;
             
         case TURN_RIGHT:Serial.println("turn right");
              turn_right();
          //    stop_machine(); 
              break;

         case TURN_LEFT:Serial.println("turn left");
              turn_left();
         //     stop_machine();              
              break;
     

        case STOP:Serial.println("STOP");
             stop_machine(); 
             break;
            
        default:
              Serial.print("IR value = ");  
              Serial.print("\t");      
              Serial.println(IRval);
              break;
      }
      
      IRval = 0;
              
    }
}


