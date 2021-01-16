#ifndef _ROBBUSKIDSY_H
#define _ROBBUSKIDSY_H_

#include <Arduino.h>
#include <Wire.h>
#include "TCS34725.h"
#include "pitches.h"

// -------------------------------------------------------------------------------------------------------------
// RobbusKidsy Classes
class RobbusKidsy {
private:
  // Global private constants
  #define PWM_MOTOR_FREQ        30000   // this variable is used to define the time period 
  #define PWM_RESOUTION         8       // this will define the resolution of the signal which is 8 in this case
  #define ARROW_BUFFER_LENGHT   25      // large of buffer for smooth arrow's touch
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
  void begin(uint8_t);
  
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
    //#define BUTTON_A          34
    //#define BUTTON_B          35
    //#define BUTTON_C          36

    // Version 3.1
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
    uint8_t thresshold = 50;
    uint8_t readIndex = 0;  // the index of the current reading
    uint16_t total = 0;     // the running total
    uint8_t average = 0;    // the average
    uint8_t noTouchCalibrate = 70;
    uint8_t touchCalibrate = 3;
    bool new_state = LOW, :1;
    bool old_state = LOW, :1;
        
  public:
    #define NOTOUCHED         0
    #define TOUCHED           1
    #define HOLD_TOUCHED      2
    #define HOLD_NOTOUCHED    3
    uint8_t pin;
    // variables and function for analog reading
    uint8_t analog;
    uint8_t analogRead();
    void calibrate(bool);
    // variables and function for digital reading
    uint8_t status;
    uint8_t read();
        
  } ArrowForward, ArrowBackward, ArrowLeft, ArrowRight;

  class movement {
  private:
    #define DCM_LEFT_IN1            2
    #define DCM_LEFT_IN2            18
    #define DCM_RIGHT_IN1           4
    #define DCM_RIGHT_IN2           17
    #define PWM_CHANNEL_LEFT_IN1    0
    #define PWM_CHANNEL_LEFT_IN2    1
    #define PWM_CHANNEL_RIGHT_IN1   2
    #define PWM_CHANNEL_RIGHT_IN2   3

    uint8_t adjusted_leftSpeed;
    uint8_t adjusted_rightSpeed;
    uint8_t speed = 0;
    uint direction = FORWARD;
    bool status = OFF;
    uint8_t pwm_channel;
    uint8_t dcm_in;
    uint8_t top_leftSpeed = 255;
    uint8_t top_rightSpeed = 255;

  public:
    #define STOP   0

    void MotorLeft(int16_t);
    void MotorRight(int16_t);
    void forward(uint8_t);
    void backward(uint8_t);
    void turnLeft(uint8_t);
    void turnRight(uint8_t);
    void stop();

  } Move;

  class Buzzer {
  private:
    uint16_t old_frequency = 0;
  public:
    #define BUZZER  25
    #define PWM_CHANNEL_BUZZER  4
    char names[];
    uint16_t tones[];

    void playTone(int16_t, uint16_t);
    void playTone(int16_t);
    void playTone(int16_t, uint16_t, uint8_t);
    void noTone();
    void playNote(char, uint16_t);
    void r2d2(uint16_t);
  } Buzzer;

  

  class Neopixel {
  private:
    uint8_t value[3];  // GRB value
    uint8_t col, bit;
    uint8_t i;
  public:
    #define NONE          0
    #define ANY_BUTTON    5
    #define ANY_ARROW     6

    int tempo;

    void color(uint8_t,uint8_t,uint8_t);
    void color(uint8_t);
    void fadeInOut(uint8_t, uint16_t);
    void fadeInOut(uint8_t);
    void heartBeat(uint8_t);
    void off(void);

  } Neopixel;

  class ColorSensor {
  private:
    #define BLACK_UMBRAL  150
    #define WHITE_UMBRAL  350
    double hue, saturation, sat_value;
    uint32_t sum;
    float r, g, b;
    
  public:
    uint16_t hue360;
    bool LedW = LOW, :1;
    uint8_t value;
    String name;
    byte gammatable[256];
    uint16_t white, red, green, blue;
    /*uint16_t red_hue = 65;
    uint16_t green_hue = 135;
    uint16_t blue_hue = 200;
    uint16_t yellow_hue = 95;
    uint16_t cyan_hue = 175;
    uint16_t magenta_hue = 220;*/
    uint16_t red_hue = 10;
    uint16_t green_hue = 80;
    uint16_t blue_hue = 200;
    uint16_t yellow_hue = 45;
    uint16_t cyan_hue = 230;
    uint16_t magenta_hue = 350;


    uint8_t offset_hue = 15;

    uint8_t read();
    void enable();
    void disable();
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
