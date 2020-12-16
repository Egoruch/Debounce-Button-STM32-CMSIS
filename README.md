# Debounce-Button-CMSIS
 Simple and easy to use debounce algorithm for STM32.
* Article (rus): https://cxemka.com/72-prostoi-kod-dlja-drebezga-knopok-k-stm32.html
* Youtube (rus): 
* Source (en): https://cxemka.com/upload/art/button%20bounce/f/Software%20debouncing%20of%20button.pdf
___
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
uint8_t cnt_pa0;```
