#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>


void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    // LED Pins
    // General-purpose output mode using the MODER register
        // Sets the 12, 14, 16, and 18 bits in the GPIOC_MODER register
        GPIOC->MODER |= (1 << 12) | (1 << 14) | (1 << 16) | (1 << 18);
        // Clears the 13, 15, 17, and 19 bits in the GPIOC_MODER register
        GPIOC->MODER &= ~((1 << 13) | (1 << 15) | (1 << 17) | (1 << 19));
    // Push-pull output type using the OTYPER register
        // Clears the 6, 7, 8, and 9 bits in the GPIOC_OTYPER register
        GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    // Low speed using the OSPEEDR register
        // Clears the 12, 14, 16, and 18 bits in the GPIOC_OSPEEDR register
        GPIOC->OSPEEDR &= ~((1 << 12) | (1 << 14) | (1 << 16) | (1 << 18));
    // No pull-up/down resistors using the PUPDR register
        // Clears the 12, 14, 16, and 18 bits in the GPIOC_PUPDR register
        GPIOC->PUPDR &= ~((1 << 12) | (1 << 14) | (1 << 16) | (1 << 18) | (1 << 13) | (1 << 15) | (1 << 17) | (1 << 19));
    
    // USER Button Pin
    // Digital input mode using the MODER register
        // Clears the 0 and 1 bits in the GPIOA_MODER register
        GPIOA->MODER &= ~(0 | 1);
    // Low speed using the OSPEEDR register
        // Clears the 0 bit in the GPIOA_OSPEEDR register
        GPIOA->OSPEEDR &= ~(0);
    // Pull-down resistor using the PUPDR register.
        // Sets the 1 bit in the GPIOA_PUPDR register
        GPIOA->PUPDR |= (1);
        // Clears the 0 bit in the GPIOA_PUPDR register
        GPIOA->PUPDR &= ~(0);

    // PC4 and PC5
        // Alternate Function mode using the MODER register
        // Sets the 9 and 11 bits in the GPIOC_MODER register
        GPIOC->MODER |= (1 << 9) | (1 << 11);
        // Clears the 8 and 10 bits in the GPIOC_MODER register
        GPIOC->MODER &= ~((1 << 8) | (1 << 10)); 
        // Set Alternate Function AF1 for USART3 TX/RX
        GPIOC->AFR[0] &= ~((0xF << (4 * 4)) | (0xF << (5 * 4)));  // Clear existing AF
        GPIOC->AFR[0] |= (1 << (4 * 4)) | (1 << (5 * 4));  // Set AF1 (USART3)
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState currentbit;

    assert_param(IS_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        currentbit = GPIO_PIN_SET;
    }
    else
    {
        currentbit = GPIO_PIN_RESET;
    }
    return currentbit;
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_PIN_ACTION(PinState));

    if (PinState != GPIO_PIN_RESET)
    {
        GPIOx->BSRR = (uint32_t)GPIO_Pin;
    }
    
    else
    {
        GPIOx->BRR = (uint32_t)GPIO_Pin;
    }
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint32_t odr;
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    odr = GPIOx->ODR;
    GPIOx->BSRR = ((odr & GPIO_Pin) << 16U) | (~odr & GPIO_Pin); 
}

void setup_interrupt()
{
    EXTI ->IMR |= 0x01;
    EXTI ->RTSR |= 0x01;

    SYSCFG->EXTICR[0] &= ~(0xF);  // Clear all EXTI line 0 settings
    //SYSCFG->EXTICR[0] |= (0x0);   // Set EXTI0 to GPIOA (for PA0)

    // NVIC_EnableIRQ (EXTI0_1_IRQn);
    // NVIC_SetPriority(SysTick_IRQn, 0);
    // NVIC_SetPriority (EXTI0_1_IRQn, 2);
}
