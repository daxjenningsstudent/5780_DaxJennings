#include <stm32f0xx_hal.h>
#include "main.h"
#include <assert.h>
volatile uint16_t read;
volatile uint16_t flag = 0;

void USART_Transmit(char data) {
    // Wait until the Transmit Data Register Empty (TXE) flag is set
    while (!(USART3->ISR & USART_ISR_TXE)){}
    
    // Write the character to the USART transmit register
    USART3->TDR = data;
}

char USART_Receive(void) {
    // Wait until a character is received (RXNE flag set)
    while (!(USART3->ISR & USART_ISR_RXNE)){}

    // Read the received character from the data register
    return USART3->RDR;
    //return read;
}


int lab4_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config(); // Configure the system clock
    My_HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC

    // Enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock


    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {
        GPIO_PIN_4 | GPIO_PIN_5, // Pins for TX and RX
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
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

    // Debug: Check if USART3 is enabled
    if (! ((USART3->CR1 & USART_CR1_TE) | (USART3->CR1 & USART_CR1_RE) | (USART3->CR1 & USART_CR1_UE))) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // Turn on blue LED if USART3 is not enabled
        while (1); // Halt execution if USART3 is not enabled
    }

    NVIC_SetPriority(USART3_4_IRQn, 1);
    NVIC_EnableIRQ(USART3_4_IRQn);
    
    while (1) {
        USART_Transmit('C');
        USART_Transmit('M');
        USART_Transmit('D');
        USART_Transmit('?');
        USART_Transmit('\n'); // New line for readability  
        while (flag == 0){}
        flag = 0;

        //char receivedChar = USART_Receive(); // Wait for a character from serial terminal
        
        // Check received character and toggle corresponding LED
        switch (read) {
            
            case 'r':
                USART_Transmit('r');
                USART_Transmit('\n');
                while (flag == 0){}
                flag = 0;
                //If 0
                if (read == 48){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // Turn Off 
                }
                //If 1
                if (read == 49){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // Turn On
                }
                //If 2
                if (read == 50){ 
                    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);  // Toggle
                }
                
                break;
            case 'g':
                USART_Transmit('g');
                USART_Transmit('\n');
                while (flag == 0){}
                flag = 0;
                //If 0
                if (read == 48){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);  // Turn Off 
                }
                //If 1
                if (read == 49){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);  // Turn On
                }
                //If 2
                if (read == 50){ 
                    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);  // Toggle
                }
                break;
            case 'b':
                USART_Transmit('b');
                USART_Transmit('\n');
                while (flag == 0){}
                flag = 0;
                //If 0
                if (read == 48){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);  // Turn Off 
                }
                //If 1
                if (read == 49){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);  // Turn On
                }
                //If 2
                if (read == 50){ 
                    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // Toggle
                }
                break;
            case 'o':
                USART_Transmit('o');
                USART_Transmit('\n');
                while (flag == 0){}
                flag = 0;
                //If 0
                if (read == 48){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  // Turn Off 
                }
                //If 1
                if (read == 49){ 
                    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);  // Turn On
                }
                //If 2
                if (read == 50){ 
                    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);  // Toggle
                }
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

void USART3_4_IRQHandler(void){
    read = USART3->RDR;
    flag = 1;
}