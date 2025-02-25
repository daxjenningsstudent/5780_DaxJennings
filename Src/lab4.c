#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>

int lab4_main(void) {
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
    USART3->CR1 |= USART_CR1_TE; // Enable the USART3 transmitter
    USART3->CR1 |= USART_CR1_RE; // Enable the USART3 receiver
    USART3->CR1 |= USART_CR1_UE; // Enable the USART3

    void USART_Transmit(char data) {
        // Wait until the Transmit Data Register Empty (TXE) flag is set
        while (!(USART3->ISR & (1 << 7))); 
    
        // Write the character to the USART data register
        USART3->RDR = data;
    }

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high
    // Assert PC8 is high
    assert(GPIOC->ODR & GPIO_PIN_6);

    while (1) {
        //USART_Transmit('A');  // Transmit the character 'A'
        //HAL_Delay(100); // Delay 100ms 
        
        // Wait for a character from the serial terminal
        char receivedChar = USART_Receive();

        // Check the received character and toggle the correct LED
        switch (receivedChar) {
            case 'r':
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);  // Toggle Red LED
                break;
            case 'g':
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);  // Toggle Green LED
                break;
            case 'b':
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // Toggle Blue LED
                break;
            case 'o':
                My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);  // Toggle Orange LED
                break;
            default:
                USART_Transmit('E');  // Send an error message if input is invalid
                USART_Transmit('r');
                USART_Transmit('r');
                USART_Transmit('o');
                USART_Transmit('r');
                USART_Transmit('\n');  // New line for readability
                break;
        }
    }
}