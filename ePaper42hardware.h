#ifndef EPAPER42HARDWARE_H_INCLUDED
#define EPAPER42HARDWARE_H_INCLUDED

#include <avr/io.h>

#define RST_PORT        PORTE
#define DC_PORT         RST_PORT
#define CS_PORT         RST_PORT
#define BUSY_PORT       RST_PORT

#define RST_PIN         PIN1_bm
#define DC_PIN          PIN3_bm
#define CS_PIN          PIN4_bm
#define BUSY_PIN        PIN2_bm

#endif // EPAPER42HARDWARE_H_INCLUDED
