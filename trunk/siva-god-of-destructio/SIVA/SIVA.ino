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
#include <dht11.h>

#include <TimerOne.h>
#include <MsTimer2.h>
//#include <TimerThree.h>
dht11 DHT;
#define DHT11_PIN 13//4

//#define PYTHON_BLUETOOTH_PROTOCOL //raspberry-pi python bluetooth protocol

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

MePIRMotionSensor myPIRsensor(PORT_8);

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
    //Serial.print("ultra value = ");  
    //Serial.print("\t");      
    //Serial.println(ultra_val);
    
    machineState = ULTRA_DISTANCE;
    //turn_left();    
   }   
  
  if(ultra_val < ULTRA_DANGER)    //5cm
  {
    //Serial.print("ultra value = ");  
    //Serial.print("\t");      
    //Serial.println(ultra_val);
    
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
    //Serial.println("InfraredReceiverDecode and Bluetooth Start!");

    bluetooth.begin(9600);
 
 #if 0  //temp
    MsTimer2::set(100, ultraSensorCheck); // 500ms period   : 500, 1 sec 1000
    MsTimer2::start();
  
  // 0.5 sec->500000, 1 sec -> 1000000
    Timer1.initialize(300000);//500000         // initialize timer1, and set a 1/2 second period
    Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
    Timer1.attachInterrupt(remoconCheck);  // attaches callback() as a timer overflow interrupt
#endif

}

String go_machine()
{
	          motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);            
                  motor3.run(-motorSpeed);            
                  motor4.run(-motorSpeed);            
    	          delay(TIME_DELAY);  // 2000 -> 10
                  return "OK_GO";
}

String stop_machine()
{
                  motor1.run(0); 
    	          motor2.run(0); 
                  motor3.run(0);
                  motor4.run(0);
                  delay(TIME_DELAY);
                  return "OK_STOP";
}

String back_machine()
{
                  motor1.run(motorSpeed); // value: between -255 and 255.
    	          motor2.run(motorSpeed); // value: between -255 and 255.
                  motor3.run(motorSpeed);
                  motor4.run(motorSpeed);
    	          delay(TIME_DELAY);
                  return "OK_BACK";
}

String turn_right()
{
                 //Serial.println("turn right");
             	  motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);                               
                  motor4.run(-motorSpeed);      
                  
                  motor1.run(motorSpeed);            
        	  motor2.run(motorSpeed);    
                  motor3.run(motorSpeed);    
                  delay(1300);   //TIME_DELAY
                  return "OK_TURNRIGHT";
        
}

String turn_left()
{
                  //Serial.println("turn left");
                  motor1.run(-motorSpeed);            
        	  motor2.run(-motorSpeed);    
                  motor3.run(-motorSpeed);    
                  
                  motor1.run(motorSpeed);            
        	  motor2.run(motorSpeed);                               
                  motor4.run(motorSpeed);
                  delay(1400);       
                  return "OK_TURNLEFT";
}

void message_process(String msg)
{
    // Serial.println("@@@@@@@@@@@@@@@@@"); 
    //  Serial.println(msg);
    String rspMachine;

      if(msg == "go"){
       // Serial.println("GO");
        rspMachine = go_machine();        
      }
      else if(msg == "stop"){
   //     Serial.println("STOP");
        rspMachine = stop_machine();
      } 
      else if(msg == "back"){
    //    Serial.println("BACK");
        rspMachine = back_machine();
      }   
      else if(msg == "turnleft"){
   //     Serial.println("turn left");
        rspMachine = turn_left();
      }   
      else if(msg == "turnright"){
   //     Serial.println("turn right");
        rspMachine = turn_right();
      }   
      else if(msg == "turbo"){
          //Serial.println("turbo");
      }   
      else if(msg == "humi"){
         rspMachine = "";
      }          
      else {
          //Serial.println("msg ERROR!!!");
      }
      
      messge_assemble(rspMachine);        
}
/**************************************************************************
 *   msg_to_rpi
 * This function is devide a String. and the output is character.
 * It's send the Raspberry Pi with BlueTooth.
 * The Bluetooth moduls is only one byte transmitter, receiver.
 * so i write this function.
*************************************************************************/

