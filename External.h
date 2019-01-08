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

#include "twi_master_driver.h"

#include "myconstants.h"

#include "Serial.h"
#include "twi_master_driver.h"
#include "External.h"
#include "MyTimer.h"
#include "ComReceiver.h"
#include "CommandFunctions.h"
#include "sensirion_protocol.h"
#include "CmultiBusy.h"
#include "Communication.h"
#include "CRC_Calc.h"
#include "ePaperTerminal.h"

extern volatile uint8_t UART0_ring_received;
extern volatile  char UART0_ring_buffer[UART0_RING_BUFFER_SIZE];
extern volatile uint8_t UART1_ring_received;
extern volatile  char UART1_ring_buffer[UART1_RING_BUFFER_SIZE];

extern char const *Node;

extern float fTemperatur,fHumidity,fDewPoint,fAbsHumitdity;
extern double dPressure , dSealevel ;
extern uint16_t uLicht;

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
extern TWI_Master_t twiC_Master;    /*!< TWI master module. */
extern TWI_Master_t twiE_Master;    /*!< TWI master module. */


class Communication;   // Forward declaration
extern Communication cmulti;
extern char quelle_KNET[3];

extern CRC_Calc crcGlobal;

extern volatile uint8_t sendFree;
extern volatile uint8_t sendAnswerFree;
extern volatile bool nextSendReady;

#endif /* EXTERNAL_H_ */
