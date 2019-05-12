#define IMAGEOIL_TEMPERATURE_SIZE 237
#include "imagedata.h"
#include <avr/pgmspace.h>
#include "epdpaint.h"

PICTURECOMP heizung = {48,48,imageOil_temperature,IMAGEOIL_TEMPERATURE_SIZE};

const unsigned char imageOil_temperature[IMAGEOIL_TEMPERATURE_SIZE] PROGMEM = {
0xff,0x0d,0xf8,0x7f,0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x3f,
0xff,0x03,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,
0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x3f,0xff,0x03,0xf0,0x3f,
0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x00,0x00,0x3f,0xff,0x02,
0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,
0xff,0x02,0xf0,0x3f,0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x3f,0xff,0x03,0xf0,0x3f,
0xff,0x03,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x00,
0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xf0,0x3f,0xff,0x03,0xf0,0x3f,
0xff,0x03,0xf0,0x3f,0xff,0x03,0xe0,0x1f,0xff,0x01,0x3f,0xff,0x00,0xe0,0x1f,0x3f,
0xfc,0x0f,0xf3,0xc0,0x0f,0x0f,0xf0,0x07,0xe3,0xc0,0x0f,0x07,0xe0,0x03,0xc3,0xc0,
0x0f,0x03,0xc0,0xc0,0x03,0xc0,0x0f,0xc0,0x03,0xe0,0x07,0xe0,0x1f,0xe0,0x07,0xf0,
0x0f,0xe0,0x1f,0xf0,0x0f,0xfc,0x3f,0xf8,0x7f,0xfc,0x3f,0xff,0x0c,0xfc,0x3f,0xfc,
0x3f,0xff,0x00,0xcf,0xf0,0x0f,0xf0,0x0f,0xf3,0xc7,0xe0,0x07,0xe0,0x07,0xe3,0xc3,
0xc0,0x03,0xc0,0x03,0xc3,0xc0,0x03,0xc0,0x03,0xc0,0x03,0xe0,0x07,0xe0,0x07,0xe0,
0x07,0xf0,0x0f,0xf0,0x0f,0xf0,0x0f,0xfc,0x3f,0xfc,0x3f,0xfc,0x3f,};

#define IMAGETELEGRAM_SIZE 181

const unsigned char imageTelegram[IMAGETELEGRAM_SIZE] PROGMEM = {
0xff,0x39,0xfe,0x3f,0xff,0x03,0xf0,0x3f,0xff,0x03,0x80,0x3f,0xff,0x02,0xfe,0x00,
0x00,0x3f,0xff,0x02,0xf0,0x00,0x00,0x3f,0xff,0x02,0xc0,0x00,0x00,0x7f,0xff,0x01,
0xfe,0x00,0x00,0x60,0x7f,0xff,0x01,0xf0,0x00,0x00,0xc0,0x7f,0xff,0x01,0xc0,0x03,
0x80,0x7f,0xff,0x00,0xfe,0x00,0x00,0x0f,0x00,0x00,0xff,0x01,0xf8,0x00,0x00,0x1e,
0x00,0x00,0xff,0x01,0xc0,0x00,0x00,0x78,0x00,0x00,0xff,0x01,0x00,0x01,0xf0,0x00,
0x00,0xff,0x00,0xfc,0x00,0x00,0x03,0xe0,0x00,0x00,0xff,0x00,0xfc,0x00,0x00,0x0f,
0xc0,0x01,0xff,0x01,0x80,0x1f,0x80,0x01,0xff,0x01,0xf0,0x7f,0x00,0x00,0x01,0xff,
0x02,0xfe,0x00,0x00,0x01,0xff,0x02,0xfc,0x00,0x00,0x01,0xff,0x02,0xf8,0x00,0x00,
0x03,0xff,0x02,0xf0,0x00,0x00,0x03,0xff,0x02,0xf0,0x00,0x00,0x03,0xff,0x02,0xf0,
0x00,0x00,0x03,0xff,0x02,0xf0,0x00,0x00,0x03,0xff,0x02,0xf0,0xc0,0x07,0xff,0x02,
0xf1,0xf0,0x07,0xff,0x02,0xf3,0xf8,0x07,0xff,0x02,0xf7,0xfc,0x07,0xff,0x04,0x0f,
0xff,0x04,0x8f,0xff,0x36,};

PICTURECOMP telegram = {48,48,imageTelegram,IMAGETELEGRAM_SIZE};


#define IMAGEEMAIL_SIZE 170

const unsigned char imageEmail[IMAGEEMAIL_SIZE] PROGMEM = {
0xff,0x2f,0xfc,0x00,0x03,0x3f,0xf8,0x00,0x03,0x1f,0xf0,0x00,0x03,0x0f,0xf0,0x00,
0x03,0x0f,0xf0,0x80,0x00,0x01,0x01,0x0f,0xf0,0xc0,0x00,0x01,0x03,0x0f,0xf0,0xf0,
0x00,0x01,0x0f,0x0f,0xf0,0xfc,0x00,0x01,0x3f,0x0f,0xf0,0x7e,0x00,0x01,0x7e,0x0f,
0xf0,0x3f,0x80,0x01,0xfc,0x0f,0xf0,0x0f,0xc0,0x03,0xf0,0x0f,0xf0,0x03,0xf0,0x0f,
0xc0,0x0f,0xf0,0x01,0xfc,0x3f,0x80,0x0f,0xf0,0x00,0x00,0x7e,0x7e,0x00,0x00,0x0f,
0xf0,0x00,0x00,0x3f,0xfc,0x00,0x00,0x0f,0xf0,0x00,0x00,0x0f,0xf0,0x00,0x00,0x0f,
0xf0,0x00,0x00,0x03,0xc0,0x00,0x00,0x0f,0xf0,0x00,0x00,0x01,0x80,0x00,0x00,0x0f,
0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,
0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,
0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,0xf0,0x00,0x03,0x0f,
0xf8,0x00,0x03,0x1f,0xfc,0x00,0x03,0x3f,0xff,0x2f,};

PICTURECOMP email = {48,48,imageEmail,IMAGEEMAIL_SIZE};
