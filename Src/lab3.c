#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

int lab3_main(void) {
    My_HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};

    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC6, PC7, PC8 & PC9

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high

    Timer2_Init();

    while (1) {
        // Main loop does nothing, the timer interrupt handles LED toggling
    }
}

void Timer2_Init(void) {
    // 1. Enable Timer 2 Peripheral in RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Configure Timer to Trigger UEV at 4 Hz
    // Processor frequency: 8 MHz
    // Desired timer frequency: 4 Hz
    // Timer clock = 8 MHz / (PSC + 1)
    // ARR counts required for 4 Hz: (Timer Clock) / 4
    TIM2->PSC = 1999;  // Prescaler: 2000 - 1 (divides 8 MHz by 2000 -> 4 kHz)
    TIM2->ARR = 999;   // Auto-reload: 1000 - 1 (4 kHz / 1000 = 4 Hz)

    // 3. Enable UEV Interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // 4. Enable Timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // 5. Enable Timer Interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
}

// Timer Interrupt Handler
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
        TIM2->SR &= ~TIM_SR_UIF; // Clear the update interrupt flag

        // Toggle LEDs after delay
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8); // Green LED
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9); // Orange LED
    }
}