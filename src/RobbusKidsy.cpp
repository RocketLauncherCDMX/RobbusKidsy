#include <Arduino.h>
#include <RobbusKidsy.h>

#ifndef _ROBBUSKIDSY_CPP_
#define _ROBBUSKIDSY_CPP_

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

  // ----------------------------------------------------------------
  // MOTORS
  pinMode(DCM_SLEEP, OUTPUT);
  pinMode(DCM_LEFT_IN1, OUTPUT);
  pinMode(DCM_LEFT_IN2, OUTPUT);
  pinMode(DCM_RIGHT_IN1, OUTPUT);
  pinMode(DCM_RIGHT_IN2, OUTPUT);

  // Canales y pines (definidos en RobbusKidsy.h)
  ledcAttach(DCM_LEFT_IN1,   MOTOR_PWM_FREQ, MOTOR_PWM_RES);
  ledcAttach(DCM_LEFT_IN2,   MOTOR_PWM_FREQ, MOTOR_PWM_RES);
  ledcAttach(DCM_RIGHT_IN1,  MOTOR_PWM_FREQ, MOTOR_PWM_RES);
  ledcAttach(DCM_RIGHT_IN2,  MOTOR_PWM_FREQ, MOTOR_PWM_RES);

  // Estado seguro inicial: "stop" (tus funciones usan 255 como HIGH/idle)
  ledcWrite(DCM_LEFT_IN1,  0);
  ledcWrite(DCM_LEFT_IN1,  0);
  ledcWrite(DCM_LEFT_IN1, 0);
  ledcWrite(DCM_LEFT_IN1, 0);

  // Habilita drivers
  Move.enableMotors();

  // ----------------------------------------------------------------
  // BUZZER
  pinMode(BUZZER, OUTPUT);
  ledcAttach(BUZZER, 2000, 10); // pin, frecuencia, resolucion

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

  Led1.off();
  Led2.off();
  Led3.off();
  Led4.off();
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

  uint32_t tl = FlashMemory.getUInt("left_maxSpeed", 0);
  if (tl >= (uint32_t)Move.bottom_leftSpeed + 1 && tl <= 255) {
    Move.top_leftSpeed = (uint8_t)tl;
  }
  uint32_t tr = FlashMemory.getUInt("right_maxSpeed", 0);
  if (tr >= (uint32_t)Move.bottom_rightSpeed + 1 && tr <= 255) {
    Move.top_rightSpeed = (uint8_t)tr;
  }
  FlashMemory.end();
  
  // ---------------------------------------------------------------------------------------------------------------------------------------
  // ARROW CALIBRATION
  ArrowForward.thresshold   =  ((ArrowForward.untouchedCalibrate - ArrowForward.touchedCalibrate) / 2) + ArrowForward.touchedCalibrate;
  ArrowBackward.thresshold  =  ((ArrowBackward.untouchedCalibrate - ArrowBackward.touchedCalibrate) / 2) + ArrowBackward.touchedCalibrate;
  ArrowLeft.thresshold      =  ((ArrowLeft.untouchedCalibrate - ArrowLeft.touchedCalibrate) / 2) + ArrowLeft.touchedCalibrate;
  ArrowRight.thresshold     =  ((ArrowRight.untouchedCalibrate - ArrowRight.touchedCalibrate) / 2) + ArrowRight.touchedCalibrate;
  
  // ---------------------------------------------------------------------------------------------------------------------------------------
  // COLOR SENSOR INITIALIZATION
  if (!RGBWSensor.begin()) {
  Serial.println("ERROR: couldn't detect the sensor");
  }

  // 2) Continuous mode, 40 ms, sensor encendido
  RGBWSensor.setConfiguration(
      VEML6040_IT_40MS
    | VEML6040_TRIG_DISABLE
    | VEML6040_AF_AUTO
    | VEML6040_SD_ENABLE
  );

  pinMode(LEDW, OUTPUT);
  digitalWrite(LEDW, LOW);

  // --------------------------------------------------------------------
  // NEOPIXEL INITIALIZATION
  rmt_config_t config = {};
  config.rmt_mode                 = RMT_MODE_TX;
  config.channel                  = RMT_CHANNEL;
  config.gpio_num                 = (gpio_num_t)NEOPIXEL_PIN;
  config.mem_block_num            = 1;
  config.clk_div                  = RMT_CLK_DIV;
  config.tx_config.loop_en        = false;
  config.tx_config.carrier_en     = false;
  config.tx_config.idle_output_en = true;
  config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  

  rmt_config(&config);
  rmt_driver_install(config.channel, 0, 0);

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
    delay(100);
    Buzzer.noTone();
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
    Serial.println("Calibracion de la maxima speedocidad de los motores.");
    Serial.println("Los motores giraran hacia el frente. Si notas que Kidsy se va de lado");
    Serial.println("presiona la flecha hacia el lado que quieres corregir, hasta que notes que");
    Serial.println("se va recto. Si quieres aumentar o disminuir la speedocidad maxima, presiona");
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
          Serial.println("speedocidad Izquierda al minimo");
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
          Serial.println("speedocidad Izquierda al minimo");
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
          Serial.println("speedocidad Derecha al minimo");
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
          Serial.println("speedocidad Derecha al minimo");
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
          Serial.println("speedocidad de al menos un motor al maximo");
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
          Serial.println("speedocidad de ambos motores al maximo");
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
          Serial.println("speedocidad de al menos un motor al minimo");
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
          Serial.println("speedocidad de al menos un motor al minimo");
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

