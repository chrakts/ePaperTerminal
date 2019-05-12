#include "localeClimate.h"

uint8_t measureClimate()
{
static uint8_t statusClimate=CLIMATE_SLEEP;
static int16_t raw;
char wert[10];
  if( nowUpdateClima )
  {
    switch(statusClimate)
    {
      case CLIMATE_SLEEP:
        LEDROT_ON;
        localClima.startMeasurementPoll(SHT2::etSHT2xMeasureType::TEMP);
        statusClimate++;
      break;
      case CLIMATE_WAIT_T:
        if(localClima.getMeasurementPoll(&raw)!=0)
        {
          statusClimate++;
        }
      break;
      case CLIMATE_CALC_T:
        fInternalTemperature = localClima.CalcTemperatureC(raw);
        statusClimate++;
      break;
      case CLIMATE_SEND_T:
        sprintf(wert,"%.4f",fInternalTemperature);
        cmulti.sendStandard(wert,BROADCAST,'C','1','t','F');
        statusClimate++;
      break;
      case CLIMATE_START_H:
        localClima.startMeasurementPoll(SHT2::etSHT2xMeasureType::HUMIDITY);
        statusClimate++;
      break;
      case CLIMATE_WAIT_H:
        if(localClima.getMeasurementPoll(&raw)!=0)
        {
          statusClimate++;
        }
      break;
      case CLIMATE_CALC_H:
        LEDROT_OFF;
        fInternalHumidity = localClima.CalcRH(raw);
        statusClimate++;
      break;
      case CLIMATE_SEND_H:
        sprintf(wert,"%.4f",fInternalHumidity);
        cmulti.sendStandard(wert,BROADCAST,'C','1','h','F');
        statusClimate++;
      break;
      case CLIMATE_CALC_D:
        float k,dew_point ;
        k = (log10(fInternalHumidity)-2)/0.4343 + (17.62*fInternalTemperature)/(243.12+fInternalTemperature);
        fInternalDewPoint = 243.12*k/(17.62-k);
        statusClimate++;
      break;
      case CLIMATE_SEND_D:
        sprintf(wert,"%.4f",fInternalDewPoint);
        cmulti.sendStandard(wert,BROADCAST,'C','1','d','F');
        nowUpdateClima = false;
        statusClimate = CLIMATE_SLEEP;
      break;

    }
  }
}
