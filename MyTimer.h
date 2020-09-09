/*
 * MyTimer.h
 *
 * Created: 11.02.2016 20:20:03
 *  Author: Christof
 */


#ifndef MYTIMER_H_
#define MYTIMER_H_

#define MYTIMER_NUM	8

enum{TM_START,TM_RESET,TM_STOP,TM_RUN};
enum{RESTART_YES,RESTART_NO};

enum{TIMER_RFM69,TIMER_SENSOREN,TIMER_REPORT,TIMER_SEKUNDE,TIMER_TIMEOUT,UPDATE_TIMER,DISPLAY_READY_TIMER,LOCAL_CLIMA_TIMER};

struct Timer
{
	uint8_t		state;
	uint8_t		restart;
	uint16_t	value;
	uint16_t	actual;
	void  (*OnReady)  (uint8_t test);
};

typedef struct Timer TIMER;

void nextSensorStatus(uint8_t test);
void nextReportStatus(uint8_t test);
void sekundenTimer(uint8_t test);
void LED_toggle(uint8_t test);
void init_mytimer(void);
void goto_sleep(uint8_t test);
void updateDisplay(uint8_t test);
void updateClimate(uint8_t test);
void displayReady(uint8_t test);


#endif /* MYTIMER_H_ */
