/*
 */

//#include <SPI.h>
#include "ePaperTerminal.h"
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "ledHardware.h"
#include "External.h"

#define COLORED     0
#define UNCOLORED   1

enum{QUARZ,CLK2M,CLK32M};

#define SYSCLK QUARZ
#define PLL 2

void init_clock(int sysclk, int pll);


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
  init_clock(SYSCLK, PLL);
  for( uint8_t i=0;i<3;i++)
  {
    LEDROT_ON;
    _delay_ms(300);
    LEDROT_OFF;
    _delay_ms(300);
  }

  initReadMonitor();
  initBusyCounter();

  SPI_MasterInit(&spiDisplay,&SPI_DEV,&SPI_PORT,false,SPI_MODE_0_gc,SPI_INTLVL_LO_gc,false,SPI_PRESCALER_DIV128_gc);

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
  sei();
  init_mytimer();
  cmulti.open(Serial::BAUD_57600,F_CPU);
  LEDROT_OFF;
  cmulti.sendInfo("Epaper ist da!","BR");
  initDisplay(&spiDisplay);//#################################################### muss wieder rein
  cmulti.sendInfo("Epaper ist immer","BR");

  //SHT2x_SoftReset();
/*
  Epd epd;

  if (epd.Init(&spiDisplay) != 0) {
    cmulti.print("e-Paper init failed\n");
    return;
  }
  cmulti.print("EPD init\n");

  epd.ClearFrame();
  cmulti.print("EPD clear\n");
  unsigned char image[7500];
  Paint paint(image, 400, 150);    //width should be the multiple of 8
  cmulti.print("EPD paint image \n");
  paint.Clear(UNCOLORED);
  cmulti.print("Display Cleared \n");
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
  cmulti.print("EPD partial\n");

//  paint.Clear(COLORED);
  paint.DrawStringAt(48, 48, "Hello world", &Font24, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

//  paint.SetWidth(64);
//  paint.SetHeight(64);

//  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 299, 100, 200, COLORED);
  paint.DrawLine(50, 50, 90, 100, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());
  cmulti.print("EPD partial2\n");

//  paint.SetWidth(100);
//  paint.SetHeight(100);

//  paint.Clear(UNCOLORED);
//  int i;
//  paint.DrawFilledCircle(200, 150, 34, COLORED);
//  paint.DrawFilledCircle(220, 170, 30, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

//  paint.Clear(UNCOLORED);
//  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

//  paint.Clear(UNCOLORED);
//  paint.DrawFilledCircle(32, 32, 30, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 400, 300, paint.GetWidth(), paint.GetHeight());
  cmulti.print("EPD partial3\n");

  // This displays the data from the SRAM in e-Paper module
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  //epd.DisplayFrame();
  cmulti.print("Display Frame\n");
  epd.SetPartialWindow(paint.GetImage(), 0, 150, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  cmulti.print("Display Frame2\n");

  // This displays an image
  //paint.Clear(UNCOLORED);
  //paint.DrawPicture(imageButterfly,IMAGEBUTTERFLY_SIZE);
  //cmulti.print("Transfer Picture\n");
  //epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  //epd.DisplayFrame();
  //cmulti.print("Display Picture\n");
  // Deep sleep
  epd.Sleep();*/

}

int main()
{
  setup();
  // put your main code here, to run repeatedly:
  nowUpdateDisplay = true;
  cmulti.sendInfo("Epaper will shown","BR");
  uint8_t test;
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
    test = showDisplay();
    comStateMachine(&cmulti);
    doJob(&cmulti);
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




void init_clock(int sysclk, int pll)
{
	CLK_t *mein_clock;
	OSC_t *mein_osc;
	mein_clock = &CLK;
	mein_osc = &OSC;
	switch(sysclk)
	{
		case QUARZ:
			mein_osc->XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
//			mein_osc->XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCPWR_bm | OSC_XOSCSEL_XTAL_16KCLK_gc;
			mein_osc->CTRL = OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 32 MHz-Clock ein

			while((mein_osc->STATUS & OSC_XOSCRDY_bm) == 0)			// wartet bis diese stabil
			;
			while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)		// wartet bis diese stabil
			;

			if ( (pll>0) & (pll<16) )
			{
				mein_osc->PLLCTRL = OSC_PLLSRC_XOSC_gc | pll;
				mein_osc->CTRL = OSC_PLLEN_bm | OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet zusätzlich die PLL ein

				while((mein_osc->STATUS & OSC_PLLRDY_bm) == 0)		// wartet bis diese stabil
				;
				CCP = CCP_IOREG_gc;										// geschuetztes Register freigeben
				mein_clock->CTRL = CLK_SCLKSEL_PLL_gc;					// umschalten auf PLL-Clock
				mein_osc->CTRL = OSC_PLLEN_bm | OSC_XOSCEN_bm | OSC_RC32KEN_bm;
			}
			else
			{
				CCP = CCP_IOREG_gc;										// geschuetztes Register freigeben
				mein_clock->CTRL = CLK_SCLKSEL_XOSC_gc;					// umschalten auf XOSC-Clock
				mein_osc->CTRL = OSC_XOSCEN_bm | OSC_RC32KEN_bm;
			}
		break; // QUARZ
		case CLK2M:
			mein_osc->CTRL = OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 2 MHz-Clock ein
			while((mein_osc->STATUS & OSC_RC2MRDY_bm) == 0)  // wartet bis diese stabil
			;
			while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)  // wartet bis diese stabil
			;
			CCP = CCP_IOREG_gc;								// geschuetztes Register freigeben
			mein_clock->CTRL = CLK_SCLKSEL_RC2M_gc;		// umschalten auf 2 MHz-Clock
//			CLKSYS_AutoCalibration_Enable(OSC_RC2MCREF_RC32K_gc,false); // OSC_RC32MCREF_bm
		break;
		case CLK32M:
			mein_osc->CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 32 MHz-Clock ein
			while((mein_osc->STATUS & OSC_RC32MRDY_bm) == 0)  // wartet bis diese stabil
			;
			while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)  // wartet bis diese stabil
			;
			CCP = CCP_IOREG_gc;								// geschuetztes Register freigeben
			mein_clock->CTRL = CLK_SCLKSEL_RC32M_gc;		// umschalten auf 32 MHz-Clock
			mein_osc->CTRL = OSC_RC32MEN_bm | OSC_RC32KEN_bm;		// abschalten der 2 MHz-Clock
//			CLKSYS_AutoCalibration_Enable(OSC_RC32MCREF_RC32K_gc,false); // OSC_RC32MCREF_bm
		break;
	}
}