void RobbusKidsy :: movement :: motorLeft(int16_t value) {
  int16_t signedLimitedValue;
  uint8_t absValue;
  if(value < -255) signedLimitedValue = -255;
  else if(value > 255) signedLimitedValue = 255;
  else signedLimitedValue = value;
  absValue = abs(signedLimitedValue);
  adjusted_leftSpeed = map(absValue, 0, 255, bottom_leftSpeed, top_leftSpeed);

  if(signedLimitedValue < 0) {
    ledcWrite(DCM_LEFT_IN1, 0);
    ledcWrite(DCM_LEFT_IN2, adjusted_leftSpeed);
  } else if(signedLimitedValue > 0) {
    ledcWrite(DCM_LEFT_IN1, adjusted_leftSpeed);
    ledcWrite(DCM_LEFT_IN2, 0);
  } else {  // equals 0
    ledcWrite(DCM_LEFT_IN2, 0);
    ledcWrite(DCM_LEFT_IN1, 0);
  }
}

void RobbusKidsy :: movement :: motorRight(int16_t value) {
  int16_t signedLimitedValue;
  uint8_t absValue;
  if(value < -255) signedLimitedValue = -255;
  else if(value > 255) signedLimitedValue = 255;
  else signedLimitedValue = value; 
  absValue = abs(signedLimitedValue);
  adjusted_rightSpeed = map(absValue, 0, 255, bottom_rightSpeed, top_rightSpeed);
  
  if(signedLimitedValue < 0) {
    ledcWrite(DCM_RIGHT_IN1, 0);
    ledcWrite(DCM_RIGHT_IN2, adjusted_rightSpeed);
  } else if(signedLimitedValue > 0) {
    ledcWrite(DCM_RIGHT_IN1, adjusted_rightSpeed);
    ledcWrite(DCM_RIGHT_IN2, 0);
  } else {  // equals 0
    ledcWrite(DCM_RIGHT_IN1, 0);
    ledcWrite(DCM_RIGHT_IN2, 0);
  }
}

void RobbusKidsy :: movement :: enableMotors() {
  digitalWrite(DCM_SLEEP, HIGH);
}

void RobbusKidsy :: movement :: disableMotors() {
  digitalWrite(DCM_SLEEP, LOW);
}

void RobbusKidsy :: movement :: forward(uint8_t speed) {
  motorLeft(speed);
  motorRight(speed);
}

void RobbusKidsy :: movement :: backward(uint8_t speed) {
  motorLeft(-speed);
  motorRight(-speed);
}

void RobbusKidsy :: movement :: turnLeft(uint8_t speed) {
  motorLeft(-speed);
  motorRight(speed);
}

void RobbusKidsy :: movement :: turnRight(uint8_t speed) {
  motorLeft(speed);
  motorRight(-speed);
}

