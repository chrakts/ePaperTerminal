/*
 * TLog.h
 *
 * Created: 03.04.2017 21:12:07
 *  Author: Christof
 */


#ifndef EPAPERTERMINAL_H_
#define EPAPERTERMINAL_H_

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
#include "Communication.h"
#include "sht2x.h"


#endif /* EPAPERTERMINAL_H_ */
