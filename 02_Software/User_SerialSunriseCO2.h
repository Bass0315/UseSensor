//
#ifndef USER_SERIALSUNRISECO2_H
#define USER_SERIALSUNRISECO2_H

/*
 * https://github.com/Bass0315/Sunrise_CO2
 */

#include <Wire.h>
#include "User_OLED.h"

#define SunriseSerial Serial1



/* Sunrise communication address, both for Modbus and I2C */
#define   SUNRISE_ADDR                  (const uint8_t)0x68

/* 
 * The delay when waiting for responses, in milliseconds.
 * Length based on the documentation, "Modbus on Senseair 
 * Sunrise".
 */
#define       WAIT_MS                     (const int)180
/* For baudrate equal 9600 the Modbus 3.5T interval is close to 3.5 ms, we round it to 4 ms*/
#define       INTER_PACKET_INTERVAL_MS    (const int)4


/* Error codes */
#define COMMUNICATION_ERROR             (const int)-1
#define ILLEGAL_FUNCTION                (const int)1
#define ILLEGAL_DATA_ADDRESS            (const int)2
#define ILLEGAL_DATA_VALUE              (const int)3

/* Register Addresses */
#define ERROR_STATUS                    (const uint16_t)0x0000
#define MEASUREMENT_MODE                (const uint16_t)0x000A
#define START_MEASUREMENT               (const uint16_t)0x0021
#define ABC_TIME                        (const uint16_t)0x0022
#define SENSOR_STATE                    (const uint16_t)0x0022

#define SENSOR_STATE_SZ                 (const uint16_t)0x000C

/* Measurement modes */
#define CONTINUOUS                      (const uint16_t)0x0000
#define SINGLE                          (const uint16_t)0x0001

/* Delays in milliseconds*/
#define STABILIZATION_MS                (const int)35
#define WAIT_FOR_PIN_MS                 200




void Init_SerialSunriseCO2(void);
bool read_sensor_id(uint8_t target);
bool read_sensor_config(uint8_t target);
bool change_measurement_mode(uint8_t target);
bool init_measurement(uint8_t target);
bool read_sensor_measurements(uint8_t target);
int modbus_read_response(int waitBytes, uint8_t funCode);
int read_holding_registers(uint8_t comAddr, uint16_t regAddr, uint16_t numReg);
int read_input_registers(uint8_t comAddr, uint16_t regAddr, uint16_t numReg);
int write_multiple_registers(uint8_t comAddr, uint16_t regAddr, uint16_t numReg, uint16_t writeVal[]);
int read_device_id(uint8_t comAddr, uint8_t objId);
int _handler(uint8_t pdu[], uint8_t funCode, int len);
uint16_t _generate_crc(uint8_t pdu[], int len);
void read_serialco2(void);

#endif
