#include "main.h"

/****** Prototypes ******/
void Cnf_GPIO(void);
void Cnf_TIM11(void);
void Debounce(uint8_t *pres_px, uint8_t *cnt_px, uint8_t *but_px, uint32_t GPIOx_IDR, uint32_t GPIO_IDR_IDx);
/************************/

/********** PV **********/
/* PA0 */
uint8_t pres_pa0;
uint8_t prev_pa0;
uint8_t cnt_pa0;

/* PA7 */
uint8_t pres_pa7;
uint8_t prev_pa7;
uint8_t cnt_pa7;

/* Other */
uint32_t counter;
/************************/

int main(void)
{
	/* Configuration */
	Cnf_GPIO();
	Cnf_TIM11();

	while(1){

		if(pres_pa0){
			pres_pa0 = 0;

			GPIOC->BSRR |= GPIO_BSRR_BR13;
			counter++;
		}

		if(pres_pa7){
			pres_pa7 = 0;

			GPIOC->BSRR |= GPIO_BSRR_BS13;
			counter--;
		}
		}
}

/*****************************************************/

void Cnf_GPIO(void){

	/* GPIOC 13 OUT PP */

	/* Clock GPIOC */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	/* 01: General-purpose output */
	GPIOC->MODER |= GPIO_MODER_MODE13_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE13_1;

	/* 0: Output push-pull */
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;

	/* 00: Low speed */
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR13;

	/* 00: Np pull-pull up, pull-down */
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13;

	/**********************/

	/* GPIOA 0 IN PU */

	/* Clock GPIOA */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* 00: Input (reset state) */
	GPIOA->MODER &= ~GPIO_MODER_MODE0_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE0_1;

	/* 01: Pull-up */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_1;

	/* GPIOA 7 IN PU */

	/* Clock GPIOA */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* 00: Input (reset state) */
	GPIOA->MODER &= ~GPIO_MODER_MODE7_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE7_1;

	/* 01: Pull-up */
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD7_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7_1;
}

void Cnf_TIM11(void){

	/* Clock TIM11 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

	/* T=10ms f=100Hz */
	TIM11->PSC = 160-1;
	TIM11->ARR = 1000-1;

	/* Update Interrupt Enable */
	TIM11->DIER |= TIM_DIER_UIE;

	/* Counter enable */
	TIM11->CR1 |= TIM_CR1_CEN;

	/* TIM11 Interrupt enable */
	NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){

	if(TIM11->SR & TIM_SR_UIF){

		/* GPIOA-0 KEY */
		Debounce(&pres_pa0, &cnt_pa0, &prev_pa0, GPIOA->IDR, GPIO_IDR_ID0);
		Debounce(&pres_pa7, &cnt_pa7, &prev_pa7, GPIOA->IDR, GPIO_IDR_ID7);

		TIM11->SR &= ~TIM_SR_UIF;
	}
}

/**
  * @brief Debounce function for button
  * @note Perform it every 10-20ms (e.g. in timer's interrupt)
  * In main loop check first variable ((*pres_px), if (it > 0) nullify it and execute required code
  * @param Three global variables, IDR register, Bit for check (e.g. &pres_pa0, &cnt_pa0, &prev_pa0, GPIOA->IDR, GPIO_IDR_ID0)
  * @retval no
  */
void Debounce(uint8_t *pres_px, uint8_t *cnt_px, uint8_t *prev_px, uint32_t GPIOx_IDR, uint32_t GPIO_IDR_IDx)
{
	/* Read current state */
	uint8_t cur_px = (~GPIOx_IDR & GPIO_IDR_IDx) != 0;

	/* If level has changed  */
	if (cur_px != *prev_px) {

	/* Increase counter */
	(*cnt_px)++;

	/* If consecutive 4*10 = 40ms approved */
	if (*cnt_px >= 4) {

	/* The button have not bounced for four checks, change state */
	*prev_px = cur_px;

	/* If the button was pressed (not released), tell main so */
	if (cur_px != 0)
	{
		*pres_px = 1;
	}

	(*cnt_px) = 0;
	}
	}
	else {

	/* Reset counter */
	*cnt_px = 0;
	}
}
