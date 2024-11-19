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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LED1_PIN       (1 << 1)
#define LED2_PIN       (1 << 2)
#define LED3_PIN       (1 << 3)
#define LED4_PIN       (1 << 4)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId myTaskTarefa_umHandle;
osThreadId myTaskTarefa_doHandle;
osThreadId myTaskTarefa_trHandle;
osMessageQId myQueueFila1Handle;
osMessageQId myQueueFila2Handle;
osMessageQId myQueueFila3Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Tarefa_um(void const * argument);
void Tarefa_dois(void const * argument);
void Tarefa_tres(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void GPIO_Config(void) {
    // Habilita o clock do GPIO A
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;



    GPIOA->MODER &= ~(0x3 << (1 * 2));  // Limpa os bits de modo para PA1
    GPIOA->MODER |= (0x1 << (1 * 2));   // Configura PA1 como saída

    GPIOA->MODER &= ~(0x3 << (2 * 2));  // Limpa os bits de modo para PA2
    GPIOA->MODER |= (0x1 << (2 * 2));   // Configura PA2 como saída

    GPIOA->MODER &= ~(0x3 << (3 * 2));  // Limpa os bits de modo para PA3
    GPIOA->MODER |= (0x1 << (3 * 2));   // Configura PA3 como saída


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
 // HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
 // SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
	GPIO_Config();

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

  /* Create the queue(s) */
  /* definition and creation of myQueueFila1 */
  osMessageQDef(myQueueFila1, 2, uint16_t);
  myQueueFila1Handle = osMessageCreate(osMessageQ(myQueueFila1), NULL);

  /* definition and creation of myQueueFila2 */
  osMessageQDef(myQueueFila2, 2, uint16_t);
  myQueueFila2Handle = osMessageCreate(osMessageQ(myQueueFila2), NULL);

  /* definition and creation of myQueueFila3 */
  osMessageQDef(myQueueFila3, 2, uint16_t);
  myQueueFila3Handle = osMessageCreate(osMessageQ(myQueueFila3), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of myTaskTarefa_um */
  osThreadDef(myTaskTarefa_um, Tarefa_um, osPriorityNormal, 0, 128);
  myTaskTarefa_umHandle = osThreadCreate(osThread(myTaskTarefa_um), NULL);

  /* definition and creation of myTaskTarefa_do */
  osThreadDef(myTaskTarefa_do, Tarefa_dois, osPriorityIdle, 0, 128);
  myTaskTarefa_doHandle = osThreadCreate(osThread(myTaskTarefa_do), NULL);

  /* definition and creation of myTaskTarefa_tr */
  osThreadDef(myTaskTarefa_tr, Tarefa_tres, osPriorityIdle, 0, 128);
  myTaskTarefa_trHandle = osThreadCreate(osThread(myTaskTarefa_tr), NULL);

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
	int value =0;
  for(;;)
  {
	  value = (value == 1) ? 0 : 1;
		  if(value == 0){
			  GPIOA->ODR |= LED1_PIN;
		  }else{
			  GPIOA->ODR &= ~LED1_PIN;
		  }

		  osMessagePut(myQueueFila1Handle, value, osWaitForever);
	    osDelay(900);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Tarefa_dois */
/**
* @brief Function implementing the myTaskTarefa_do thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tarefa_dois */
void Tarefa_dois(void const * argument)
{
  /* USER CODE BEGIN Tarefa_dois */
	/*infinite loop*/
	   osEvent event;
	for(;;)
	{
	  event = osMessageGet(myQueueFila1Handle, osWaitForever);
	 	  if (event.status == osEventMessage) {
	 	    int valueR = event.value.v;
	 	    if (valueR == 0) {

	 	    GPIOA->ODR |= LED2_PIN;

	 	    } else {
	 	    GPIOA->ODR &= ~LED2_PIN;

	 	    }
	 	    osMessagePut(myQueueFila2Handle, valueR, osWaitForever);
	 	    }
	     osDelay(1000);
}
  /* USER CODE END Tarefa_dois */
}

/* USER CODE BEGIN Header_Tarefa_tres */
/**
* @brief Function implementing the myTaskTarefa_tr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tarefa_tres */
void Tarefa_tres(void const * argument)
{
  /* USER CODE BEGIN Tarefa_tres */
  /* Infinite loop */
	  for(;;)
	  {
		  osEvent event; // Estrutura para armazenar o evento recebido

		  	  for(;;)
		  	  {
		  		  event = osMessageGet(myQueueFila2Handle, osWaitForever);
		  		  if (event.status == osEventMessage) {
		  		    int valueR = event.value.v;
		  		    if (valueR == 0) {

		  		    GPIOA->ODR |= LED3_PIN;

		  		    } else {
		  		    GPIOA->ODR &= ~LED3_PIN;

		  		    }
		  		    }
		  	    osDelay(1300);
		  	  }
	  }
  /* USER CODE END Tarefa_tres */
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
