# Debounce-Button-CMSIS
 Simple and easy to use debounce algorithm for STM32.
* Article (rus): https://cxemka.com/72-prostoi-kod-dlja-drebezga-knopok-k-stm32.html
* Youtube (rus): 
* Source (en): https://cxemka.com/upload/art/button%20bounce/f/Software%20debouncing%20of%20button.pdf
___
  ![alt text](https://cxemka.com/upload/art/button%20bounce/software_button_bounce_stm32.svg)
* STM32F411CU6
* STM32CubeIDE
* CMSIS
 ## Connection diagram
  ![alt text](https://cxemka.com/upload/art/button%20bounce/button_circuit_diagram.svg)
 ## How to use?
 1. Create three global variables (for each pin):
```ruby
/* PA0 */
uint8_t pres_pa0;
uint8_t prev_pa0;
uint8_t cnt_pa0;
```
 2) Insert function Debounce in timer's Interrupt Handler (T=10ms):
```ruby
void TIM1_TRG_COM_TIM11_IRQHandler(void){

	if(TIM11->SR & TIM_SR_UIF){

		/* GPIOA-0 KEY */
		Debounce(&pres_pa0, &cnt_pa0, &prev_pa0, GPIOA->IDR, GPIO_IDR_ID0);

		TIM11->SR &= ~TIM_SR_UIF;
	}
}
```
3) In main loop check variable pres_pa0, if it's true -> execute required code:
```ruby
while(1){

		if(pres_pa0){
			pres_pa0 = 0;
   /* Here code to execute */
   
		}
		}
```
## Debug
  ![alt text](https://cxemka.com/upload/art/button%20bounce/debug_debounce.png)