//////////  ==> msg_to_rpi function is necessary debugging
#define ASCII 48  // decimal to ASCII
#define ONEB 1    // Header is 1 byte
#define TWOB 2    // Header is 2 byte

void msg_to_rpi(String msg)
{
  int temp = 0;
  int msgLength = msg.length();
  char sendChar;
    
  //Serial.print(msg);   //+ ASCII
  //Serial.print(msgLength);   //+ ASCII
  //manager the over 10 value 
  #ifdef PYTHON_BLUETOOTH_PROTOCOL 
  // for raspberry-pi python bluetooth protocol
    if(msgLength > 9){
      bluetooth.write(TWOB + ASCII);
      
      temp = msgLength / 10;
      bluetooth.write(temp + ASCII); 
      temp = msgLength % 10;
      bluetooth.write(temp + ASCII); 
  }
  else{
      bluetooth.write(ONEB + ASCII);
      bluetooth.write(msgLength + ASCII); 
  }
  #endif
  
  //Serial.print('\n');  
  for( int i=0; i < msgLength; i++)
  {
    sendChar = msg.charAt(i);
    
    bluetooth.write(sendChar);     
    //bluetooth.write(i + ASCII);    
     
    Serial.print(sendChar);  
    //Serial.print('\n');  

  }
  
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
     
     //PIR sensor module
    if(myPIRsensor.isPeopleDetected())
    {
	Serial.println("People Detected");
        delay(100);
     }

  
    if( EVENT_CHECK & motorHandler )
    {
      /*
         0011 1111  : 0 bit -> turn right, 1 bit -> turn left, 2 bit -> back, 3 bit ->  Go
                     4 bit -> ULTRA DANGER ON/OFF, 5 bit -> ULTRA DISTANCE ON/OFF
      */
    //  Serial.println("IF check");      
    //  Serial.println(motorHandler);  
      if( motorHandler & 0x01) //0 bit -> turn right,
      {
        //Serial.println("turn right");      
        turn_right();           
        motorHandler &= 0xFE;
     //   Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x02) //1 bit -> turn left,
      {
        //Serial.println("turn left");      
        turn_left();           
        motorHandler &= 0xFD;
      //  Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x10) //4 bit -> ULTRA DANGER ON->OFF
      {
        //Serial.println("ULTRA DANGER stop machine");
        //stop_machine();               
        go_machine();
        motorHandler &= 0xEF;
        machineState = STATE_CLEAR;
     //   Serial.println(motorHandler);     
      }
      
      if( motorHandler & 0x20) //5 bit -> ULTRA DISTANCE ON->OFF
      {
        //Serial.println("ULTRA DISTANCE stop machine");
        //stop_machine();               
        go_machine();
        motorHandler &= 0xDF;
        machineState = STATE_CLEAR;
    //    Serial.println(motorHandler);     
      }
    }
 
    int po1=0;
    int po2=0;
    static int cnt;
    char temp[20];
        
    if(bluetooth.available())
    {
        inDat = bluetooth.read();
        sCommand.concat(inDat);    
        po1 = sCommand.indexOf('+');
        po2 = sCommand.indexOf('\n');         

        if(po2 > 0)
        {
            cmdBuffer[cnt] = sCommand;
            sCommand = "";
            //Serial.println("============"); 
            //Serial.println(cmdBuffer[cnt]);   
           
            cmdBuffer[cnt].trim();
            if(cmdBuffer[cnt] == "+CONNECTED"){
                BTstatus = "CONNECT";
                //Serial.println("OK_CONNECT"); 
            }            
            cnt++;
        }
                
        if(BTstatus == "CONNECT"){
           //Serial.println(BTstatus);
           //Serial.println(sCommand);           
          if(sCommand.startsWith("SIVA")){
        
              mainMsg.concat(inDat); 
              if(mainMsg.endsWith("END")){
                //messge_assemble();//good
                //Serial.println(mainMsg);
                int index_last = mainMsg.lastIndexOf("END");
                //Serial.println(index_last);        
                mainMsg = mainMsg.substring(1, index_last);
                //Serial.println(mainMsg);               
                message_process(mainMsg);
                
                //'A'->'A', 'ABC'-> 'C', 48->0, 55->7, 56->8
                //Only 1byte send on BlueTooth
                //msg_to_rpi("msgProcess_OK");
                
                sCommand = "";
                mainMsg = "";
              }
              else{
           //     Serial.println("tail_Bad");
              }         
            }
            else{
         //     Serial.println("header_Bad");
            }
            
        }        
    
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
     //Serial.println("<<<===  Serial available ===>>>");
     inDat = Serial.read();    
     sCommand.concat(inDat);  
    
    if(sCommand.startsWith("SIVA")){

      mainMsg.concat(inDat); 
      if(mainMsg.endsWith("END")){
        //Serial.println("#############");
        //Serial.println(mainMsg);
        int index_last = mainMsg.lastIndexOf("END");
        //Serial.println(index_last);        
        mainMsg = mainMsg.substring(1, index_last);    
               
        //Serial.println(mainMsg);
        message_process(mainMsg);
        
        msg_to_rpi("KANG");//test code
        
        sCommand = "";
        mainMsg = "";
      }
      else{
        //Serial.println("tail_Bad");
      }         

    }
    else{
      //Serial.println("header_Bad");
    }
    
     //Serial.println(sCommand);
     delay(100);
   }
//<== String check test code.
  
    if(infraredReceiverDecode.available())
    {
      IRval = infraredReceiverDecode.read();      
       
      switch(IRval)
      {
        case GO://Serial.println("GO");
             go_machine();
          //   stop_machine(); 
              break;
             
        case BACK://Serial.println("BACK");
             back_machine();
           //  stop_machine(); 
             break;
             
         case TURN_RIGHT://Serial.println("turn right");
              turn_right();
          //    stop_machine(); 
              break;

         case TURN_LEFT://Serial.println("turn left");
              turn_left();
         //     stop_machine();              
              break;
     

        case STOP://Serial.println("STOP");
             stop_machine(); 
             break;
            
        default:
              //Serial.print("IR value = ");  
              //Serial.print("\t");      
              //Serial.println(IRval);
              break;
      }
      
      IRval = 0;
              
    }
  
    
}


//humidity sensor    
void messge_assemble(String rcvMsg)
{  
    //START
  int chk;
  String sendMsg = NULL;
  //Serial.println("read_temper");
  //Serial.print("\nGet sensor data\t");
  chk = DHT.read(DHT11_PIN);    // READ DATA

  switch (chk){
    case DHTLIB_OK:  
                //Serial.print("\nOK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                //Serial.print("\nChecksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                //Serial.print("\nTime out error,\t"); 
                break;
    default: 
                //Serial.print("\nUnknown error,\t"); 
                break;
  }
 // DISPLAT DATA
  //Serial.print(DHT.humidity,1);
  //Serial.print(",\t");
  //Serial.println(DHT.temperature,1);
  
  /*
     - humidity : "humi" %
     - temperature : "temp" C
  */
  sendMsg = NULL;

#ifdef PYTHON_BLUETOOTH_PROTOCOL  
  sendMsg = "humi" + String(DHT.humidity) + "%"+ "temp" + String(DHT.temperature) + "C" + rcvMsg;
#else
  sendMsg = "H" + String(DHT.humidity) + "%::"+ "T" + String(DHT.temperature) + "C";
#endif

  msg_to_rpi(sendMsg);  
  //delay(100); //1000
}


