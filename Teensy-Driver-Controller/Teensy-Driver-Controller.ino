/*
*  MST Solar Car TEENSY-Driver-Controller main file
*  
*  Designed for use with the Teensy 3.6 Development board
*  for use with the Arduino IDE, with the Teensy board package installed
*
*  This file must not move from the Teensy-Driver-Controller folder otherwise the Arduino IDE will not
*  know what it is.
*
*/

//Includes
#include "FlexCAN.h"
#include <Metro.h>

//Serial Pins (digital)
int accelSwitch = 23;

//motorController ID
#define motorControllerBase 0x400u
#define powerCommand motorControllerBase + 0x02u
#define highPower 0x3c23d70a

//System timer
Metro sysTimer = Metro(1);  // milliseconds

//flexCan connection
FlexCAN CANbus = FlexCAN(500000);     // 500kb/s baudrate

//messages to be sent through the CANbus
static CAN_message_t powerMsg;    // sending message
static CAN_message_t rxmsg; // return message

//On board LED pin for DebugState
// solid: initialized and ready for activity
// slow blinking:
// rapid blinking: 
// 1-3 blinking : 
int led = 13;

//counters to keep the software in sync, transmission and receive
int txCount,rxCount;
unsigned int txTimer,rxTimer;

void setup() {
  CANbus.begin();   //initialize the CANbus

  delay(1000);      //give some time for the initilization process to finish
  Serial.println(F("Initilization Complete"));

  //PINS
  pinMode(led, OUTPUT); //turn on LED pin 
  digitalWrite(led, 1);

  pinMode(accelSwitch, INPUT);

  //msg setup for motor power command
  powerMsg.id = powerCommand;
  powerMsg.len = 8;
  powerMsg.timeout = 250;

  sysTimer.reset();
}

void motor_drive(uint8_t current, uint8_t velocity){


  return;
}

void loop() {
  // for every tick of sysTimer decreases the software timers
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
    if ( rxTimer ) {
      --rxTimer;
    }
  }

  //transmit message code if txTimer != 0
  if(!txTimer) {

    if(digitalRead(accelSwitch)) {
      //Send powerMsg to motor controller with high current
      Serial.println("MOVING");

      //inputing uint8_t values into the buffer
      powerMsg.buf[7] = (uint8_t) (highPower & 0x000000ff);
      powerMsg.buf[6] = (uint8_t) ((highPower >> 8) & 0x000000ff);
      powerMsg.buf[5] = (uint8_t) ((highPower >> 16) & 0x000000ff);
      powerMsg.buf[4] = (uint8_t) ((highPower >> 24) & 0x000000ff);
      powerMsg.buf[3] = 0;
      powerMsg.buf[2] = 0;
      powerMsg.buf[1] = 0;
      powerMsg.buf[0] = 0;
      CANbus.write(powerMsg);

    } else {
      //send powerMsg to motor controller with low current
      Serial.println("NotMoving");
      powerMsg.buf[0] = 0;
      powerMsg.buf[1] = 0;
      powerMsg.buf[2] = 0;
      powerMsg.buf[3] = 0;
      powerMsg.buf[4] = 0;
      powerMsg.buf[5] = 0;
      powerMsg.buf[6] = 0;
      powerMsg.buf[7] = 0;
      CANbus.write(powerMsg);
    }

    //set the txTimer to count down 10 ms
    txTimer = 10;
  }

  //return message code if rxTimer != 0
  if(!rxTimer) {



    
  }

  
}
