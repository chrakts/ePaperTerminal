/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "ePaperTerminal.h"

const char *Node = "IP";

const char RFM69Node    = 'T';
const char RFM69Network = '1';

const char RFM69Key[16] = {RFM69KEY};

const char *fehler_text[]={"memory errors","parameter error","unknown job","no transmission","command not allowed","CRC error","no active sensor"};

char quelle_KNET[3]="E1";

volatile TIMER MyTimers[MYTIMER_NUM]= {	{TM_STOP,RESTART_NO,5,0,NULL},
                                        {TM_START,RESTART_YES,50,0,nextSensorStatus},
                                        {TM_STOP,RESTART_YES,REPORT_BETWEEN_SENSORS,0,nextReportStatus},
                                        {TM_START,RESTART_YES,100,0,sekundenTimer},
                                        {TM_STOP,RESTART_NO,100,0,NULL},		// Timeout-Timer
                                        {TM_START,RESTART_YES,6000,0,updateDisplay},		  // Update-Timer
                                        {TM_START,RESTART_NO,20,0,displayReady},		  // Ready-Display-Timer
                                        {TM_START,RESTART_YES,4000,0,updateClimate}		  // Local-Clima-Timer
};



//float fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;

double fExternalTemperature = -99.0;
double fExternalHumidity    = -99.0;
double fExternalPressure    = -99.0;
double fExternalDewPoint    = -99.0;
double fInternalTemperature = -99.0;
double fInternalHumidity    = -99.0;
double fInternalDewPoint    = -99.0;
double fInternalAbsHumitdity= -99.0;
char   heaterAlarm[5]="non";
char   heaterWater[5]="non";
bool heaterCollectionAlarm=false;
double   MqttTime= 1111111111;
uint32_t secondsCounter = 1545264000;
uint8_t gotEmailNumber = 0;
uint8_t windowOpen = 0;

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

#ifdef USE_FUNK
SPI_Master_t spiRFM69;
RFM69 myRFM(&MyTimers[TIMER_RFM69],&spiRFM69,true);
// Wrapper, um eine Klassenfunktion für den ComReceiver nutzen zu können

void sendRFMRelay(char *test)
{
  myRFM.sendRelay(test);
}

#endif // USE_FUNK




Communication cmulti(0,Node,5,USE_BUSY_0);

#ifdef USE_FUNK
ComReceiver cmultiRec( &cmulti,Node,cnetCommands,NUM_COMMANDS,information,NUM_INFORMATION,"R1R2R3",sendRFMRelay );
#else
ComReceiver cmultiRec( &cmulti,Node,cnetCommands,NUM_COMMANDS,information,NUM_INFORMATION,"",NULL );
#endif // USE_FUNK

SPI_Master_t spiDisplay;


SHT2 localClima(&twiC_Master,0x40);
