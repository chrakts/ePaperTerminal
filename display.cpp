#include "epdpaint.h"
#include "display.h"
#include "ledHardware.h"
#include "time.h"
#define COLORED     0
#define UNCOLORED   1

Epd epd;
//unsigned char globalImage[15000]; #################################################### muss wieder rein
unsigned char globalImage[5000];
Paint paint(globalImage, 400, 100);
uint8_t statusDisplay=DISPLAY_SLEEP;
const char *Wochentage[]={"Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag"};
uint8_t showDisplay()
{
uint8_t temp;
char infoString[30];
char infoString2[30];
  if( nowUpdateDisplay )
  {
    switch(statusDisplay)
    {
      case DISPLAY_SLEEP:
        temp = epd.ResetStep();
        if(temp<1)
          statusDisplay = DISPLAY_CLEAR;
        // hier noch die Aufwachroutine einbauen
      break;
      case DISPLAY_CLEAR:
        temp = epd.ClearFrameStep();
        if(temp<1)
          statusDisplay = DISPLAY_PREPARE;
      break;
      case DISPLAY_PREPARE:
        paint.ClearFast();
        //paint.Clear(UNCOLORED);
        statusDisplay = DISPLAY_SETUP1;
      break;
      case DISPLAY_SETUP1:

        // Erstes Drittel

        paint.DrawStringAt(10, 5, "Draussen", &Font24, COLORED);

        sprintf(infoString,"%.2f`C",fExternalTemperature);
        paint.DrawStringAt(10, 35, infoString, &Grotesk48, COLORED);

        sprintf(infoString,"%.1f%%rH",fExternalHumidity);
        paint.DrawStringAt(220, 5, infoString, &Font24, COLORED);

        sprintf(infoString,"%.1fmbar",fExternalPressure);
        paint.DrawStringAt(220, 32, infoString, &Font24, COLORED);

        sprintf(infoString,"%.1f`C",fExternalDewPoint);
        paint.DrawStringAt(220, 59, infoString, &Font24, COLORED);
        epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
        statusDisplay = DISPLAY_SETUP2;
      break;
      case DISPLAY_SETUP2:
        // Zweites Drittel

        paint.ClearFast();

        paint.DrawStringAt(10, 5, "Drinnen", &Font24, COLORED);

        sprintf(infoString,"%.2f`C",fInternalTemperature);
        paint.DrawStringAt(10, 35, infoString, &Grotesk48, COLORED);

        epd.SetPartialWindow(paint.GetImage(), 0, 100, paint.GetWidth(), paint.GetHeight());
        statusDisplay = DISPLAY_SETUP3;
      break;
      case DISPLAY_SETUP3:  // Drittes Drittel
        paint.ClearFast();
        time_t lokalTime;
        cli();
        lokalTime = (time_t)secondsCounter;
        sei();
        struct tm info;
        localtime_r( &lokalTime,&info );

        strftime(infoString,20,"%H:%M", &info);
        paint.DrawStringAt(10, 35, infoString, &Grotesk48, COLORED);
        strftime(infoString,20,"%w", &info);
        strftime(infoString2,20,"%d.%m.%y", &info);
        sprintf(infoString,"%s, %s",Wochentage[infoString[0]-48],infoString2);
        paint.DrawStringAt(10, 5, infoString, &Font24, COLORED);
        paint.DrawPicture(&heizung,224,35);
        paint.DrawPicture(&telegram,224+48+8,35);
        // fehlt noch die Möglichkeit kleine Bilder zu printen
        //sprintf(infoString,"%" PRIu32 ,secondsCounter);
        //paint.DrawStringAt(220, 59, infoString, &Font24, COLORED);
        LEDROT_ON;
        epd.SetPartialWindow(paint.GetImage(), 0, 200, paint.GetWidth(), paint.GetHeight());
        LEDROT_OFF;
        statusDisplay = DISPLAY_SHOW;
      break;
      case DISPLAY_SHOW:
/*        temp = epd.DisplayFrameStep(paint.GetImage());
        if(temp<1)
          statusDisplay = DISPLAY_GOTO_SLEEP;*/
         //epd.SetPartialWindow(paint.GetImage(), 0, 100, paint.GetWidth(), paint.GetHeight());
         //epd.SetPartialWindow(paint.GetImage(), 0, 200, paint.GetWidth(), paint.GetHeight());
         //epd.DisplayFrame();
        temp = epd.DisplayFrameStep();
        if(temp<1)
          statusDisplay = DISPLAY_GOTO_SLEEP;
      break;
      case DISPLAY_GOTO_SLEEP:

        nowUpdateDisplay = false;
        statusDisplay = DISPLAY_SLEEP;
      break;
      default:
        statusDisplay = DISPLAY_SLEEP;
    }
  }
//  return(temp);
  return(statusDisplay);
}

void initDisplay(SPI_Master_t *spi)
{

  if (epd.Init(spi) != 0) {
    //Serial.print("e-Paper init failed");
    return;
  }

}
