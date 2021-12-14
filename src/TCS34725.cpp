#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include <math.h>
#include <stdlib.h>

#include "TCS34725.h"

/*!
 *  @brief  Writes a register and an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
void TCS34725::write8(uint8_t reg, uint32_t value) {
  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
  _wire->write(value & 0xFF);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
  _wire->send(value & 0xFF);
#endif
  _wire->endTransmission();
}

/*!
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
uint8_t TCS34725::read8(uint8_t reg) {
  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
#endif
  _wire->endTransmission();

  _wire->requestFrom(_i2caddr, (uint8_t)1);
#if ARDUINO >= 100
  return _wire->read();
#else
  return _wire->receive();
#endif
}

/*!
 *  @brief  Reads a 16 bit values over I2C
 *  @param  reg
 *  @return value
 */
uint16_t TCS34725::read16(uint8_t reg) {
  uint16_t x;
  uint16_t t;

  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
#endif
  _wire->endTransmission();

  _wire->requestFrom(_i2caddr, (uint8_t)2);
#if ARDUINO >= 100
  t = _wire->read();
  x = _wire->read();
#else
  t = _wire->receive();
  x = _wire->receive();
#endif
  x <<= 8;
  x |= t;
  return x;
}

/*!
 *  @brief  Enables the device
 */
void TCS34725::enable() {
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(3);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  /* Set a delay for the integration time.
    This is only necessary in the case where enabling and then
    immediately trying to read values back. This is because setting
    AEN triggers an automatic integration, so if a read RGBC is
    performed too quickly, the data is not yet valid and all 0's are
    returned */
  switch (_tcs34725IntegrationTime) {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    delay(700);
    break;
  }
}

/*!
 *  @brief  Disables the device (putting it in lower power sleep mode)
 */
void TCS34725::disable() {
  /* Turn the device off to save power */
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

/*!
 *  @brief  Constructor
 *  @param  it
 *          Integration Time
 *  @param  gain
 *          Gain
 */
TCS34725::TCS34725(tcs34725IntegrationTime_t it,
                                     tcs34725Gain_t gain) {
  _tcs34725Initialised = false;
  _tcs34725IntegrationTime = it;
  _tcs34725Gain = gain;
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @return True if initialization was successful, otherwise false.
 */
boolean TCS34725::begin() {
  _i2caddr = TCS34725_ADDRESS;
  _wire = &Wire;

  return init();
}

/*!
 *  @brief  Part of begin
 *  @return True if initialization was successful, otherwise false.
 */
boolean TCS34725::init() {
  _wire->begin(23,22);

  /* Make sure we're actually connected */
  uint8_t x = read8(TCS34725_ID);
  if ((x != 0x44) && (x != 0x10)) {
    return false;
  }
  _tcs34725Initialised = true;

  /* Set default integration time and gain */
  setIntegrationTime(_tcs34725IntegrationTime);
  setGain(_tcs34725Gain);

  /* Note: by default, the device is in power down mode on bootup */
  enable();

  return true;
}

/*!
 *  @brief  Sets the integration time for the TC34725
 *  @param  it
 *          Integration Time
 */
void TCS34725::setIntegrationTime(tcs34725IntegrationTime_t it) {
  if (!_tcs34725Initialised)
    begin();

  /* Update the timing register */
  write8(TCS34725_ATIME, it);

  /* Update value placeholders */
  _tcs34725IntegrationTime = it;
}

/*!
 *  @brief  Adjusts the gain on the TCS34725
 *  @param  gain
 *          Gain (sensitivity to light)
 */
void TCS34725::setGain(tcs34725Gain_t gain) {
  if (!_tcs34725Initialised)
    begin();

  /* Update the timing register */
  write8(TCS34725_CONTROL, gain);

  /* Update value placeholders */
  _tcs34725Gain = gain;
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void TCS34725::getRawData(uint16_t *r, uint16_t *g, uint16_t *b,
                                   uint16_t *c) {
  if (!_tcs34725Initialised)
    begin();

  *c = read16(TCS34725_CDATAL);
  *r = read16(TCS34725_RDATAL);
  *g = read16(TCS34725_GDATAL);
  *b = read16(TCS34725_BDATAL);

  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime) {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    delay(700);
    break;
  }
}