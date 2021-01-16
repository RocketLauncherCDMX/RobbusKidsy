/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accelerometer.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <RobbusKidsy.h>
#include <DabbleESP32.h>
#include <KidsyDabbleJoystick.h>

RobbusKidsy Kidsy;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // make sure your Serial Monitor is also set at this baud rate.
  Kidsy.begin(BIP);
  Dabble.begin("Robbus Kidsy");       //set bluetooth name of your device
}

bool new_forwardButton = false;
bool old_forwardButton = false;

void loop() {
  // this function is used to refresh data obtained from smartphone.
  // Hence calling this function is mandatory in order to get data properly from your mobile.
  Dabble.processInput();

  new_forwardButton = GamePad.isUpPressed();

  if(new_forwardButton == true && old_forwardButton == false) {
    Serial.print("Forward");
  }

  old_forwardButton = new_forwardButton;
  
  /*if (GamePad.isUpPressed())
  {
    Serial.print("Forward");
    Kidsy.Move.forward(255);
  }
  else Kidsy.Move.stop();

  if (GamePad.isDownPressed())
  {
    Serial.print("Backward");
    Kidsy.Move.backward(255);
  }
  else Kidsy.Move.stop();

  if (GamePad.isLeftPressed())
  {
    Serial.print("Left");
    Kidsy.Move.turnLeft(255);
  }
  else Kidsy.Move.stop();

  if (GamePad.isRightPressed())
  {
    Serial.print("Right");
    Kidsy.Move.turnRight(255);
  }
  else Kidsy.Move.stop();

  if (GamePad.isSquarePressed())
  {
    Serial.print("Square");
  }

  if (GamePad.isCirclePressed())
  {
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed())
  {
    Serial.print("Cross");
  }

  if (GamePad.isTrianglePressed())
  {
    Serial.print("Triangle");
  }

  if (GamePad.isStartPressed())
  {
    Serial.print("Start");
  }

  if (GamePad.isSelectPressed())
  {
    Serial.print("Select");
  }
  Serial.print('\t');

  int a = GamePad.getAngle();
  Serial.print("Angle: ");
  Serial.print(a);
  Serial.print('\t');
  int b = GamePad.getRadius();
  Serial.print("Radius: ");
  Serial.print(b);
  Serial.print('\t');
  float c = GamePad.getXaxisData();
  Serial.print("x_axis: ");
  Serial.print(c);
  Serial.print('\t');
  float d = GamePad.getYaxisData();
  Serial.print("y_axis: ");
  Serial.println(d);
  Serial.println();*/
}
