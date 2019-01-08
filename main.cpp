/*
 */

//#include <SPI.h>
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


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  LEDROTSETUP;
  PORTA.DIRSET = PIN4_bm | PIN7_bm;
  PORTB.DIRSET = PIN2_bm | PIN5_bm | PIN7_bm;
  PORTC.DIRSET = PIN1_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTD.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTE.DIRSET = PIN1_bm | PIN4_bm | PIN5_bm | PIN7_bm;
  PORTF.DIRSET = PIN3_bm | PIN4_bm | PIN5_bm;
  init_clock(SYSCLK, PLL);
	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
  sei();
  cmulti.open(Serial::BAUD_57600,F_CPU);
  cmulti.print("Hallo Display\n");
  LEDROT_ON;
  Epd epd;

  if (epd.Init() != 0) {
    //Serial.print("e-Paper init failed");
    return;
  }
  cmulti.print("EPD init\n");

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  unsigned char image[15000];
  Paint paint(image, 400, 300);    //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  cmulti.print("Display Cleared\n");
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
  int i;
  paint.DrawFilledCircle(200, 150, 34, COLORED);
  paint.DrawFilledCircle(220, 170, 30, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

//  paint.Clear(UNCOLORED);
//  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

//  paint.Clear(UNCOLORED);
//  paint.DrawFilledCircle(32, 32, 30, COLORED);
//  epd.SetPartialWindow(paint.GetImage(), 400, 300, paint.GetWidth(), paint.GetHeight());
  cmulti.print("EPD partial3\n");

  /* This displays the data from the SRAM in e-Paper module */
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  cmulti.print("Display Frame\n");

  /* This displays an image */
  paint.Clear(UNCOLORED);
  paint.DrawPicture(imageButterfly,IMAGEBUTTERFLY_SIZE);
  cmulti.print("Transfer Picture\n");
  epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  cmulti.print("Display Picture\n");
  /* Deep sleep */
  epd.Sleep();
}

int main()
{
  setup();
  // put your main code here, to run repeatedly:
  while(1)
  {
    comStateMachine(&cmulti);
    doJob(&cmulti);
  }
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
