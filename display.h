#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "External.h"
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"


uint8_t showDisplay();
void initDisplay(SPI_Master_t *spi);


enum{DISPLAY_SLEEP=0,DISPLAY_CLEAR,DISPLAY_PREPARE ,DISPLAY_SETUP,DISPLAY_SHOW,DISPLAY_GOTO_SLEEP};

#endif // DISPLAY_H_INCLUDED
