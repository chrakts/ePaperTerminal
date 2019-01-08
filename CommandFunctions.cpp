/*
 * CommandFunctions.cpp
 *
 * Created: 26.04.2017 14:54:45
 *  Author: a16007
 */

#include "CommandFunctions.h"
#include "External.h"
#include "CRC_Calc.h"

void jobGotCRCError(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswer(fehler_text[CRC_ERROR],quelle_KNET,function,address,job,false);
}

void jobGetCTemperatureSensor(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswerDouble(quelle_KNET,function,address,job,(double)fTemperatur,true);
}

void jobGetCHumiditySensor(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswerDouble(quelle_KNET,function,address,job,(double)fHumidity,true);
}

void jobGetCAbsHumiditySensor(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswerDouble(quelle_KNET,function,address,job,(double)fAbsHumitdity,true);
}

void jobGetCDewPointSensor(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswerDouble(quelle_KNET,function,address,job,(double)fDewPoint,true);
}

void jobSetSecurityKey(Communication *output, char function,char address,char job, void * pMem)
{
uint8_t ret = true;
	if (strcmp((char *)pMem,"Phe6%!kdf?+2aQ")==0)
	{
		SecurityLevel = PRODUCTION;
	}
	else if(strcmp((char *)pMem,"D=&27ane%24dez")==0)
	{
		SecurityLevel = DEVELOPMENT;
	}
	else
	{
		SecurityLevel = CUSTOMER;
		ret = false;
	}
	output->sendAnswerInt(quelle_KNET,function,address,job,SecurityLevel,ret);
}

void jobGetSecurityKey(Communication *output, char function,char address,char job, void * pMem)
{
	output->sendAnswerInt(quelle_KNET,function,address,job,SecurityLevel,true);
}


void jobGetCompilationDate(Communication *output, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Date);
	output->sendAnswer(temp,quelle_KNET,function,address,job,true);
}

void jobGetCompilationTime(Communication *output, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Time);
	output->sendAnswer(temp,quelle_KNET,function,address,job,true);
}

void jobGetFreeMemory(Communication *output, char function,char address,char job, void * pMem)
{
extern int __heap_start, *__brkval;
int v;

	uint16_t mem = (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
	output->sendAnswerInt(quelle_KNET,function,address,job,mem,true);
}



void jobSetMeasureRate(Communication *output, char function,char address,char job, void * pMem)
{
	uint16_t rate;
	rate = ((uint16_t *)pMem)[0];
	if (rate<10)
	{
        output->sendAnswer("Rate [100ms] must not smaller than 10",quelle_KNET,function,address,job,false);
	}
	else
	{
		measureRate_100ms = rate;
		output->sendPureAnswer(quelle_KNET,function,address,job,true);
	}
}

void jobGetMeasureRate(Communication *output, char function,char address,char job, void * pMem)
{
    output->sendAnswerInt(quelle_KNET,function,address,job,measureRate_100ms,true);
}

void jobSetAverageRate(Communication *output, char function,char address,char job, void * pMem)
{
	uint8_t rate;
	rate = ((uint8_t *)pMem)[0];
	if ( 1 )
	{
	}
	else
		output->sendAnswer(fehler_text[NO_ACTIVE_SENSOR],quelle_KNET,function,address,job,false);
}

