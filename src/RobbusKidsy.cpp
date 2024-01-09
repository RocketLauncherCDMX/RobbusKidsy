#include <Arduino.h>
#include <RobbusKidsy.h>

#ifndef _ROBBUSKIDSY_CPP_
#define _ROBBUSKIDSY_CPP_

rmt_data_t led_data[24];
rmt_obj_t* rmt_send = NULL;

VEML6040 RGBWSensor;

Preferences FlashMemory;

void RobbusKidsy :: begin() {
  // ---------------------------------------------------------------
  // IOs
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LEDW, OUTPUT);
  
  pinMode(DCM_LEFT_IN1, OUTPUT);
  pinMode(DCM_LEFT_IN2, OUTPUT);
  pinMode(DCM_RIGHT_IN1, OUTPUT);
  pinMode(DCM_RIGHT_IN2, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  // ----------------------------------------------------------------
  // MOTORS
  pinMode(DCM_SLEEP, OUTPUT);

  ledcSetup(PWM_CHANNEL_LEFT_IN1, PWM_MOTOR_FREQ, PWM_RESOUTION);
  ledcAttachPin(DCM_LEFT_IN1, PWM_CHANNEL_LEFT_IN1);
  ledcSetup(PWM_CHANNEL_LEFT_IN2, PWM_MOTOR_FREQ, PWM_RESOUTION);
  ledcAttachPin(DCM_LEFT_IN2, PWM_CHANNEL_LEFT_IN2);

  ledcSetup(PWM_CHANNEL_RIGHT_IN1, PWM_MOTOR_FREQ, PWM_RESOUTION);
  ledcAttachPin(DCM_RIGHT_IN1, PWM_CHANNEL_RIGHT_IN1);
  ledcSetup(PWM_CHANNEL_RIGHT_IN2, PWM_MOTOR_FREQ, PWM_RESOUTION);
  ledcAttachPin(DCM_RIGHT_IN2, PWM_CHANNEL_RIGHT_IN2);

  ledcSetup(PWM_CHANNEL_BUZZER, 0, PWM_RESOUTION);
  ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
  ledcWrite(BUZZER, 128);

  Move.enableMotors();
  // ----------------------------------------------------------------
  // BUTTONS
  ButtonA.pin = BUTTON_A;
  ButtonB.pin = BUTTON_B;
  ButtonC.pin = BUTTON_C;
  // ----------------------------------------------------------------
  // LEDS
  Led1.pin = LED1;
  Led2.pin = LED2;
  Led3.pin = LED3;
  Led4.pin = LED4;
  // ----------------------------------------------------------------
  // ARROW INITIALIZATION
  ArrowForward.pin = AN_UP;
  ArrowBackward.pin = AN_DOWN;
  ArrowLeft.pin = AN_LEFT;
  ArrowRight.pin = AN_RIGHT;

  for(int i=0; i<ARROW_BUFFER_SIZE; i++) {
    ArrowForward.analogRead();
    ArrowBackward.analogRead();
    ArrowLeft.analogRead();
    ArrowRight.analogRead();
  }
  // -----------------------------------------------------------------------
  // FLASH MEMORY READING
  FlashMemory.begin("kidsy_memory", false);
  ArrowForward.touchedCalibrate = FlashMemory.getUInt("t_forward", 0);
  ArrowBackward.touchedCalibrate = FlashMemory.getUInt("t_backward", 0);
  ArrowLeft.touchedCalibrate = FlashMemory.getUInt("t_left", 0);
  ArrowRight.touchedCalibrate = FlashMemory.getUInt("t_right", 0);
  ArrowForward.untouchedCalibrate = FlashMemory.getUInt("unt_forward", 0);
  ArrowBackward.untouchedCalibrate = FlashMemory.getUInt("unt_backward", 0);
  ArrowLeft.untouchedCalibrate = FlashMemory.getUInt("unt_left", 0);
  ArrowRight.untouchedCalibrate = FlashMemory.getUInt("unt_right", 0);

  ColorSensor.red_hue = FlashMemory.getUInt("red_hue", 0);
  ColorSensor.green_hue = FlashMemory.getUInt("green_hue", 0);
  ColorSensor.blue_hue = FlashMemory.getUInt("blue_hue", 0);
  ColorSensor.yellow_hue = FlashMemory.getUInt("yellow_hue", 0);
  ColorSensor.cyan_hue = FlashMemory.getUInt("cyan_hue", 0);
  ColorSensor.magenta_hue = FlashMemory.getUInt("magenta_hue", 0);
  ColorSensor.white_umbral = FlashMemory.getUInt("white_umbral", 0);
  ColorSensor.black_umbral = FlashMemory.getUInt("black_umbral", 0);

  Move.top_leftSpeed = FlashMemory.getUInt("left_maxSpeed", 0);
  Move.top_rightSpeed = FlashMemory.getUInt("right_maxSpeed", 0);
  FlashMemory.end();
  // ---------------------------------------------------------------------------------------------------------------------------------------
  // ARROW CALIBRATION
  ArrowForward.thresshold   =  ((ArrowForward.untouchedCalibrate - ArrowForward.touchedCalibrate) / 2) + ArrowForward.touchedCalibrate;
  ArrowBackward.thresshold  =  ((ArrowBackward.untouchedCalibrate - ArrowBackward.touchedCalibrate) / 2) + ArrowBackward.touchedCalibrate;
  ArrowLeft.thresshold      =  ((ArrowLeft.untouchedCalibrate - ArrowLeft.touchedCalibrate) / 2) + ArrowLeft.touchedCalibrate;
  ArrowRight.thresshold     =  ((ArrowRight.untouchedCalibrate - ArrowRight.touchedCalibrate) / 2) + ArrowRight.touchedCalibrate;
  // ---------------------------------------------------------------------------------------------------------------------------------------
  // COLOR SENSOR INITIALIZATION

  Wire.begin(); 
  if(!RGBWSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
  }
  RGBWSensor.setConfiguration(VEML6040_IT_40MS + VEML6040_TRIG_ENABLE + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);

  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    ColorSensor.gammatable[i] = x;
  }


  // NEOPIXEL INITIALIZATION
  rmt_send = rmtInit(19, true, RMT_MEM_64);
  rmtSetTick(rmt_send, 100);

  Neopixel.off();
  delay(1);

  // ---------------------------------------------------------------------------
  // CALIBRATION
  calibrateSensor();
  calibrateArrows();
  calibrateMotors();
}

