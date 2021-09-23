#include <Arduino.h>
#include <fifo.h>
#include <digital_out.h>
#include <stdint.h>

Fifo buffer;                // The buffer class.
Digital_out LED(5);         // Class for output, used for the onboard LED.
uint32_t old_time = 0;      // For using millis
const int take_time = 1000; // time in ms.
int command = 0;            // variable for the serial input.

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  LED.init();
}

void loop()
{

  if (Serial.available())
  {
    command = Serial.read();

    Serial.print("I recieved: ");
    Serial.println(command, DEC);
    buffer.put(command);
  }
  if (buffer.is_full())
  {
    LED.set_hi();
    Serial.println("I'm full");
  }
  else
    LED.set_lo();

  if ((millis() - old_time) >= 1000)
  {
    if (!(buffer.is_empty()))
    {
      buffer.get();
      Serial.println("I take. Golumn!");
    }
    old_time = millis();
  }
}