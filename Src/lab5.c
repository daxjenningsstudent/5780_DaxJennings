#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

int lab5_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); //Configure the system clock

    // 5.2.1 Enable GPIOB and GPIOC in the RCC.
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // 5.2.2 Set PB11 to alternate function mode, open-drain output type, and select I2C2_SDA as its alternate function
    // Set PB11 to alternate function mode
    GPIOB->MODER &= ~(GPIO_MODER_MODER11); // (Clear bit 0)
    GPIOB->MODER |= (GPIO_MODER_MODER11_1); // (Set bit 1)
    // Open-drain output type
    GPIOB->OTYPER |= (1 << 11); // Sets the 11 bit in the GPIOB_OTYPER register
    // Select I2C2_SDA as alternate function
    GPIOB->AFR[0] &= ~(0xF << (11 * 4));  // Clear current AF settings
    GPIOB->AFR[0] |= (1 << (11 * 4)); // Set AF1 for PB11

    // 5.2.3 Set PB13 to alternate function mode, open-drain output type, and select I2C2_SCL as its alternate function.
    // Set PB13 to alternate function mode
    GPIOB->MODER &= ~(GPIO_MODER_MODER13); // (Clear bit 0)
    GPIOB->MODER |= (GPIO_MODER_MODER13_1); // (Set bit 1)
    // Open-drain output type
    GPIOB->OTYPER |= (1 << 11); // (Set bit 13)
    // Select I2C2_SCL as alternate function
    GPIOB->AFR[0] &= ~(0xF << (11 * 4));  // Clear current AF settings
    GPIOB->AFR[0] |= (5 << (13 * 4)); // Set AF5 for PB13

    // 5.2.4 Set PB14 to output mode, push-pull output type, and initialize/set the pin high.
    // Set PB14 to output mode
    GPIOB->MODER |= (1 << 28); // (Set bit 28)
    GPIOB->MODER &= ~(1 << 29); // (Clear bit 29)
    // Push-pull output type
    GPIOB->OTYPER &= ~(1 << 14); // (Clear bit 14)
    // Set pin high
    My_HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Start PB14 high

    // 5.2.5 Set PC0 to output mode, push-pull output type, and initialize/set the pin high.
    // Set PC0 to output mode
    GPIOC->MODER |= (1 << 0); // (Set bit 0)
    GPIOC->MODER &= ~(1 << 1); // (Clear bit 1)
    // Push-pull output type
    GPIOC->OTYPER &= ~(1 << 0); // (Clear bit 14)
    // Set pin high
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); // Start PC0 high



    // 5.3.1 Enable the I2C2 peripheral in the RCC.
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    // 5.3.2 Set the parameters in the TIMINGR register to use 100 kHz standard-mode I2C.
    // PRESC to 1
    I2C->TIMINGR |= (1 << 28); // (Set bit 28)
    // SCLL to 0x13
    I2C->TIMINGR |= (1 << 0) | (1 << 1) | (1 << 4); // (Set bits 0, 1, and 4)
    // SCLH to 0xF
    I2C->TIMINGR |= (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11); // (Set bits 8-11)
    // SDADEL to 0x2
    I2C->TIMINGR |= (1 << 17); // (Set bit 17)
    // SCLDEL to 0x4
    I2C->TIMINGR |= (1 << 22); // (Set bit 22)
    // 5.3.3 Enable the I2C peripheral using the PE bit in the CR1 register.
    I2C->CR1 |= (1 << 0); // (Set bit 0)



    //5.4.1 Set the L3GD20 slave address = 0x6B
    I2C->CR2 |= (1 << 1) | (1 << 4) | (1 << 6) | (1 << 7); // (Set bits 1, 4, 6, and 7)
    // Number of bytes to transmit = 1.
    I2C->NBYTES |= (1 << 16); // (Set bit 0)
    
}