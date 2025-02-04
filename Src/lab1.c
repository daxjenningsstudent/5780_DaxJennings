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

    while (1) {
        HAL_Delay(200); // Delay 200ms
        // Toggle the output state of both PC8 and PC9
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
    }
}