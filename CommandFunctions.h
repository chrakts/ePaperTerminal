/*
 * CommandFunctions.h
 *
 * Created: 26.04.2017 14:55:18
 *  Author: a16007
 */


#ifndef COMMANDFUNCTIONS_H_
#define COMMANDFUNCTIONS_H_

#include "ePaperTerminal.h"

#define NUM_COMMANDS 10+7
#define NUM_INFORMATION 8

extern COMMAND cnetCommands[];
extern INFORMATION information[];

void jobGotCRCError(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCTemperatureSensor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCHumiditySensor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCAbsHumiditySensor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCDewPointSensor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCompilationDate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCompilationTime(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetFreeMemory(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetMeasureRate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetMeasureRate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetAverageRate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetAverageRate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGotExternalTemperature(ComReceiver *comRec, char function,char address,char job, void * pMem);

void gotNewMqttTime();
void gotHeaterAlarmInfo();

#endif /* COMMANDFUNCTIONS_H_ */
