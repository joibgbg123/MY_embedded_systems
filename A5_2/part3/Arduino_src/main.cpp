#include <Arduino.h>

/* Arduino Serial Command Processor */

int ledPin = 11;          // LED with PWM brightness control
size_t MSG_LEN = 2;

void setup() {            // called once on start up
   // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
   Serial.begin(115200, SERIAL_8N1);
   pinMode(ledPin, OUTPUT);         // the LED is an output
}

void loop() {              // loops forever
   unsigned char command[MSG_LEN];
   char buffer[MSG_LEN];       // stores the return buffer on each loop   
   if (Serial.available()>0){                 // bytes received
      Serial.readBytes(command, MSG_LEN); // C strings end with \0
      buffer[0]=command[0];
      if(command[0] == 0x02){   // begins with "LED "?
         // String intString = command.substring(4, command.length());
         int level = command[1];       // extract the int
         if(level>=0 && level<=255){          // is it in range?
            analogWrite(ledPin, level);       // yes, write out
            buffer[1]=0xAA;
         }
         else{                                // no, error message back
            buffer[1]=0xF0;
         } 
      }                                       // otherwise, unknown cmd
      else{ buffer[1]=0xFF; }
      Serial.print(buffer);               // send the buffer to the RPi
   }
}