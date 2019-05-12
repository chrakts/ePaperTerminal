/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "ePaperTerminal.h"

const char *Node = "IP";


const char *fehler_text[]={"memory errors","parameter error","unknown job","no transmission","command not allowed","CRC error","no active sensor"};

char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;

char quelle_KNET[3]="E1";
uint8_t isBroadcast = false;


volatile TIMER MyTimers[MYTIMER_NUM]= {	{TM_START,RESTART_YES,50,0,nextSensorStatus},
                                        {TM_STOP,RESTART_YES,REPORT_BETWEEN_SENSORS,0,nextReportStatus},
                                        {TM_START,RESTART_YES,100,0,sekundenTimer},
                                        {TM_STOP,RESTART_NO,100,0,NULL},		// Timeout-Timer
                                        {TM_START,RESTART_YES,6000,0,updateDisplay},		  // Update-Timer
                                        {TM_START,RESTART_NO,20,0,displayReady},		  // Ready-Display-Timer
                                        {TM_START,RESTART_YES,1000,0,updateClimate}		  // Local-Clima-Timer
};



float fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;

double fExternalTemperature = -99.0;
double fExternalHumidity    = -99.0;
double fExternalPressure    = -99.0;
double fExternalDewPoint    = -99.0;
double fInternalTemperature    = -99.0;
double fInternalHumidity    = -99.0;
double fInternalDewPoint    = -99.0;
char   heaterAlarm[5]="non";
char   heaterWater[5]="non";
bool heaterCollectionAlarm=false;
double   MqttTime= 1111111111;
uint32_t secondsCounter = 1545264000;
uint8_t gotEmailNumber = 0;

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
TWI_MasterDriver_t twiC_Master;    /*!< TWI master module. */
TWI_MasterDriver_t twiE_Master;    /*!< TWI master module. */


volatile bool nextSendReady=false;
volatile bool nowUpdateDisplay=false;
volatile bool nowUpdateClima=false;
volatile bool isDisplayReady=false;

Communication cmulti(0,Node,5);

SPI_Master_t spiDisplay;

SHT2 localClima(&twiC_Master,0x40);
