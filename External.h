/*
 * External.h
 *
 * Created: 03.04.2017 21:04:41
 *  Author: Christof
 */



#ifndef EXTERNAL_H_
#define EXTERNAL_H_
#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "uartHardware.h"
#include "MyTimer.h"
#include "CRC_Calc.h"
#include "twi_master_driver.h"
#include "spi_driver.h"
#include "display.h"
#include "../ComReceiver/ComReceiver.h"

extern volatile uint8_t UART0_ring_received;
extern volatile  char UART0_ring_buffer[UART0_RING_BUFFER_SIZE];
extern volatile uint8_t UART1_ring_received;
extern volatile  char UART1_ring_buffer[UART1_RING_BUFFER_SIZE];

extern char const *Node;
extern const char RFM69Node;
extern const char RFM69Network;
extern const char RFM69Key[];

extern double dPressure , dSealevel ;
extern uint16_t uLicht;

extern double fExternalTemperature;
extern double fExternalHumidity;
extern double fExternalPressure;
extern double fExternalDewPoint;
extern double fInternalTemperature;
extern double fInternalHumidity;
extern double fInternalDewPoint;
extern double fInternalAbsHumitdity;
extern double MqttTime;
extern char   heaterAlarm[5];
extern char   heaterWater[5];
extern bool heaterCollectionAlarm;
extern uint32_t secondsCounter;
extern uint8_t gotEmailNumber;
extern uint8_t windowOpen;

extern char Compilation_Date[];
extern char Compilation_Time[];


extern const char *fehler_text[];
extern volatile TIMER MyTimers[MYTIMER_NUM];
extern volatile uint8_t statusReport;
extern volatile bool sendStatusReport;


extern volatile uint8_t statusKlima;
extern volatile uint8_t statusDruck;
extern volatile uint8_t statusSensoren;
extern volatile uint8_t statusLicht;
extern volatile uint8_t statusLastSensor;
extern volatile uint8_t statusTemperature;

extern char SecurityLevel;

extern uint16_t measureRate_100ms;

/* Global variables for TWI */
extern TWI_MasterDriver_t twiC_Master;    /*!< TWI master module. */
extern TWI_MasterDriver_t twiE_Master;    /*!< TWI master module. */

extern char quelle_KNET[3];

class Communication;   // Forward declaration
extern Communication cmulti;
class ComReceiver;
extern ComReceiver cmultiRec;

class SHT2;
extern SHT2 localClima;

typedef struct SPI_Master SPI_Master_t;
extern SPI_Master_t spiDisplay;

#ifdef USE_FUNK
extern SPI_Master_t spiRFM69;
class RFM69;
extern RFM69 myRFM;
#endif // USE_FUNK

extern PICTURECOMP email;
extern PICTURECOMP heizung;
extern PICTURECOMP telegram;
extern PICTURECOMP picWindowOpen;


extern CRC_Calc crcGlobal;

extern volatile uint8_t sendFree;
extern volatile uint8_t sendAnswerFree;
extern volatile bool nextSendReady;
extern volatile bool nowUpdateDisplay;
extern volatile bool nowUpdateClima;
extern volatile bool isDisplayReady;

#endif /* EXTERNAL_H_ */
