#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

void USART_Transmit(char data) {
    // Wait until the Transmit Data Register Empty (TXE) flag is set
    while (!(USART3->ISR & USART_ISR_TXE));
    
    // Write the character to the USART transmit register
    USART3->TDR = data;
}

char USART_Receive(void) {
    // Wait until a character is received (RXNE flag set)
    while (!(USART3->ISR & USART_ISR_RXNE));
    
    // Read the received character from the data register
    return USART3->RDR;
}

int lab4_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); // Configure the system clock

    // Enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {
        GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, // Pins for LEDs
        GPIO_MODE_OUTPUT_PP,
        GPIO_SPEED_FREQ_LOW,
        GPIO_NOPULL,
        0x1
    };

    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins

    // Set baud rate for 115200 bits/second (assuming PCLK1 = HCLK)
    USART3->BRR = HAL_RCC_GetPCLK1Freq() / 115200;

    // Enable USART3 transmitter, receiver, and USART itself
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    // Start with PC8 high
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    
    // Assert PC8 is high
    assert(GPIOC->ODR & GPIO_PIN_8);

    // Debug: Check if USART3 is enabled
    if ((USART3->CR1 & USART_CR1_UE)) {
        My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Turn on Orange LED if USART3 is not enabled
        while (1); // Halt execution if USART3 is not enabled
    }

    while (1) {
        
        char receivedChar = USART_Receive(); // Wait for a character from serial terminal

        // Check received character and toggle corresponding LED
        switch (receivedChar) {
            case 'r':
                USART_Transmit('r');
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);  // Toggle Red LED
                break;
            case 'g':
                USART_Transmit('g');
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);  // Toggle Green LED
                break;
            case 'b':
                USART_Transmit('b');
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // Toggle Blue LED
                break;
            case 'o':
                USART_Transmit('o');
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);  // Toggle Orange LED
                break;
            default:
                // Send error message if input is invalid
                USART_Transmit('E');
                USART_Transmit('r');
                USART_Transmit('r');
                USART_Transmit('o');
                USART_Transmit('r');
                USART_Transmit('\n'); // New line for readability
                break;
        }
    }
}