/*
 * CommandFunctions.cpp
 *
 * Created: 26.04.2017 14:54:45
 *  Author: a16007
 */

#include "CommandFunctions.h"
#include "External.h"
#include "../Secrets/secrets.h"
#include "ledHardware.h"
#include "../ComReceiver/cmultiStandardCommands.h"

COMMAND cnetCommands[NUM_COMMANDS] =
{
    cmultiStandardCommands,
    {'C','t',CUSTOMER,NOPARAMETER,0,jobGetCTemperatureSensor},
    {'C','h',CUSTOMER,NOPARAMETER,0,jobGetCHumiditySensor},
    {'C','d',CUSTOMER,NOPARAMETER,0,jobGetCDewPointSensor},
    {'C','a',CUSTOMER,NOPARAMETER,0,jobGetCAbsHumiditySensor},
    {'M','r',PRODUCTION,NOPARAMETER,16,jobGetMeasureRate},
    {'M','R',PRODUCTION,UINT_16,16,jobSetMeasureRate}
};

INFORMATION information[NUM_INFORMATION]=
{
  {"CQ",'C','1','t',FLOAT,1,(void*)&fExternalTemperature,NULL},
  {"CQ",'C','1','h',FLOAT,1,(void*)&fExternalHumidity,NULL},
  {"C1",'C','1','p',FLOAT,1,(void*)&fExternalPressure,NULL},
  {"CQ",'C','1','d',FLOAT,1,(void*)&fExternalDewPoint,NULL},
  {"DT",'e','c','n',UINT_8,1,(void*)&gotEmailNumber,NULL},
  {"DT",'t','1','s',FLOAT,1,(void*)&MqttTime,gotNewMqttTime},
  {"H1",'H','1','a',STRING,5,(void*)&heaterAlarm,gotHeaterAlarmInfo},
  {"H1",'H','1','w',STRING,5,(void*)&heaterWater,gotHeaterAlarmInfo},
  {"DT",'F','K','o',UINT_8,1,(void*)&windowOpen,NULL}
};

void jobGetCTemperatureSensor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,(double)fInternalTemperature,true);
}

void jobGetCHumiditySensor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,(double)fInternalHumidity,true);
}

void jobGetCAbsHumiditySensor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,(double)fInternalAbsHumitdity,true);
}

void jobGetCDewPointSensor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,(double)fInternalDewPoint,true);
}

void jobSetMeasureRate(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	uint16_t rate;
	rate = ((uint16_t *)pMem)[0];
	if (rate<10)
	{
        comRec->sendAnswer("Rate [100ms] must not smaller than 10",function,address,job,false);
	}
	else
	{
		measureRate_100ms = rate;
		comRec->sendPureAnswer(function,address,job,true);
	}
}

void jobGetMeasureRate(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
    comRec->sendAnswerInt(function,address,job,measureRate_100ms,true);
}

void jobSetAverageRate(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	uint8_t rate;
	rate = ((uint8_t *)pMem)[0];
	if ( 1 )
	{
	}
	else
		comRec->sendAnswer(fehler_text[NO_ACTIVE_SENSOR],function,address,job,false);
}

/* "2019-04-18-06-36-02" */
// https://stackoverflow.com/questions/5754315/c-convert-char-to-timestamp/5754417#5754417
// https://stackoverflow.com/questions/1859201/add-seconds-to-a-date
void gotNewMqttTime()
{
  cli();
  secondsCounter = uint32_t(MqttTime);
  sei();
}

void gotHeaterAlarmInfo()
{
  if((strcmp(heaterAlarm,"ON")==0) | (strcmp(heaterWater,"ON")==0))
    heaterCollectionAlarm = true;
  else
    heaterCollectionAlarm = false;
}

