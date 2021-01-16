#include <Arduino.h>
#include <RobbusKidsy.h>

#ifndef _ROBBUSKIDSY_CPP_
#define _ROBBUSKIDSY_CPP_

rmt_data_t led_data[24];
rmt_obj_t* rmt_send = NULL;

TCS34725 tcs = TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

void RobbusKidsy :: begin() {

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

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

  ButtonA.pin = BUTTON_A;
  ButtonB.pin = BUTTON_B;
  ButtonC.pin = BUTTON_C;

  Led1.pin = LED1;
  Led2.pin = LED2;
  Led3.pin = LED3;
  Led4.pin = LED4;

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

  tcs.begin();

  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    ColorSensor.gammatable[i] = x;
  }

  rmt_send = rmtInit(19, true, RMT_MEM_64);
  rmtSetTick(rmt_send, 100);

  Neopixel.off();
  delay(1);
}

void RobbusKidsy :: begin(uint8_t mode) {

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

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

  ButtonA.pin = BUTTON_A;
  ButtonB.pin = BUTTON_B;
  ButtonC.pin = BUTTON_C;

  Led1.pin = LED1;
  Led2.pin = LED2;
  Led3.pin = LED3;
  Led4.pin = LED4;

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

  tcs.begin();

  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    ColorSensor.gammatable[i] = x;
  }

  rmt_send = rmtInit(19, true, RMT_MEM_64);
  rmtSetTick(rmt_send, 100);

  Neopixel.off();
  delay(1);

  switch(mode) {
    case BUZZER:
      delay(100);
      Buzzer.playTone(2000, 50);
      delay(100);
      Buzzer.playTone(2000, 50);
      break;
    case L1:
      
      Led1.blink(2, 100);
      break;
    case L2:
      Led2.blink(2, 100);
      break;
    case L3:
      Led3.blink(2, 100);
      break;
    case L4:
      Led4.blink(2, 100);
      break;
    case NEOPIXEL:
      Neopixel.heartBeat(GREEN);
      break;
  }
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

void RobbusKidsy :: movement :: MotorLeft(int16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < -255) vel = -255;
  if(vel > 0) direction = FORWARD;
  else direction = BACKWARD;
  speed = abs(vel);
  if(direction == FORWARD) {
    adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
  }
  else {
    adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
    ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
  }
}

void RobbusKidsy :: movement :: MotorRight(int16_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < -255) vel = -255;
  if(vel > 0) direction = FORWARD;
  else direction = BACKWARD;
  speed = abs(vel);
  if(direction == FORWARD) {
    adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  }
  else {
    adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
    ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
  }
}

void RobbusKidsy :: movement :: forward(uint8_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
  adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
}

void RobbusKidsy :: movement :: backward(uint8_t vel) {
  if(vel > 255) vel = 255;
  else if(vel < 0) vel = 0;
  speed = vel;
  adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
  adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
}

void RobbusKidsy :: movement :: turnLeft(uint8_t speed) {
  adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255 - adjusted_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255 - adjusted_leftSpeed);
}

void RobbusKidsy :: movement :: turnRight(uint8_t speed) {
  adjusted_leftSpeed = map(speed, 0, 255, 0, top_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255 - adjusted_leftSpeed);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
  adjusted_rightSpeed = map(speed, 0, 255, 0, top_rightSpeed);
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255 - adjusted_rightSpeed);
}

void RobbusKidsy :: movement :: stop() {
  ledcWrite(PWM_CHANNEL_RIGHT_IN1, 255);
  ledcWrite(PWM_CHANNEL_RIGHT_IN2, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN1, 255);
  ledcWrite(PWM_CHANNEL_LEFT_IN2, 255);
}

uint8_t RobbusKidsy :: Arrows :: analogRead() {
  // subtract the last reading:
  total = total - buffer[readIndex];
  // read from the sensor:
  buffer[readIndex] = touchRead(pin);
  // add the reading to the total:
  total = total + buffer[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= ARROW_BUFFER_LENGHT) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / ARROW_BUFFER_LENGHT;
  analog = average;
  return(average);
}

void RobbusKidsy :: Arrows :: calibrate(bool state) {
  if(state == NOTOUCHED) {
    noTouchCalibrate = analogRead();
  }
  else touchCalibrate = analogRead();
  thresshold = (noTouchCalibrate - touchCalibrate) / 2;
}

uint8_t RobbusKidsy :: Arrows :: read() {
  analogRead();
  if(analog < thresshold) new_state = HIGH;
  else new_state = LOW;
  if(new_state == LOW && old_state == LOW) status = HOLD_NOTOUCHED;
  if(new_state == LOW && old_state == HIGH) status = NOTOUCHED;
  if(new_state == HIGH && old_state == LOW) status = TOUCHED;
  if(new_state == HIGH && old_state == HIGH) status = HOLD_TOUCHED;
  if(old_state != new_state) delay(25);
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

void RobbusKidsy :: Neopixel :: off() {
  color(0,0,0);
}

uint8_t RobbusKidsy :: ColorSensor :: read() {
  tcs.getRawData(&red, &green, &blue, &white);
  
  // Hacer rgb medición relativa
  sum = white;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
 
  // Escalar rgb a bytes
  r *= 256; g *= 256; b *= 256;

  // Convertir a hue, saturation, value
  ColorConverter::RgbToHsv(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), hue, saturation, sat_value);
  hue360 = hue * 360;

  if (hue360 >= (red_hue - offset_hue) && hue360 < (red_hue + offset_hue) && 
                white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "red";
    value = RED;
  }
  else if (hue360 >= (yellow_hue - offset_hue) && hue360 < (yellow_hue + offset_hue) && 
          white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "yellow";
    value = YELLOW;
  }
  else if (hue360 >= (green_hue - offset_hue) && hue360 < (green_hue + offset_hue) && 
          white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "green";
    value = GREEN;
  }
  else if (hue360 >= (cyan_hue - offset_hue) && hue360 < (cyan_hue + offset_hue) && 
          white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "cyan";
    value = CYAN;
  }
  else if (hue360 >= (blue_hue - offset_hue) && hue360 < (blue_hue + offset_hue) && 
          white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "blue";
    value = BLUE;
  }
  else if (hue360 >= (magenta_hue - offset_hue) && hue360 < (magenta_hue + offset_hue) && 
          white > BLACK_UMBRAL && white < WHITE_UMBRAL)
  {
    name = "magenta";
    value = MAGENTA;
  }
  else if(white <= BLACK_UMBRAL) {
    name = "black";
    value = OFF;
  }
  else if(white >= WHITE_UMBRAL) {
    name = "white";
    value = WHITE;
  }
  return(value);
}

void RobbusKidsy :: ColorSensor :: enable() {
    tcs.enable();
    digitalWrite(LEDW, HIGH);
}

void RobbusKidsy :: ColorSensor :: disable() {
    tcs.disable();
    digitalWrite(LEDW, LOW);
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
