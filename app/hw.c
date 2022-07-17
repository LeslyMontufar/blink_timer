/*
 * hw.c
 * Abstrai as funções do HAL e CMSIS
 *
 *  Created on: Jun 20, 2022
 *      Author: lesly
 */

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "app.h"
#include "hw.h"

#define CLKINT 2000 // 36000 - 1 = preescalar

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void hw_timer_start(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
}

void hw_set_timer(uint16_t delay) {
	uint16_t arr = (CLKINT*delay/1000)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim1, arr); // led delay
}

void hw_set_debouncing_timer(uint16_t time_ms) {
	uint16_t arr = (CLKINT*time_ms/1000)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim2, arr); // led delay
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // Debouncing
{
	if(htim == &htim1)	{
		hw_led_toggle();
		__HAL_TIM_SET_COUNTER(&htim1, 0); // Reseta o timer de piscar led
	}
	else if(htim == &htim2)	{
		__HAL_TIM_SET_COUNTER(&htim2, 0); // Reseta timer do debouncing
		HAL_TIM_Base_Stop_IT(&htim2); // Para o temporizador de debouncing
		HAL_NVIC_EnableIRQ(EXTI0_IRQn); // Reativa a interrupção do botão
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BUTTON_Pin)
	{
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		app_button_interrupt();
	}
}

bool hw_button_state_get(void){
	GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
	if(button_state == GPIO_PIN_RESET)
		return true;
	else
		return false;
}

void hw_led_toggle(void){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void hw_led_state_set(bool state){
	GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);
}

void hw_delay_ms(uint32_t time_ms){
	HAL_Delay(time_ms);
}

void hw_cpu_sleep(){
	__WFI();
}

uint32_t hw_tick_ms_get(void){
	return HAL_GetTick();
}
