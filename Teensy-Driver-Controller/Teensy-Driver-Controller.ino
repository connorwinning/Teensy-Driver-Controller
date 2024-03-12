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
int breaks = 33;
int accelerator = 22;

bool isBreaking = true;
bool isAccelerating = false;


//System timer
Metro sysTimer = Metro(1);  // milliseconds

//flexCan connection
FlexCAN CANbus = FlexCAN(500000);     // 500kb/s baudrate

//messages to be sent through the CANbus
static CAN_message_t msg;    // sending message
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

  pinMode(breaks, INPUT);
  pinMode(accelerator, INPUT);



  sysTimer.reset();
}

//DEBUG AND TESTING//
float acceleration = 0;

void loop() {
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
    if ( rxTimer ) {
      --rxTimer;
    }
  }

  //update Acceleration and Breaking status
  isAccelerating = digitalRead(accelerator);
  isBreaking = digitalRead(breaks);

  if(isAccelerating){
    acceleration = 100;
  } else {
    acceleration = 0;
  }
  if(isBreaking){
    acceleration = 0;
  }

  Serial.print("ACCELERATION: ");
  Serial.println(acceleration);
}
