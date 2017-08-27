/*

   Library doing all the Nunchuk lifting:
   Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/

   Nunchuk project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/


   Pin overview of the nunchuk attachment:
   + to 5v of the arduino
   - to GND of the arduino
   Data (d) to Analog pin 4 (A4)
   Clock (c) to Analog pin 5 (A5)
*/

#include <Wire.h>
#include <ArduinoNunchuk.h>

#define BAUDRATE 19200
#define FORWARDPIN 6 //PWM for vertical forward motor contror 
#define BACKPIN 5 //PWN for vertical backwards motor control
#define RIGHTPIN 2
#define LEFTPIN 3
#define BUZZERPIN 7

#define FWRDDEADZONE 140
#define BACKDEADZONE 110


byte acceleration;



ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
  pinMode(FORWARDPIN, OUTPUT);
  pinMode(BACKPIN, OUTPUT);
  pinMode(LEFTPIN, OUTPUT);
  pinMode(RIGHTPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

}

void loop()
{

  nunchuk.update();

  Serial.print(nunchuk.analogX, DEC);
  Serial.print(' ');
  Serial.println(nunchuk.analogY, DEC);

//  Serial.print(' ');
//  Serial.print(nunchuk.accelX, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.accelY, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.accelZ, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.zButton, DEC);
//  Serial.print(' ');
//  Serial.println(nunchuk.cButton, DEC);

  accelerationMotorControl();
  steeringMotorControl();

}

void accelerationMotorControl ()
{
  /* Y deadzone 110 - 140,
     if Y is between 14-240, the car goes forward at acceleration = PWM,
     if Y is between 110-0, the car goes backwards at acceleration = PWM,
  */

  if (nunchuk.analogY > FWRDDEADZONE)
  {
    acceleration = map(nunchuk.analogY, 140, 230, 0, 255);
    analogWrite(FORWARDPIN, acceleration);

  }
  else if (nunchuk.analogY < BACKDEADZONE)
  {
    acceleration = map(nunchuk.analogY, 110, 20, 0, 255);
    analogWrite(BACKPIN, acceleration);
  }
  else
  {
    analogWrite(FORWARDPIN, 0);
    analogWrite(BACKPIN, 0);
  }
}

void steeringMotorControl()
{
  /* X deadzone 80 - 170
     when X is lower than 80, the motor turns left.
     when X is higher than 170, the motor turns right.
  */

  switch (nunchuk.analogX)
  {
    case 0 ... 80:
      digitalWrite(LEFTPIN, HIGH);
      break;

    case 170 ... 255:
      digitalWrite(RIGHTPIN, HIGH);
      break;

    default:
      digitalWrite(LEFTPIN, LOW);
      digitalWrite(RIGHTPIN, LOW);
      break;
  }
  //  if (nunchuk.analogX < 80)
  //  {
  //    digitalWrite(LEFTPIN, HIGH);
  //  }
  //
  //  else if (nunchuk.analogX > 170)
  //  {
  //    digitalWrite(RIGHTPIN, HIGH);
  //  }
  //
  //  else
  //  {
  //    digitalWrite(LEFTPIN, LOW);
  //    digitalWrite(RIGHTPIN, LOW);
  //  }
}

