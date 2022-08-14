# Blink usando temporizadores

Blink por interrupção usando temporizador e entrada para troca de delay do blink com debouncing implementado em outro temporizador.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição

Parta do código criado anteriormente onde, através de um jumper em PA0, era simulada a ação de
aperto de um botão, alterando a o tempo de piscada de um led. Vamos repetir esse trabalho mas
agora utilizando um temporizador e não o systick.

A taxa de acendimento do led deve ser alterada entre os padrões de 250ms on/off e 500ms on/off, a
cada novo pressionamento, com o botão tratado na interrupção e com uma rotina de debouncing
descrita mais adiante.

Nesse exercício a taxa de piscada do led será dada por um timer cujo tempo irá variar a cada
pressionamento em PA0 e quando o timer estourar, deverá ser feita a operação de toggle no led.

O debouncing também será obrigatório mas sendo realizado por outro timer. No caso, ao chegar a
interrupção PA0, essa interrupção EXTI deve ser desabilitada e o timer de deboucing ligado. Ao
estourar o tempo do timer, a interrupção deve ser novamente ativada, dentro da função de callback
do timer e esse timer desativado.

Perceba que todo o seu código será executado em modo interrompido. Coloque o controlador em
estado de economia de energia dentro da função main.

## Desenvolvimento
1. Inicialização dos tempos dos timers de blink e debouncing. E inicia o timer do blink.
```
void app_init(void){
	app_started = true;
	hw_set_blink_timer(delay);
	hw_set_debouncing_timer(APP_DEBOUNCING_TIME_MS);
	hw_blink_timer_start();
}
```
```
void hw_timer_start(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
}

void hw_blink_timer_start(void){
	hw_timer_start(&htim1);
}
```
2. Quando o tempo do timer finaliza entra na função de callaback do timer. 
```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)	{
		hw_led_toggle();
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	if(htim == &htim2)	{
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn); // Reativa a interrupção do botão
	}
}
```
3. Quando o botão é pressionado, o delay do led muda entre 250 ms e 500 ms. A interrupção do botão é desabilitada para evitar o deabouncing, e é reativada quando o timer indica fim do tempo estipulado para debouncing.
```
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BUTTON_Pin)
	{
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		app_button_interrupt();
	}
}
```
```
void app_button_interrupt(void){
	if(!app_started)
		return;
	app_delay_toggle();
	hw_debouncing_timer_start();
}
```
```
void app_delay_toggle(void){
	delay = (delay == DELAY_250)? DELAY_500 : DELAY_250;
	hw_set_blink_timer(delay);
}
```
4. As funções do HAL responsáveis por setar o novo valor do timer baseia-se em calcular o novo ARR, considerando o CLKINT para o Pre-escalar escolhido.
```
#define CLKINT 2000 // 36000 - 1 = preescalar
```

```
void hw_set_blink_timer(uint16_t delay) {
	uint16_t arr = (CLKINT*delay/1000)-1;
	__HAL_TIM_SET_AUTORELOAD(&htim1, arr);
	__HAL_TIM_SET_COUNTER(&htim1, 0);
}
```
