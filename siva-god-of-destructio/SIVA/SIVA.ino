/*************************************************************************
* File Name          : SIVA version 1.5 .ino
* Author             : wahihi
* Updated            : wahihi
* Version            : V1.0.1
* Date               : 3/27/2014
* Description        : SIVA receive IR and BlueTooth. and control motor.
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 Maker Works Technology Co., Ltd. All right reserved.
* https://code.google.com/p/siva-god-of-destruction/
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

/*
Blue module can only be connected to port 3, 4, 5, 6 of base shield.
*/
MeBluetooth bluetooth(PORT_4);

MeInfraredReceiver infraredReceiverDecode(PORT_6); 
MeUltrasonicSensor ultraSensor(PORT_7); //Ultrasonic module can ONLY be connected to port 3, 4, 5, 6, 7, 8 of base shield.

int IRval = 0;
long ultra_val = 0;
String sCommand = "";
String cmdBuffer[20];

String mainMsg, stringTwo;

String BTstatus;

MeDCMotor motor1(PORT_1);
MeDCMotor motor2(PORT_2);
MeDCMotor motor3(M1);
MeDCMotor motor4(M2);

uint8_t motorSpeed = 100; //1000: Fast Speed //100: Normal Speed
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
    Serial.println("InfraredReceiverDecode and Bluetooth Start!");
    
    bluetooth.begin(9600);
 
 #if 0   //temp
    MsTimer2::set(100, ultraSensorCheck); // 500ms period   : 500, 1 sec 1000
    MsTimer2::start();
  
  // 0.5 sec->500000, 1 sec -> 1000000
    Timer1.initialize(300000);//500000         // initialize timer1, and set a 1/2 second period
    Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
    Timer1.attachInterrupt(remoconCheck);  // attaches callback() as a timer overflow interrupt
#endif

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

