/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "ePaperTerminal.h"

const char *Node = "H1";


const char *fehler_text[]={"memory errors","parameter error","unknown job","no transmission","command not allowed","CRC error","no active sensor"};

char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;

char quelle_KNET[3]="";


volatile TIMER MyTimers[MYTIMER_NUM]= {	{TM_START,RESTART_YES,50,0,nextSensorStatus},
                                        {TM_STOP,RESTART_YES,REPORT_BETWEEN_SENSORS,0,nextReportStatus},
                                        {TM_STOP,RESTART_YES,100,0,sekundenTimer},
										{TM_STOP,RESTART_NO,100,0,NULL}		// Timeout-Timer
};



float fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;

volatile uint8_t statusSensoren = KLIMASENSOR;
volatile uint8_t statusReport = TEMPREPORT;
volatile bool    sendStatusReport = false; // dient dazu, keinen Report zu senden, bevor Sensordaten vorhanden sind
volatile uint8_t statusKlima = NOTHING_CLIMA_TODO;
volatile uint8_t statusLastSensor = NOTHING_LAST_TODO;
volatile uint8_t statusTemperature=NOTHING_TODO;

int errno;      // Globale Fehlerbehandlung

char SecurityLevel = 0;

uint16_t measureRate_100ms=10;
//extern uint8_t averageRate=32;

/* Global variables for TWI */
TWI_Master_t twiC_Master;    /*!< TWI master module. */
TWI_Master_t twiE_Master;    /*!< TWI master module. */


volatile bool nextSendReady=false;

Communication cmulti(1,Node,5);


