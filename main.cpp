/*
 */

//#include <SPI.h>
#include "ePaperTerminal.h"
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "ledHardware.h"
#include "External.h"
#include "sht2x.h"
#include "localeClimate.h"
#include "xmegaClocks.h"


#define COLORED     0
#define UNCOLORED   1


#define SYSCLK QUARZ
#define PLL 2

void setup()
{
  LEDROTSETUP;
  PORTA.DIRSET = 0xff; //PIN4_bm | PIN7_bm;
  PORTB.DIRSET = PIN2_bm | PIN5_bm | PIN7_bm;
  PORTC.DIRSET = PIN1_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTD.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTE.DIRSET = PIN1_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTF.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm;
  AUX1_ON;
  AUX2_ON;
  AUX3_ON;
  AUX4_ON;
  init_clock(SYSCLK,PLL,false,0);

  SPI_MasterInit(&spiDisplay,&SPI_DEV,&SPI_PORT,false,SPI_MODE_0_gc,SPI_INTLVL_LO_gc,false,SPI_PRESCALER_DIV128_gc);

  TWI_MasterInit(&twiC_Master, &TWIC, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);
#ifdef USE_FUNK
  SPI_MasterInit(&spiRFM69,&(SPID),&(PORTD),false,SPI_MODE_0_gc,SPI_INTLVL_LO_gc,false,SPI_PRESCALER_DIV128_gc);
#endif // USE_FUNK
	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
  sei();
  init_mytimer();
  cmulti.open(Serial::BAUD_57600,F_CPU);
  LEDROT_OFF;
  uint8_t serialNumber[8];
  char serialText[20],temp[5];
  cmulti.sendInfo("Epaper ist da!","BR");
  serialText[0]='\000';
  localClima.getSerialNumber(serialNumber);

  for(uint8_t i=0;i<8;i++)
  {
    sprintf(temp, "%" PRIx8, serialNumber[i]);
    strcat( serialText,temp );
  }
  cmulti.sendInfo(serialText,"BR");

  LEDROT_ON;
  localClima.startMeasurementPoll(SHT2::etSHT2xMeasureType::TEMP);
  int16_t temperature;
  while(localClima.getMeasurementPoll(&temperature)==0)
  {
  }
  LEDROT_OFF;
  fInternalTemperature = localClima.CalcTemperatureC(temperature);
  localClima.startMeasurementPoll(SHT2::etSHT2xMeasureType::HUMIDITY);
  int16_t humidity;
  while(localClima.getMeasurementPoll(&humidity)==0)
  {
  }
  LEDROT_OFF;
  fInternalHumidity = localClima.CalcRH(humidity);

  initDisplay(&spiDisplay);//#################################################### muss wieder rein

#ifdef USE_FUNK
  myRFM.initialize(RF69_868MHZ,RFM69Node,RFM69Network);
  myRFM.encrypt(RFM69Key);
  //myRFM.readAllRegsCompact();
#endif // USE_FUNK

}

int main()
{

  setup();
  // put your main code here, to run repeatedly:
  nowUpdateDisplay = true;
  cmulti.sendInfo("Epaper will shown","BR");
  uint32_t bremse=0;
  while(1)
  {
    if(bremse>=300000)
    {
      //cmulti.sendAnswerInt("BR",'+','+','+',test,true);
      bremse = 0;
    }
    bremse++;
/*    if(nowUpdateDisplay==true)
    {
      if(statusDisplay==DISPLAY_CLEAR)
        cmulti.sendInfo("Epaper update clear","BR");
    }*/
    showDisplay();
    measureClimate();
    cmultiRec.comStateMachine();
    cmultiRec.doJob();

#ifdef USE_FUNK

    if(myRFM.isDataFromRelayAvailable())
    {
      LEDROT_ON;
      myRFM.processRelay();
    }
    else
    {
      LEDROT_OFF;
      if (myRFM.receiveDone())
      {
        LEDROT_ON;
        //debug.pformat("[%d]-[%d]-[RX_RSSI: %d]:",myRFM.SENDERID,myRFM.DATALEN,myRFM.RSSI);
        //debug.print((char *)myRFM.DATA);
        //cmulti.broadcastUInt8(myRFM.SENDERID,'x','x','x');

        if (myRFM.ACKRequested())
        {
          myRFM.sendACK();
          //debug.print(" - ACK sent\n");
        }
        cmulti.broadcastInt16(myRFM.RSSI,'R','S','I');
        //"BRL1SJF1TKaffee"
        char target[3],source[3];
        char job,function,address,dataType;
        target[0]=((char *)myRFM.DATA)[0];
        target[1]=((char *)myRFM.DATA)[1];
        target[2]=0;
        source[0]=((char *)myRFM.DATA)[2];
        source[1]=((char *)myRFM.DATA)[3];
        source[2]=0;
        function = ((char *)myRFM.DATA)[5];
        address = ((char *)myRFM.DATA)[6];
        job = ((char *)myRFM.DATA)[7];
        dataType = ((char *)myRFM.DATA)[8];
        cmulti.setAlternativeNode(source);
        cmulti.sendStandard(&(((char *)myRFM.DATA)[9]),target,function,address,job,dataType);
        cmulti.resetNode();
        LEDROT_OFF;
      }

    }
    if(myRFM.getDebugFlag()>0)
    {
      cmulti.broadcastUInt8(myRFM.getDebugFlag(),'F','d','s');
      myRFM.setDebugFlag(0);
    }
#endif // USE_FUNK

  }
}

/*! \brief SPI master interrupt service routine.
 *
 *  The interrupt service routines calls one common function,
 *  SPI_MasterInterruptHandler(SPI_Master_t *spi),
 *  passing information about what module to handle.
 *
 *  Similar ISRs must be added if other SPI modules are to be used.
 */
ISR(SPIE_INT_vect)
{
	SPI_MasterInterruptHandler(&spiDisplay);
}