void RobbusKidsy :: movement :: stop() {
  motorLeft(0);
  motorRight(0);
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

void RobbusKidsy::Buzzer::playTone(int16_t frequency, uint16_t duration) {
  if (frequency <= 0) { 
    noTone(); 
    return;
  }
  ledcChangeFrequency(BUZZER, frequency, 10);
  ledcWrite(BUZZER, 128);     // usa volumen global
  delay(duration);
  ledcWrite(BUZZER, 0);     // duty = 0 → silencio
}

void RobbusKidsy::Buzzer::playTone(int16_t frequency) {
  if (frequency <= 0) { noTone(); return; }
  ledcChangeFrequency(BUZZER, frequency, 10);
  ledcWrite(BUZZER, 128);
}

void RobbusKidsy::Buzzer::noTone() {
  ledcWrite(BUZZER, 0);     // duty = 0 → silencio
}

// Nota musical a frecuencia básica (ejemplo simple en octava 4)
void RobbusKidsy::Buzzer::playNote(char note, uint16_t duration) {
  int16_t freq = 0;
  switch (note) {
    case 'C': freq = 261; break;
    case 'D': freq = 294; break;
    case 'E': freq = 329; break;
    case 'F': freq = 349; break;
    case 'G': freq = 392; break;
    case 'A': freq = 440; break;
    case 'B': freq = 494; break;
    default:  freq = 0;   break;
  }
  if (freq > 0) playTone(freq, duration);
  else noTone();
}

// Sonido estilo R2D2: barrido de frecuencias rápidas
void RobbusKidsy::Buzzer::r2d2(uint16_t time) {
  uint32_t start = millis();
  while (millis() - start < time) {
    int16_t f = random(800, 3000);     // rango agudo
    uint16_t d = random(40, 120);      // duración breve
    playTone(f, d);
    delay(d);                          // en este caso sí breve bloqueante
  }
  noTone();
}

void RobbusKidsy :: Neopixel :: color(uint8_t r, uint8_t g, uint8_t b) {
  rmt_item32_t items[24];
  uint8_t grb[3] = { g, r, b };
  int idx = 0;

  for (int j = 0; j < 3; ++j) {
    for (int bit = 7; bit >= 0; --bit) {
      bool one = (grb[j] >> bit) & 0x01;
      items[idx++] = ws_bit(one);
    }
  }

  // Transmitir e insertar reset (>50us) al final con un delay
  rmt_write_items(RMT_CHANNEL, items, 24, true);  // wait_tx_done = true
  // Reset: mantener bajo > 50 us (el RMT ya quedó en idle bajo)
  delayMicroseconds(80);
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
  if (!RGBWSensor.begin()) {
  Serial.println("ERROR: couldn't detect the sensor");
  }

  // 2) Continuous mode, 40 ms, sensor encendido
  RGBWSensor.setConfiguration(
      VEML6040_IT_40MS
    | VEML6040_TRIG_DISABLE
    | VEML6040_AF_AUTO
    | VEML6040_SD_ENABLE
  );

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

uint8_t RobbusKidsy::ColorSensor::read() {
  // --- OPCIONAL: medición diferencial para cancelar luz ambiente ----
  // Si el ambiente es MUY variable, cada N lecturas apaga LEDW 1 ciclo:
  // static uint8_t k = 0;
  // uint16_t r0,g0,b0,w0, r1,g1,b1,w1;
  // if (++k >= 8) {                                   // cada 8 lecturas
  //   digitalWrite(LEDW, LOW); delayMicroseconds(200); // settle rápido
  //   r0 = RGBWSensor.getRed(); g0 = RGBWSensor.getGreen();
  //   b0 = RGBWSensor.getBlue(); w0 = RGBWSensor.getWhite();
  //   digitalWrite(LEDW, HIGH);                        // ON otra vez
  //   delayMicroseconds(200);
  //   k = 0;
  // }
  // ---------------------------------------------------------------

  // Lectura continua (40 ms integración ya en curso)
  uint32_t R = RGBWSensor.getRed();
  uint32_t G = RGBWSensor.getGreen();
  uint32_t B = RGBWSensor.getBlue();
  uint32_t W = RGBWSensor.getWhite();

  // Si usaste diferencial, resta: R-=r0; G-=g0; B-=b0; W-=w0; y satura a 0.

  // Guardas simples ante valores raros
  const uint32_t W_MIN = 20;    // evita dividir por casi-cero
  const uint32_t W_MAX = 65000; // evita saturación ADC
  if (W > W_MAX) W = W_MAX;

  // Normalización: usa W si es confiable; si no, usa suma RGB
  double denom = (W >= W_MIN) ? (double)W : (double)(R + G + B + 1);
  double r = (double)R / denom;
  double g = (double)G / denom;
  double b = (double)B / denom;

  // A bytes (0..255) sin gamma (más estable para HSV rápido)
  uint8_t r8 = (uint8_t) (r * 255.0 + 0.5);
  uint8_t g8 = (uint8_t) (g * 255.0 + 0.5);
  uint8_t b8 = (uint8_t) (b * 255.0 + 0.5);

  // HSV
  double h,s,v;
  ColorConverter::RgbToHsv(r8,g8,b8, h,s,v);  // h in [0..1]
  double h360_now = h * 360.0;

  // IIR suave sobre el hue (reduce jitter, ~1 muestra de “memoria”)
  static bool hueInit = false;
  static double h360_f = 0.0;
  if (!hueInit) { h360_f = h360_now; hueInit = true; }
  const double ALPHA = 0.65;          // más alto = responde más rápido
  // manejar wrap-around 359->0:
  double dh = h360_now - h360_f;
  if (dh > 180) dh -= 360;
  if (dh < -180) dh += 360;
  h360_f += ALPHA * dh;
  if (h360_f < 0) h360_f += 360;
  if (h360_f >= 360) h360_f -= 360;

  hue360 = h360_f;          // guarda para calibración/telemetría
  saturation = s;           // si lo usas para depurar
  sat_value  = v;
  white      = W;
  red = R; green = G; blue = B;

  // Clasificación: primero blancos/negros por W
  if (W <= black_umbral) {
    name = "black"; value = OFF; return value;
  }
  if (W >= white_umbral) {
    name = "white"; value = WHITE; return value;
  }

  // Histéresis por color (±offset_hue para entrar, ±offset_hue/2 para salir)
  // y “pegajosidad”: no cambies de color si el anterior sigue “cerca”.
  auto inWin = [&](uint16_t target, uint16_t off){
    // maneja wrap-around (ej. rojo cerca de 0/360)
    int16_t d = (int16_t)hue360 - (int16_t)target;
    while (d > 180) d -= 360;
    while (d < -180) d += 360;
    return (abs(d) <= off);
  };

  static int8_t last = -1;
  const uint16_t IN  = offset_hue;        // ventana de entrada (calibrable)
  const uint16_t OUT = offset_hue / 2;    // ventana de salida (más estricta)

  // Si ya teníamos color, aplica ventana más estrecha para mantenerlo
  auto preferKeep = [&](uint16_t target){
    return inWin(target, OUT);
  };

  // Prioriza mantener color actual si sigue dentro OUT
  if (last == RED     && preferKeep(red_hue))       { name="red";     value=RED;     return last; }
  if (last == YELLOW  && preferKeep(yellow_hue))    { name="yellow";  value=YELLOW;  return last; }
  if (last == GREEN   && preferKeep(green_hue))     { name="green";   value=GREEN;   return last; }
  if (last == CYAN    && preferKeep(cyan_hue))      { name="cyan";    value=CYAN;    return last; }
  if (last == BLUE    && preferKeep(blue_hue))      { name="blue";    value=BLUE;    return last; }
  if (last == MAGENTA && preferKeep(magenta_hue))   { name="magenta"; value=MAGENTA; return last; }

  // Si no, detecta nueva entrada con ventana IN (más ancha)
  if (inWin(red_hue,     IN)) { name="red";     value=RED;     last=value; return value; }
  if (inWin(yellow_hue,  IN)) { name="yellow";  value=YELLOW;  last=value; return value; }
  if (inWin(green_hue,   IN)) { name="green";   value=GREEN;   last=value; return value; }
  if (inWin(cyan_hue,    IN)) { name="cyan";    value=CYAN;    last=value; return value; }
  if (inWin(blue_hue,    IN)) { name="blue";    value=BLUE;    last=value; return value; }
  if (inWin(magenta_hue, IN)) { name="magenta"; value=MAGENTA; last=value; return value; }

  // No clasificado
  value = (uint8_t)-1;
  return value;
}

String RobbusKidsy::ColorSensor::getName(uint8_t id) {
  switch (id) {
    case BLACK:   return "black";
    case RED:     return "red";
    case GREEN:   return "green";   // <-- corregido
    case BLUE:    return "blue";
    case YELLOW:  return "yellow";
    case CYAN:    return "cyan";
    case MAGENTA: return "magenta";
    case WHITE:   return "white";
    default:      return "unknown";
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
