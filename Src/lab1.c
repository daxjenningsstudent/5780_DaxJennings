#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

int lab1_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    assert(HAL_GetTick() == 0); // Ensure HAL initialization resets tick counter
    
    SystemClock_Config(); //Configure the system clock

    /* This example uses HAL library calls to control
        the GPIOC peripheral. You’ll be redoing this code
        with hardware register access. */

    My_HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};

    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
    // Assert that PC8 and PC9 are set to output mode (MODER = 01 for each pin)
    assert((GPIOC->MODER & (0b11 << (6 * 2))) == (0b01 << (6 * 2))); // PC8
    assert((GPIOC->MODER & (0b11 << (7 * 2))) == (0b01 << (7 * 2))); // PC9

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