#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>


void EXTI0_1_IRQHandler(void)
{
    // Toggle PC8 and PC9 (Green & Orange LEDs)
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    EXTI->PR = (1<<0);

    // Busy-wait delay loop (approx 1-2 seconds)
    //volatile uint32_t delay_count = 1500000;
    //while (delay_count--);  // Decrement until the delay is over
    volatile int count = 0;
    while(count < 1500000)
    {
        count ++;
    }
    count = 0;

    // Toggle LEDs after delay
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8); // Green LED
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9); // Orange LED

    // Clear the EXTI pending flag for line 0 (EXTI_PR_PIF0)
    EXTI->PR |= (1 << 0);
}

int lab2_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    assert(HAL_GetTick() == 0); // Ensure HAL initialization resets tick counter
    setup_interrupt();

    NVIC_SetPriority(EXTI0_1_IRQn, 1);
    NVIC_SetPriority(SysTick_IRQn, 2);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    
    SystemClock_Config(); //Configure the system clock

    /* This example uses HAL library calls to control
        the GPIOC peripheral. You’ll be redoing this code
        with hardware register access. */

    My_HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};

    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC6, PC7, PC8 & PC9

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Start PC9 (Green) high

    while (1) {
        // Toggle the output state of both PC8 and PC9
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        HAL_Delay(500); // Delay 500ms
    }
}