void message_process(String msg)
{
     Serial.println("@@@@@@@@@@@@@@@@@"); 
      Serial.println(msg);

      if(msg == "go"){
        Serial.println("GO");
        go_machine();
      }
      else if(msg == "stop"){
        Serial.println("STOP");
        stop_machine();
      } 
      else if(msg == "back"){
        Serial.println("BACK");
        back_machine();
      }   
      else if(msg == "turnleft"){
        Serial.println("turn left");
        turn_left();
      }   
      else if(msg == "turnright"){
        Serial.println("turn right");
        turn_right();
      }   
      else if(msg == "turbo"){
          Serial.println("turbo");
      }   
      else {
          Serial.println("msg ERROR!!!");
      }
      
#if 0   
      switch(msg)
      {
        case "go":Serial.println("GO");
             go_machine();
              break;
             
        case "back":Serial.println("BACK");
             back_machine();
             break;
             
         case "turnright":Serial.println("turn right");
              turn_right();
              break;

         case "turnleft":Serial.println("turn left");
              turn_left();     
              break;
     

        case "stop":Serial.println("STOP");
             stop_machine(); 
             break;

        case "turbo":Serial.println("turbo");             
             break;
            
        default:
              Serial.print("BlueTooth value = ");  
              Serial.print("\t");                    
              stop_machine();
              break;
      }
#endif
     
}
void loop()
{
    char inDat;
    char outDat;
//    String sCommand = "";

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
  /*
    if(bluetooth.available())
    {
        inDat = bluetooth.read();
        sCommand.concat(inDat);
        //Serial.print(inDat);
        Serial.println(inDat);
       // Serial.println("Input Stop");       
       
        if( inDat == '+' )
        {
          Serial.println("GO");
          go_machine();
        }
      
        Serial.println(sCommand);           
    }
  */
    int po1=0;
    int po2=0;
    static int cnt;
    char temp[20];
    
    
    
    if(bluetooth.available())
    {
#if 1
        inDat = bluetooth.read();
        sCommand.concat(inDat);    
        po1 = sCommand.indexOf('+');
        po2 = sCommand.indexOf('\n');  
        
        if(po2 > 0)
        {
            cmdBuffer[cnt] = sCommand;
            sCommand = "";
            Serial.println("============"); 
            Serial.println(cmdBuffer[cnt]);   
            //cmdBuffer[cnt].toCharArray(temp, 11); 
            
           // Serial.println(temp);  
            cmdBuffer[cnt].trim();
            if(cmdBuffer[cnt] == "+CONNECTED"){
                BTstatus = "CONNECT";
                Serial.println("OK_START"); 
            }
            
            cnt++;
        }
        
        //sCommand.indexOf('n');  
        if(BTstatus == "CONNECT"){
           Serial.println(BTstatus);
           Serial.println(sCommand);
           
#if 1  //insert bluetooth char
          if(sCommand.startsWith("SIVA")){
        
              mainMsg.concat(inDat); 
              if(mainMsg.endsWith("END")){
                Serial.println("#############");
                Serial.println(mainMsg);
                int index_last = mainMsg.lastIndexOf("END");
                Serial.println(index_last);        
                mainMsg = mainMsg.substring(1, index_last);
                Serial.println(mainMsg);
                message_process(mainMsg);
                sCommand = "";
                mainMsg = "";
              }
              else{
                Serial.println("tail_Bad");
              }         
            }
            else{
              Serial.println("header_Bad");
            }
#endif  //insert bluetooth char
           
           
        }        
#endif    

#if 0
    Serial.println("<<<===  BlueTooth available ===>>>");        
    sCommand.concat(inDat);  
    
    if(sCommand.startsWith("SIVA")){

      mainMsg.concat(inDat); 
      if(mainMsg.endsWith("END")){
        Serial.println("#############");
        Serial.println(mainMsg);
        int index_last = mainMsg.lastIndexOf("END");
        Serial.println(index_last);        
        mainMsg = mainMsg.substring(1, index_last);
        Serial.println(mainMsg);
        message_process(mainMsg);
        sCommand = "";
        mainMsg = "";
      }
      else{
        Serial.println("tail_Bad");
      }         
    }
    else{
      Serial.println("header_Bad");
    }
#endif        
     //Serial.println(sCommand);
    // delay(100);
    }

//==> String check test code.
/************
* message : SIVAgoEND, SIVAstopEND, SIVAbackEND, SIVAturnleftEND, SIVAturnrightEND

@ start check method -> sCommand.startsWith("SIVA")
* message check sequence : head and tail check ( SIVA, END )
************/
/*
    sCommand = "SIVAgoEND";
    if(sCommand.startsWith("SIVA")){
      Serial.println("============");
      Serial.println("SIVA");
    }
    else{
      Serial.println("startBad");
    }

    if(sCommand.endsWith("END")){
      Serial.println("#############");
      Serial.println("END");
    }
    else{
      Serial.println("endBad");
    }    
*/    

/*******************************************************************************
 * Get out the message from serial communication.
 * The BlueTooth message is usart communication. so it's simmiliar a uart
*******************************************************************************/
   static int tag_check = 0;
   
   if(Serial.available())
   {
     Serial.println("<<<===  Serial available ===>>>");
     inDat = Serial.read();    
     sCommand.concat(inDat);  
    
    if(sCommand.startsWith("SIVA")){

      mainMsg.concat(inDat); 
      if(mainMsg.endsWith("END")){
        Serial.println("#############");
        Serial.println(mainMsg);
        int index_last = mainMsg.lastIndexOf("END");
        Serial.println(index_last);        
        mainMsg = mainMsg.substring(1, index_last);
        Serial.println(mainMsg);
        message_process(mainMsg);
        sCommand = "";
        mainMsg = "";
      }
      else{
        Serial.println("tail_Bad");
      }         

    }
    else{
      Serial.println("header_Bad");
    }
    
     Serial.println(sCommand);
     delay(100);
   }
//<== String check test code.
  
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





