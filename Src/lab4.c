#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

int lab2_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    
    SystemClock_Config(); //Configure the system clock

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable the USART3 clock in the RCC

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL,
                                0x1};

    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins
    
    USART3->BRR = HAL_RCC_GetHCLKFreq() / 115200; // Set the Baud rate for communication to be 115200 bits/second

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high
    // Assert PC8 is high
    assert(GPIOC->ODR & GPIO_PIN_6);

    static GPIO_PinState last_button_state = GPIO_PIN_RESET; // Remember the last state of the button

    while (1) {
        HAL_Delay(100); // Delay 100ms
        // Toggle the output state of both PC8 and PC9
        //My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);

        GPIO_PinState current_button_state = My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        if (current_button_state == GPIO_PIN_SET && last_button_state == GPIO_PIN_RESET) 
        {
            // Button is pressed
            My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
        } 

        // Update the last button state
        last_button_state = current_button_state;
    }
}