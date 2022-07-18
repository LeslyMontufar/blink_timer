/*
 * Lesly Montúfar
 * app.c
 * parece um pouco o arduino esse .c
 *
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "hw.h"
#include "main.h"

#define APP_DEBOUNCING_TIME_MS 	50

#define DELAY_250				250
#define DELAY_500				500

static uint16_t delay = DELAY_500;
bool app_started = false;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void app_delay_toggle(void){
	delay = (delay == DELAY_250)? DELAY_500 : DELAY_250;
	hw_set_timer(delay);
	__HAL_TIM_SET_COUNTER(&htim1, 0);
}

void app_button_interrupt(void){
	if(!app_started)
		return;
	app_delay_toggle();
	hw_timer_start(&htim2);
}

void app_init(void){
	app_started = true;
	hw_set_timer(delay);
	hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
	hw_timer_start(&htim1);
}

void app_loop(void){
//	hw_cpu_sleep();
}
