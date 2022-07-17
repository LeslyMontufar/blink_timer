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

#define APP_DEBOUNCING_TIME_MS 	50

#define DELAY_250				500
#define DELAY_500				1000

static uint32_t delay=DELAY_500;
bool app_started = false;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void app_delay_toggle(void){
	delay = (delay == DELAY_250)? DELAY_500 : DELAY_250;
	hw_set_timer(delay);
}

void app_button_interrupt(void){
	if(!app_started)
		return;
	hw_timer_start(&htim2); // debouncing, para por interrupção Elapsed
}

void app_init(void){
	app_started = true;
	hw_set_timer(delay);
	hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
	hw_timer_start(&htim1);
}

void app_loop(void){
	hw_cpu_sleep();
}
