#ifndef LOCALECLIMATE_H_INCLUDED
#define LOCALECLIMATE_H_INCLUDED

#include "External.h"
#include "sht2x.h"
#include "Communication.h"
#include "ledHardware.h"
#include "RFM69.h"

void measureClimate();

enum{CLIMATE_SLEEP=0,CLIMATE_WAIT_T,CLIMATE_CALC_T,CLIMATE_SEND_T,CLIMATE_START_H,CLIMATE_WAIT_H,CLIMATE_CALC_H,CLIMATE_SEND_H,CLIMATE_CALC_D,CLIMATE_SEND_D};


#endif // LOCALECLIMATE_H_INCLUDED
