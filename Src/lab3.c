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
    TIM3_PWM_Init();

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

void TIM3_PWM_Init(void) {
    // 1. Enable TIM3 clock in RCC
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // 2. Set the timer update period for 800 Hz PWM
    // Formula: PWM frequency = timer clock / ((PSC + 1) * (ARR + 1))
    // Assume APB1 Timer Clock = 16 MHz
    TIM3->PSC = 99;  // Prescaler to slow down clock (16MHz / (99+1) = 160kHz)
    TIM3->ARR = 199; // Auto-reload value to get 800Hz (160kHz / 200 = 800Hz)

    // 3. Configure CCMR1 to set channels 1 & 2 in PWM mode
    TIM3->CCMR1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S); // Set as output
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // PWM Mode 2 for CH1
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos); // PWM Mode 1 for CH2

    // Enable output compare preload for both channels
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    // 4. Enable output channels in CCER
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // 5. Set CCRx registers to 20% of ARR
    TIM3->CCR1 = (uint16_t)(TIM3->ARR * 0.2); // 20% duty cycle for CH1
    TIM3->CCR2 = (uint16_t)(TIM3->ARR * 0.2); // 20% duty cycle for CH2

    // Start the timer
    TIM3->CR1 |= TIM_CR1_CEN;
}