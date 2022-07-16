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

#define DELAY_250				250
#define DELAY_500				500

volatile uint32_t delay;
bool app_started = false;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void app_button_interrupt(){
	if(!app_started)
		return;
	hw_timer_start(&htim2); // debouncing
}

void app_delay_toggle(){
	delay = (delay == DELAY_250)? DELAY_500 : DELAY_250;
}

void app_tick_1ms(void){
	static uint32_t delay_cnt = 0;

	if(!app_started)
		return;

	delay_cnt++;

	if(delay_cnt >= delay){
		delay_cnt = 0;
		hw_led_toggle();
	}

}

void app_init(void){
	app_started = true;
}

void app_loop(void){
	hw_cpu_sleep();
}
