#ifndef _ROBBUSKIDSY_H
#define _ROBBUSKIDSY_H_

#include <Arduino.h>
#include <Wire.h>
#include "veml6040.h"
#include "Preferences.h"
#include "driver/rmt.h"

// -------------------------------------------------------------------------------------------------------------
// RobbusKidsy Classes
class RobbusKidsy {
private:
  // Global private constants
  #define ARROW_BUFFER_LENGHT   25      // large of buffer for smooth arrow's touch

  uint64_t timeStamp;
  uint8_t repeatCounter = 0;
public:
  // Global public constants
  #define BLACK         0
  #define OFF           0
  #define RED           1
  #define GREEN         2
  #define BLUE          3
  #define YELLOW        4
  #define CYAN          5
  #define MAGENTA       6
  #define WHITE         7

  #define FORWARD    1     
  #define BACKWARD  2
  #define LEFT  3
  #define RIGHT 4
  #define ON    1
  #define OFF   0
  #define NONE  0

  #define LEDW      16
  #define BUZZER    0
  #define NEOPIXEL  1
  #define L1  2
  #define L2  3
  #define L3  4
  #define L4  5

  void begin();
  void calibrateSensor();
  void calibrateArrows();
  void calibrateMotors();
  uint8_t getArrowPad();
  
  class buttons {
  private:
    bool new_state = HIGH;
    bool old_state = HIGH;
  public:
    #define PRESSED           1
    #define RELEASED          0
    #define HOLD_RELEASED     2
    #define HOLD_PRESSED      3
    
    // Version 3.2
    #define BUTTON_A          34
    #define BUTTON_B          35
    #define BUTTON_C          36
    
    uint8_t pin;      
    uint8_t status;
    uint8_t read();
    
  } ButtonA, ButtonB, ButtonC;

  class Led {
  public:
    #define LED1    21
    #define LED2    27
    #define LED3    5
    #define LED4    26
    uint8_t status;
    uint8_t pin;
    bool Led1 = LOW, :1;
    bool Led2 = LOW, :1;
    bool Led3 = LOW, :1;
    bool Led4 = LOW, :1;
    void set(bool);
    void on();
    void off();
    bool toggle();
    void blink(uint8_t, uint16_t);
      
  } Led1, Led2, Led3, Led4;

  class Arrows {    
  private:
    #define AN_UP             T5     
    #define AN_DOWN           T4
    #define AN_LEFT           T6
    #define AN_RIGHT          T3
    #define ARROW_BUFFER_SIZE 25
    uint8_t buffer[ARROW_BUFFER_SIZE];
    uint8_t readIndex = 0;  // the index of the current reading
    uint16_t total = 0;     // the running total
    uint8_t average = 0;    // the average
    bool new_state = LOW, :1;
    bool old_state = LOW, :1;
        
  public:
    #define NOTOUCHED         0
    #define TOUCHED           1
    #define HOLD_TOUCHED      2
    #define HOLD_NOTOUCHED    3
    uint8_t pin;
    // variables and function for analog reading
    uint8_t thresshold = 100;
    uint8_t untouchedCalibrate = 70;
    uint8_t touchedCalibrate = 3;
    uint8_t analog;
    uint8_t analogRead();
    // variables and function for digital reading
    uint8_t status;
    uint8_t read();
        
  } ArrowForward, ArrowBackward, ArrowLeft, ArrowRight;

  class movement {
  private:
    #define MOTOR_PWM_FREQ          20000
    #define MOTOR_PWM_RES           8

    #define DCM_LEFT_IN1            2
    #define DCM_LEFT_IN2            4
    #define DCM_RIGHT_IN1           17
    #define DCM_RIGHT_IN2           18
    #define DCM_SLEEP               33

    uint8_t adjusted_leftSpeed;
    uint8_t adjusted_rightSpeed;
    uint direction = FORWARD;
    bool status = OFF;

  public:
    #define STOP   0
    uint8_t bottom_leftSpeed = 50;
    uint8_t bottom_rightSpeed = 50;
    uint8_t top_leftSpeed = 255;
    uint8_t top_rightSpeed = 255;

    void disableMotors();
    void enableMotors();
    void motorLeft(int16_t);
    void motorRight(int16_t);
    void forward(uint8_t);
    void backward(uint8_t);
    void turnLeft(uint8_t);
    void turnRight(uint8_t);
    void stop();

  } Move;

  class Buzzer {
  private:
    bool _beepActive = false;
    uint32_t _beepEnd;

  public:
    #define BUZZER  25

    void playTone(int16_t, uint16_t);
    void playTone(int16_t);
    void noTone();
    void playNote(char, uint16_t);
    void r2d2(uint16_t);
  } Buzzer;

  

  class Neopixel {
  private:
    #define NEOPIXEL_PIN     19
    #define RMT_CHANNEL      RMT_CHANNEL_0
    #define RMT_CLK_DIV      2

    #define WS_T0H_TICKS     16           // ~0.40 us
    #define WS_T0L_TICKS     34           // ~0.85 us
    #define WS_T1H_TICKS     32           // ~0.80 us
    #define WS_T1L_TICKS     18           // ~0.45 us

    static inline rmt_item32_t ws_bit(bool one) {
      rmt_item32_t i;
      if (one) {
        i.level0 = 1; i.duration0 = WS_T1H_TICKS;
        i.level1 = 0; i.duration1 = WS_T1L_TICKS;
      } else {
        i.level0 = 1; i.duration0 = WS_T0H_TICKS;
        i.level1 = 0; i.duration1 = WS_T0L_TICKS;
      }
      return i;
    }

  public:
    #define NONE             0
    #define ANY_BUTTON       5
    #define ANY_ARROW        6

    void color(uint8_t,uint8_t,uint8_t);
    void color(uint8_t,uint8_t);
    void color(uint8_t);
    void fadeInOut(uint8_t, uint16_t);
    void fadeInOut(uint8_t);
    void heartBeat(uint8_t);
    void heartBeat(uint8_t, uint8_t);
    void off(void);

  } Neopixel;

  class ColorSensor {
  private:
        
  public:
    double hue, saturation, sat_value;
    uint32_t sum;
    float r, g, b;
    uint16_t hue360;
    bool LedW = LOW, :1;
    uint8_t value;
    uint8_t valueMatrix[3];
    String name;
    byte gammatable[256];
    uint16_t white, red, green, blue;

    uint16_t black_umbral;
    uint16_t white_umbral;

    uint16_t red_hue;
    uint16_t green_hue;
    uint16_t blue_hue;
    uint16_t yellow_hue;
    uint16_t cyan_hue;
    uint16_t magenta_hue;

    uint8_t offset_hue = 10;

    void enable();
    void disable();
    uint16_t getRed();
    uint16_t getGreen();
    uint16_t getBlue();
    uint16_t getWhite();
    uint8_t read();
    String getName(uint8_t);
  } ColorSensor;  
};
// End of Robbus classes
// ---------------------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------------------
// Independent helping Classes
class ColorConverter {
  public:
    static void RgbToHsv(uint8_t r, uint8_t g, uint8_t b, double &hue, double &saturation, double &value);
  
  private:
    static double threeway_max(double a, double b, double c);
    static double threeway_min(double a, double b, double c);
    static double hue2rgb(double p, double q, double t);
};


// End of independent helping classes
// ---------------------------------------------------------------------------------------------------------------

#endif