void RobbusKidsy :: calibrateSensor() {
  if(ButtonA.read() == HOLD_PRESSED) {
    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    ColorSensor.enable();
    Neopixel.heartBeat(RED, 50);

    Serial.println("Calibracion del sensor de color");
    Serial.println("Posiciona a Kidsy sobre el color rojo y presiona A");
    Neopixel.color(RED, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.red_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color verde y presiona A");
    Neopixel.color(GREEN, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.green_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color azul y presiona A");
    Neopixel.color(BLUE, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.blue_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color amarillo y presiona A");
    Neopixel.color(YELLOW, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.yellow_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color cyan y presiona A");
    Neopixel.color(CYAN, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.cyan_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color magenta y presiona A");
    Neopixel.color(MAGENTA, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.magenta_hue = ColorSensor.hue360;

    Serial.println("Posiciona a Kidsy sobre el color negro y presiona A");
    Neopixel.color(BLACK);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.black_umbral = ColorSensor.white * 1.2;

    Serial.println("Posiciona a Kidsy sobre el color white y presiona A");
    Neopixel.color(WHITE, 50);
    do ColorSensor.read();
    while(ButtonA.read() != PRESSED);
    Buzzer.playTone(200, 20);
    ColorSensor.white_umbral = ColorSensor.white * 0.8;

    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(GREEN, 50);

    Serial.println("Calibracion completa!, guardando en memoria...");

    FlashMemory.begin("kidsy_memory", false);
    FlashMemory.putUInt("red_hue", ColorSensor.red_hue);
    FlashMemory.putUInt("green_hue", ColorSensor.green_hue);
    FlashMemory.putUInt("blue_hue", ColorSensor.blue_hue);
    FlashMemory.putUInt("yellow_hue", ColorSensor.yellow_hue);
    FlashMemory.putUInt("cyan_hue", ColorSensor.cyan_hue);
    FlashMemory.putUInt("magenta_hue", ColorSensor.magenta_hue);
    FlashMemory.putUInt("white_umbral", ColorSensor.white_umbral);
    FlashMemory.putUInt("black_umbral", ColorSensor.black_umbral);
    FlashMemory.end();

    while(ButtonA.read() != PRESSED) {
      ColorSensor.read();
      Serial.println(ColorSensor.name);
      Neopixel.color(ColorSensor.value, 50);
    }
    ColorSensor.disable();
    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(GREEN, 50);

    ESP.restart();
  }
}

void RobbusKidsy :: calibrateArrows() {
  if(ButtonB.read() == HOLD_PRESSED) {
    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(GREEN, 50);
    Serial.println("Calibracion de las flechas tactiles");
    Serial.println("Sin presionar ninguna flecha, presiona el boton B");
    Led1.on();
    Led2.on();
    Led3.on();
    Led4.on();
    while(ButtonB.read() != PRESSED);
    Buzzer.playTone(200, 20);
    Led1.off();
    Led2.off();
    Led3.off();
    Led4.off();
    ArrowForward.analogRead();
    ArrowBackward.analogRead();
    ArrowLeft.analogRead();
    ArrowRight.analogRead();
    Serial.println("Lecturas de las flechas sin tocar:");
    Serial.println("Forward: " + String(ArrowForward.analogRead()));
    Serial.println("Backward: " + String(ArrowBackward.analogRead()));
    Serial.println("Left: " + String(ArrowLeft.analogRead()));
    Serial.println("Right: " + String(ArrowRight.analogRead()));
    Serial.println();

    FlashMemory.begin("kidsy_memory", false);
    FlashMemory.putUInt("unt_forward", ArrowForward.analog);
    FlashMemory.putUInt("unt_backward", ArrowBackward.analog);
    FlashMemory.putUInt("unt_left", ArrowLeft.analog);
    FlashMemory.putUInt("unt_right", ArrowRight.analog);
    FlashMemory.end();

    Neopixel.heartBeat(GREEN, 50);
    Serial.println("Mientras presionas la flecha Forward, presiona el boton B");
    Led1.on();
    do ArrowForward.analogRead();
    while(ButtonB.read() != PRESSED);
    Buzzer.playTone(200, 20);
    Serial.println("Lectura: " + String(ArrowForward.analogRead()));
    Neopixel.heartBeat(MAGENTA, 50);
    Serial.println("Mientras presionas la flecha Backward, presiona el boton B");
    Led1.off();
    Led2.on();
    do ArrowBackward.analogRead();
    while(ButtonB.read() != PRESSED);
    Buzzer.playTone(200, 20);
    Serial.println("Lectura: " + String(ArrowBackward.analogRead()));
    Neopixel.heartBeat(YELLOW, 50);
    Serial.println("Mientras presionas la flecha Left, presiona el boton B");
    Led2.off();
    Led3.on();
    do ArrowLeft.analogRead();
    while(ButtonB.read() != PRESSED);
    Buzzer.playTone(200, 20);
    Serial.println("Lectura: " + String(ArrowLeft.analogRead()));
    Neopixel.heartBeat(CYAN, 50);
    Serial.println("Mientras presionas la flecha Right, presiona el boton B");
    Led3.off();
    Led4.on();
    do ArrowRight.analogRead();
    while(ButtonB.read() != PRESSED);
    Buzzer.playTone(200, 20);
    Serial.println("Lectura: " + String(ArrowRight.analogRead()));
    Serial.println();
    Neopixel.heartBeat(BLUE, 50);
    Led1.off();
    Led2.off();
    Led3.off();
    Led4.off();

    FlashMemory.begin("kidsy_memory", false);
    FlashMemory.putUInt("t_forward", ArrowForward.analog);
    FlashMemory.putUInt("t_backward", ArrowBackward.analog);
    FlashMemory.putUInt("t_left", ArrowLeft.analog);
    FlashMemory.putUInt("t_right", ArrowRight.analog);
    FlashMemory.end();

    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(GREEN, 50);
    Serial.println("Calibracion completa!, guardando en memoria...");
    delay(1000);
    ESP.restart();
  }
}

void RobbusKidsy :: calibrateMotors() {
  if(ButtonC.read() == HOLD_PRESSED) {
    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(BLUE, 50);
    Serial.println();
    Serial.println("Calibracion de la maxima velocidad de los motores.");
    Serial.println("Los motores giraran hacia el frente. Si notas que Kidsy se va de lado");
    Serial.println("presiona la flecha hacia el lado que quieres corregir, hasta que notes que");
    Serial.println("se va recto. Si quieres aumentar o disminuir la velocidad maxima, presiona");
    Serial.println("las flechas Forward o Backward respectivamente.");
    Serial.println("Presiona C para terminar la calibracion");
    Move.forward(255);
    do {
      if(ArrowLeft.read() == TOUCHED) {
        if(Move.top_leftSpeed > 100 || Move.top_leftSpeed == 0) {
          Move.top_leftSpeed--;
          Serial.println("TopLeftSpeed: " + String(Move.top_leftSpeed));
          Led1.blink(1, 10);
          Buzzer.playTone(200, 20);
          Move.forward(255);
          timeStamp = millis();
          repeatCounter = 0;
        } else {
          Serial.println("Velocidad Izquierda al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      else if(ArrowLeft.status == HOLD_TOUCHED && millis() > timeStamp + 500) {
        repeatCounter++;
        if(Move.top_leftSpeed > 100 || Move.top_leftSpeed == 0) {
          Move.top_leftSpeed--;
          Serial.println("TopLeftSpeed: " + String(Move.top_leftSpeed));
          Led1.blink(1, 10);
          Buzzer.playTone(200, 20);
          Move.forward(255);
          if(repeatCounter < 10) delay(75);
          else delay(10);
        } else {
          Serial.println("Velocidad Izquierda al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      if(ArrowRight.read() == TOUCHED) {
        if(Move.top_rightSpeed > 100 || Move.top_rightSpeed == 0) {
          Move.top_rightSpeed--;
          Serial.println("TopRightSpeed: " + String(Move.top_rightSpeed));
          Led4.blink(1, 10);
          Buzzer.playTone(250, 20);
          Move.forward(255);
          timeStamp = millis();
          repeatCounter = 0;
        } else {
          Serial.println("Velocidad Derecha al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      else if(ArrowRight.status == HOLD_TOUCHED && millis() > timeStamp + 500) {
        repeatCounter++;
        if(Move.top_rightSpeed > 100 || Move.top_rightSpeed == 0) {
          Move.top_rightSpeed--;
          Serial.println("TopRightSpeed: " + String(Move.top_rightSpeed));
          Led4.blink(1, 10);
          Buzzer.playTone(250, 20);
          Move.forward(255);
          if(repeatCounter < 10) delay(75);
          else delay(10);
        } else {
          Serial.println("Velocidad Derecha al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      if(ArrowForward.read() == TOUCHED) {
        if(Move.top_leftSpeed < 255 || Move.top_rightSpeed < 255) {
          if(Move.top_leftSpeed < 255) Move.top_leftSpeed++;
          if(Move.top_rightSpeed < 255) Move.top_rightSpeed++;
          Serial.println("TopLeftSpeed: " + String(Move.top_rightSpeed));
          Serial.println("TopRightSpeed: " + String(Move.top_leftSpeed));
          Led2.blink(1, 10);
          Buzzer.playTone(300, 20);
          Move.forward(255);
          timeStamp = millis();
          repeatCounter = 0;
        } else {
          Serial.println("Velocidad de al menos un motor al maximo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      else if(ArrowForward.status == HOLD_TOUCHED && millis() > timeStamp + 500) {
        repeatCounter++;
        if(Move.top_leftSpeed < 255 || Move.top_rightSpeed < 255) {
          if(Move.top_leftSpeed < 255) Move.top_leftSpeed++;
          if(Move.top_rightSpeed < 255) Move.top_rightSpeed++;
          Serial.println("TopLeftSpeed: " + String(Move.top_rightSpeed));
          Serial.println("TopRightSpeed: " + String(Move.top_leftSpeed));
          Led2.blink(1, 10);
          Buzzer.playTone(200, 20);
          Move.forward(255);
          if(repeatCounter < 10) delay(75);
          else delay(10);
        } else {
          Serial.println("Velocidad de ambos motores al maximo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      if(ArrowBackward.read() == TOUCHED) {
        if(Move.top_leftSpeed > 100 && Move.top_rightSpeed > 100) {
          Move.top_leftSpeed--;
          Move.top_rightSpeed--;
          Serial.println("TopLeftSpeed: " + String(Move.top_rightSpeed));
          Serial.println("TopRightSpeed: " + String(Move.top_leftSpeed));
          Led3.blink(1, 10);
          Buzzer.playTone(350, 20);
          Move.forward(255);
          timeStamp = millis();
          repeatCounter = 0;
        } else {
          Serial.println("Velocidad de al menos un motor al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
      else if(ArrowBackward.status == HOLD_TOUCHED && millis() > timeStamp + 500) {
        repeatCounter++;
        if(Move.top_leftSpeed > 100 && Move.top_rightSpeed > 100) {
          Move.top_leftSpeed--;
          Move.top_rightSpeed--;
          Serial.println("TopLeftSpeed: " + String(Move.top_rightSpeed));
          Serial.println("TopRightSpeed: " + String(Move.top_leftSpeed));
          Led2.blink(1, 10);
          Buzzer.playTone(200, 20);
          Move.forward(255);
          if(repeatCounter < 10) delay(75);
          else delay(10);
        } else {
          Serial.println("Velocidad de al menos un motor al minimo");
          Buzzer.playTone(500, 200);
          Neopixel.heartBeat(RED, 50);
        }
      }
    }
    while(ButtonC.read() != PRESSED);
    Move.stop();
    Buzzer.playTone(2000, 50);
    delay(100);
    Buzzer.playTone(2000, 50);
    Neopixel.heartBeat(GREEN, 50);
    Serial.println("Calibracion completa!, guardando en memoria...");

    FlashMemory.begin("kidsy_memory", false);
    FlashMemory.putUInt("left_maxSpeed", Move.top_leftSpeed);
    FlashMemory.putUInt("right_maxSpeed", Move.top_rightSpeed);
    FlashMemory.end();

    delay(1000);
    ESP.restart();
  }
}

uint8_t RobbusKidsy :: getArrowPad() {
  if(ArrowForward.read() == TOUCHED) return(FORWARD);
  else if(ArrowBackward.read() == TOUCHED) return(BACKWARD);
  else if(ArrowLeft.read() == TOUCHED) return(LEFT);
  else if(ArrowRight.read() == TOUCHED) return(RIGHT);
  else return(NONE);
}

uint8_t RobbusKidsy :: buttons :: read() {
  new_state = digitalRead(pin);
  if(new_state == HIGH && old_state == HIGH) status = HOLD_PRESSED;    // button remains released
  if(new_state == HIGH && old_state == LOW) status = PRESSED;          // button is pressed once
  if(new_state == LOW && old_state == HIGH) status = RELEASED;         // button is released once
  if(new_state == LOW && old_state == LOW) status = HOLD_RELEASED;     // button remains pressed
  old_state = new_state;                                               // the last state gets old
  if(status == RELEASED || status == PRESSED) {
    delay(50);
  }
  return(status);
}

void RobbusKidsy :: Led :: set(bool new_status) {
  status = new_status;
  digitalWrite(pin, status);
}

void RobbusKidsy :: Led :: on() {
  status = ON;
  digitalWrite(pin, status);
}

void RobbusKidsy :: Led :: off() {
  status = OFF;
  digitalWrite(pin, status);
}

bool RobbusKidsy :: Led :: toggle() {
  status = !status;
  digitalWrite(pin, status);
  return(!status);
}

void RobbusKidsy :: Led :: blink(uint8_t times, uint16_t time) {
  for(int i=0; i<times; i++) {
    digitalWrite(pin, ON);
    delay(time);
    digitalWrite(pin, OFF);
    delay(time);
  }
}

void RobbusKidsy :: movement :: enableMotors() {
  digitalWrite(DCM_SLEEP, HIGH);
}

void RobbusKidsy :: movement :: disableMotors() {
  digitalWrite(DCM_SLEEP, LOW);
}

void RobbusKidsy :: movement :: MotorLeft(int16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < -255) vel = -255;
  if(vel > 0) direction = FORWARD;
  else if(vel < 0) direction = BACKWARD;
  else direction = STOP;
  speed = abs(vel);
  if(direction == FORWARD) {
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
  }
  else if(direction == BACKWARD){
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
  }
  else stop();
}

void RobbusKidsy :: movement :: MotorRight(int16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < -255) vel = -255;
  if(vel > 0) direction = FORWARD;
  else if(vel < 0) direction = BACKWARD;
  else direction = STOP;
  speed = abs(vel);
  if(direction == FORWARD) {
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  }
  else if(direction == BACKWARD){
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
  }
  else stop();
}

void RobbusKidsy :: movement :: forward(uint16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  if(speed > 0) {
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  }
  else stop();
}

void RobbusKidsy :: movement :: backward(uint16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  if(speed > 0) {
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
  }
  else stop();
}

void RobbusKidsy :: movement :: turnLeft(uint16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  if(speed > 0) {
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
  }
  else stop();
}

void RobbusKidsy :: movement :: turnRight(uint16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  if(speed > 0) {
    adjusted_leftSpeed = map(speed, 0, 255, 50, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
    adjusted_rightSpeed = map(speed, 0, 255, 50, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
  }
  else stop();
}

void RobbusKidsy :: movement :: stop() {
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
}

uint8_t RobbusKidsy :: Arrows :: analogRead() {
  total = 0;
  for(int i=0; i<ARROW_BUFFER_SIZE; i++) {
    buffer[readIndex] = touchRead(pin);
    total = total + buffer[readIndex];
  }  
  average = total / ARROW_BUFFER_LENGHT;
  analog = average;
  return(average);
}

uint8_t RobbusKidsy :: Arrows :: read() {
  analogRead();
  if(analog < thresshold) new_state = HIGH;
  else new_state = LOW;
  if(new_state == LOW && old_state == LOW) status = HOLD_NOTOUCHED;
  if(new_state == LOW && old_state == HIGH) status = NOTOUCHED;
  if(new_state == HIGH && old_state == LOW) status = TOUCHED;
  if(new_state == HIGH && old_state == HIGH) status = HOLD_TOUCHED;
  old_state = new_state;
  return(status);
}

void RobbusKidsy :: Buzzer :: playTone(int16_t frequency, uint16_t duration) {
  ledcSetup(PWM_CHANNEL_BUZZER, frequency, PWM_RESOUTION);
  ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
  ledcWrite(PWM_CHANNEL_BUZZER, 128);
  delay(duration);
  ledcWrite(PWM_CHANNEL_BUZZER, 0);
}

void RobbusKidsy :: Buzzer :: playTone(int16_t frequency) {
  if(old_frequency != frequency) {
    ledcSetup(PWM_CHANNEL_BUZZER, frequency, PWM_RESOUTION);
    ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
    ledcWrite(PWM_CHANNEL_BUZZER, 128);
  }
  old_frequency = frequency;
}


void RobbusKidsy :: Buzzer :: noTone() {
  ledcSetup(PWM_CHANNEL_BUZZER, 0, PWM_RESOUTION);
  ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
  ledcWrite(PWM_CHANNEL_BUZZER, 0);
  old_frequency = 0;
}

void RobbusKidsy :: Buzzer :: playNote(char note, uint16_t duration) {
  
}

void RobbusKidsy :: Buzzer :: r2d2(uint16_t time) {
  for(int i=0; i<time/75; i++) {
    playTone(random(50, 5000), 75);
  }
  noTone();
}

void RobbusKidsy :: Neopixel :: color(uint8_t red, uint8_t green, uint8_t blue) {
  value[0] = green;
  value[1] = red;
  value[2] = blue;
  i=0;

  for (col=0; col<3; col++ ) {
    for (bit=0; bit<8; bit++){
      if ( (value[col] & (1<<(7-bit)))) {
        led_data[i].level0 = 1;
        led_data[i].duration0 = 8;
        led_data[i].level1 = 0;
        led_data[i].duration1 = 4;
      } else {
        led_data[i].level0 = 1;
        led_data[i].duration0 = 4;
        led_data[i].level1 = 0;
        led_data[i].duration1 = 8;
      }
      i++;
    }
  }
  // Send the data
  rmtWrite(rmt_send, led_data, 24);
}

void RobbusKidsy :: Neopixel :: color(uint8_t colorName) {
  switch(colorName) {
    case OFF:       color(0,0,0);        break;
    case RED:       color(255,0,0);      break;
    case GREEN:     color(0,255,0);      break;
    case BLUE:      color(0,0,255);      break;
    case YELLOW:    color(255,255,0);    break;
    case CYAN:      color(0,255,255);    break;
    case MAGENTA:   color(255,0,255);    break;
    case WHITE:     color(255,255,255);  break;
  }
}

void RobbusKidsy :: Neopixel :: color(uint8_t colorName, uint8_t brightness) {
  if(brightness < 1) brightness = 1;
  else if(brightness > 255) brightness = 255;
  switch(colorName) {
    case BLACK:   off();                                     break;
    case RED:     color(brightness, 0, 0);                   break;
    case GREEN:   color(0, brightness, 0);                   break;
    case BLUE:    color(0, 0, brightness);                   break;
    case YELLOW:  color(brightness, brightness, 0);          break;
    case CYAN:    color(0, brightness, brightness);          break;
    case MAGENTA: color(brightness, 0, brightness);          break;
    case WHITE:   color(brightness, brightness, brightness); break;
  }
}

void RobbusKidsy :: Neopixel :: fadeInOut(uint8_t colorName, uint16_t speed) {
  for(int i=0; i<=255; i++) {
    switch(colorName) {
      case RED:     color(i,0,0); break;
      case GREEN:   color(0,i,0); break;
      case BLUE:    color(0,0,i); break;
      case YELLOW:  color(i,i,0); break;
      case CYAN:    color(0,i,i); break;
      case MAGENTA: color(i,0,i); break;
      case WHITE:   color(i,i,i); break;
    }
    delayMicroseconds(speed);
  }
  for(int i=255; i>=0; i--) {
    switch(colorName) {
      case RED:     color(i,0,0); break;
      case GREEN:   color(0,i,0); break;
      case BLUE:    color(0,0,i); break;
      case YELLOW:  color(i,i,0); break;
      case CYAN:    color(0,i,i); break;
      case MAGENTA: color(i,0,i); break;
      case WHITE:   color(i,i,i); break;
    }
    delayMicroseconds(speed);
  }
}

void RobbusKidsy :: Neopixel :: fadeInOut(uint8_t colorName) {
  for(int i=0; i<=255; i++) {
    switch(colorName) {
      case RED:     color(i,0,0); break;
      case GREEN:   color(0,i,0); break;
      case BLUE:    color(0,0,i); break;
      case YELLOW:  color(i,i,0); break;
      case CYAN:    color(0,i,i); break;
      case MAGENTA: color(i,0,i); break;
      case WHITE:   color(i,i,i); break;
    }
    delay(1);
  }
  for(int i=255; i>=0; i--) {
    switch(colorName) {
      case RED:     color(i,0,0); break;
      case GREEN:   color(0,i,0); break;
      case BLUE:    color(0,0,i); break;
      case YELLOW:  color(i,i,0); break;
      case CYAN:    color(0,i,i); break;
      case MAGENTA: color(i,0,i); break;
      case WHITE:   color(i,i,i); break;
    }
    delay(1);
  }
}

void RobbusKidsy :: Neopixel :: heartBeat(uint8_t colorName) {
  fadeInOut(colorName, 750);
  delay(100);
  fadeInOut(colorName, 750);
}

void RobbusKidsy :: Neopixel :: heartBeat(uint8_t colorName, uint8_t brightness) {
  if(brightness >= 255) brightness = 254;
  else if(brightness < 10) brightness = 10;
  for(uint8_t j=0; j<2; j++) {
    for(uint8_t i=0; i<=brightness; i++) {
      switch(colorName) {
        case RED:     color(i,0,0); break;
        case GREEN:   color(0,i,0); break;
        case BLUE:    color(0,0,i); break;
        case YELLOW:  color(i,i,0); break;
        case CYAN:    color(0,i,i); break;
        case MAGENTA: color(i,0,i); break;
        case WHITE:   color(i,i,i); break;
      }
      delay((uint8_t)255/brightness);
    }
    for(int i=brightness; i>=0; i--) {
      switch(colorName) {
        case RED:     color(i,0,0); break;
        case GREEN:   color(0,i,0); break;
        case BLUE:    color(0,0,i); break;
        case YELLOW:  color(i,i,0); break;
        case CYAN:    color(0,i,i); break;
        case MAGENTA: color(i,0,i); break;
        case WHITE:   color(i,i,i); break;
      }
      delay((uint8_t)255/brightness);
    }
    delay(100);
  }
}

void RobbusKidsy :: Neopixel :: off() {
  color(0,0,0);
}

void RobbusKidsy :: ColorSensor :: enable() {
  Wire.begin(); 
  if(!RGBWSensor.begin()) {
    Serial.println("ERROR: couldn't detect the sensor");
  }
  RGBWSensor.setConfiguration(VEML6040_IT_40MS + VEML6040_TRIG_ENABLE + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
  pinMode(LEDW, OUTPUT);
  digitalWrite(LEDW, HIGH);
}

void RobbusKidsy :: ColorSensor :: disable() {
  pinMode(LEDW, OUTPUT);
  digitalWrite(LEDW, LOW);
}

uint16_t RobbusKidsy :: ColorSensor :: getRed()   { return(RGBWSensor.getRed());   } 
uint16_t RobbusKidsy :: ColorSensor :: getGreen() { return(RGBWSensor.getGreen()); }
uint16_t RobbusKidsy :: ColorSensor :: getBlue()  { return(RGBWSensor.getBlue());  }
uint16_t RobbusKidsy :: ColorSensor :: getWhite() { return(RGBWSensor.getWhite()); }

uint8_t RobbusKidsy :: ColorSensor :: read() {
  red = RGBWSensor.getRed();
  green = RGBWSensor.getGreen();
  blue = RGBWSensor.getBlue();
  white = RGBWSensor.getWhite();

  sum = white;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
 
  // Escalar rgb a bytes
  r *= 256; g *= 256; b *= 256;

  ColorConverter :: RgbToHsv(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), hue, saturation, sat_value);
  hue360 = hue * 360;

  if (hue360 >= (red_hue - offset_hue) && hue360 < (red_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "red";
    value = RED;
  }
  else if (hue360 >= (yellow_hue - offset_hue) && hue360 < (yellow_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "yellow";
    value = YELLOW;
  }
  else if (hue360 >= (green_hue - offset_hue) && hue360 < (green_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "green";
    value = GREEN;
  }
  else if (hue360 >= (cyan_hue - offset_hue) && hue360 < (cyan_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "cyan";
    value = CYAN;
  }
  else if (hue360 >= (blue_hue - offset_hue) && hue360 < (blue_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "blue";
    value = BLUE;
  }
  else if (hue360 >= (magenta_hue - offset_hue) && hue360 < (magenta_hue + offset_hue) && (white > black_umbral && white < white_umbral))
  {
    name = "magenta";
    value = MAGENTA;
  }
  else if(white <= black_umbral) {
    name = "black";
    value = OFF;
  }
  else if(white >= white_umbral) {
    name = "white";
    value = WHITE;
  }
  else value = -1;
  return(value);
}

String RobbusKidsy :: ColorSensor :: getName(uint8_t name) {
  switch(name) {
    case BLACK:   return("black");    break;
    case RED:     return("red");      break;
    case GREEN:   return("gren");     break;
    case BLUE:    return("blue");     break;
    case YELLOW:  return("yellow");   break;
    case CYAN:    return("cyan");     break;
    case MAGENTA: return("magenta");  break;
    case WHITE:   return("white");    break;
  }
}

void ColorConverter :: RgbToHsv(uint8_t red, uint8_t green, uint8_t blue, double& hue, double& saturation, double& sat_value) {
  auto rd = static_cast<double>(red) / 255;
  auto gd = static_cast<double>(green) / 255;
  auto bd = static_cast<double>(blue) / 255;
  auto max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
   
  sat_value = max;

  auto d = max - min;
  saturation = max == 0 ? 0 : d / max;

  hue = 0;
  if (max != min) {
    if (max == rd) hue = (gd - bd) / d + (gd < bd ? 6 : 0);
    else if (max == gd) hue = (bd - rd) / d + 2;
    else if (max == bd) hue = (rd - gd) / d + 4;
    hue /= 6;
  }
}

double inline ColorConverter :: threeway_max(double a, double b, double c) {
  return max(a, max(b, c));
}

double inline ColorConverter :: threeway_min(double a, double b, double c) {
  return min(a, min(b, c));
}

double ColorConverter :: hue2rgb(double p, double q, double t) {
  if (t < 0) t += 1;
  if (t > 1) t -= 1;
  if (t < 1 / 6.0) return p + (q - p) * 6 * t;
  if (t < 1 / 2.0) return q;
  if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
  return p;
}

#endif
