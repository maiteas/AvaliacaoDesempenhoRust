/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32g0xx.h"
#include <stdbool.h>  // Biblioteca para usar o tipo bool
#include <stdatomic.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LED2_PIN       (1 << 1)
#define LED1_PIN       (1 << 2)

atomic_bool SAIDA_STATE;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId myTaskTarefa_umHandle;
osThreadId myTaskTarefa_saHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Tarefa_um(void const * argument);
void Tarefa_saida(void const * argument);

/* USER CODE BEGIN PFP */


void EXTI4_15_IRQHandler(void){

	if(EXTI->FPR1 & EXTI_FPR1_FPIF9){
	EXTI->FPR1 |= EXTI_FPR1_FPIF9;

		bool current_state = atomic_load(&SAIDA_STATE);
		atomic_store(&SAIDA_STATE, !current_state);
		}

}



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void GPIO_Config(void) {
    // Habilita o clock do GPIO A
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;



    GPIOA->MODER &= ~(0x3 << (1 * 2));  // Limpa os bits de modo para PA1
    GPIOA->MODER |= (0x1 << (1 * 2));   // Configura PA1 como saída

    GPIOA->MODER &= ~(0x3 << (2 * 2));  // Limpa os bits de modo para PA2
    GPIOA->MODER |= (0x1 << (2 * 2));   // Configura PA2 como saída




    /********** PB9 set for EXTI ************/
    /*set Input (0b00)*/
    GPIOB->MODER &= ~(GPIO_MODER_MODE9_0 | GPIO_MODER_MODE9_1);
    /*set low speed (0b01)*/
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_0 ;
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9_1 ;
    /*set Pull-up (0b01)*/
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0 ;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD9_1;
    /*set EXTI for PB9 - 9(EXTI9) PB(0b01)*/
    EXTI->EXTICR[2] &= ~EXTI_EXTICR3_EXTI9 ; //clear
    EXTI->EXTICR[2] |=  EXTI_EXTICR3_EXTI9_0 ; //set 0b01
    /*set EXTI Falling triger for Line no. 9 (PB9)*/
    EXTI->FTSR1 |= EXTI_FTSR1_FT9 ;
    /*set EXTI Interrupt no-masked for Line no.9 (PB9)*/
    EXTI->IMR1 |= EXTI_IMR1_IM9 ;

         // 4. Habilitar a interrupção no NVIC
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);


}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//  HAL_Init();

  /* USER CODE BEGIN Init */
	  GPIO_Config();

  /* USER CODE END Init */

  /* Configure the system clock */
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of myTaskTarefa_um */
  osThreadDef(myTaskTarefa_um, Tarefa_um, osPriorityNormal, 0, 128);
  myTaskTarefa_umHandle = osThreadCreate(osThread(myTaskTarefa_um), NULL);

  /* definition and creation of myTaskTarefa_sa */
  osThreadDef(myTaskTarefa_sa, Tarefa_saida, osPriorityNormal, 0, 128);
  myTaskTarefa_saHandle = osThreadCreate(osThread(myTaskTarefa_sa), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Tarefa_um */
/**
  * @brief  Function implementing the myTaskTarefa_um thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Tarefa_um */
void Tarefa_um(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
	  for(;;)
	  {
		  GPIOA->ODR ^= LED2_PIN; // Toggle the LED state

	    osDelay(500);
	  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Tarefa_saida */
/**
* @brief Function implementing the myTaskTarefa_sa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tarefa_saida */
void Tarefa_saida(void const * argument)
{
  /* USER CODE BEGIN Tarefa_saida */
  /* Infinite loop */
	atomic_store(&SAIDA_STATE, false); // Inicializa o estado como OFF
  for(;;)
  {
      if (atomic_load(&SAIDA_STATE)) {
    	  GPIOA->ODR |= LED1_PIN;
		 } else {
			 GPIOA->ODR &= ~LED1_PIN;


		 }
		 osDelay(500);  // Atualiza a cada 1 segundo
  }
  /* USER CODE END Tarefa_saida */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
