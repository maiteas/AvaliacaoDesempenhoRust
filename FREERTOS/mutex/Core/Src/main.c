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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//#define LED1_PIN       (1 << 0)
#define LED1_PIN       (1 << 1)
#define LED2_PIN       (1 << 2)
#define LED3_PIN       (1 << 3)
#define LED4_PIN       (1 << 4)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId myTaskTarefa_umHandle;
osThreadId myTaskTarefa_doHandle;
osThreadId myTaskTarefa_trHandle;
osThreadId myTaskTarefa_quHandle;
osMutexId myMutexSaida1Handle;
osMutexId myMutexSaida2Handle;
osMutexId myMutexSaida3Handle;
osMutexId myMutexSaida4Handle;
osMutexId myMutexLED5Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void StartDefaultTask(void const * argument);
void Tarefa_um(void const * argument);
void Tarefa_dois(void const * argument);
void Tarefa_tres(void const * argument);
void Tarefa_quatro(void const * argument);

/* USER CODE BEGIN PFP */
/* Função para configurar o GPIOA */
void GPIO_Config(void) {
    // Habilita o clock do GPIO A
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    // Configura PA0-PA4 como saída
    GPIOA->MODER &= ~(0x3 << (0 * 2));  // Limpa os bits de modo para PA0
    GPIOA->MODER |= (0x1 << (0 * 2));   // Configura PA0 como saída

    GPIOA->MODER &= ~(0x3 << (1 * 2));  // Limpa os bits de modo para PA1
    GPIOA->MODER |= (0x1 << (1 * 2));   // Configura PA1 como saída

    GPIOA->MODER &= ~(0x3 << (2 * 2));  // Limpa os bits de modo para PA2
    GPIOA->MODER |= (0x1 << (2 * 2));   // Configura PA2 como saída

    GPIOA->MODER &= ~(0x3 << (3 * 2));  // Limpa os bits de modo para PA3
    GPIOA->MODER |= (0x1 << (3 * 2));   // Configura PA3 como saída

    GPIOA->MODER &= ~(0x3 << (4 * 2));  // Limpa os bits de modo para PA4
    GPIOA->MODER |= (0x1 << (4 * 2));   // Configura PA4 como saída
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	 GPIO_Config();

  /* USER CODE END Init */

  /* Configure the system clock */
 // SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of myMutexSaida1 */
  osMutexDef(myMutexSaida1);
  myMutexSaida1Handle = osMutexCreate(osMutex(myMutexSaida1));

  /* definition and creation of myMutexSaida2 */
  osMutexDef(myMutexSaida2);
  myMutexSaida2Handle = osMutexCreate(osMutex(myMutexSaida2));

  /* definition and creation of myMutexSaida3 */
  osMutexDef(myMutexSaida3);
  myMutexSaida3Handle = osMutexCreate(osMutex(myMutexSaida3));

  /* definition and creation of myMutexSaida4 */
  osMutexDef(myMutexSaida4);
  myMutexSaida4Handle = osMutexCreate(osMutex(myMutexSaida4));


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
  /* definition and creation of defaultTask */


  /* definition and creation of myTaskTarefa_um */
  osThreadDef(myTaskTarefa_um, Tarefa_um, osPriorityIdle, 0, 128);
  myTaskTarefa_umHandle = osThreadCreate(osThread(myTaskTarefa_um), NULL);

  /* definition and creation of myTaskTarefa_do */
  osThreadDef(myTaskTarefa_do, Tarefa_dois, osPriorityIdle, 0, 128);
  myTaskTarefa_doHandle = osThreadCreate(osThread(myTaskTarefa_do), NULL);

  /* definition and creation of myTaskTarefa_tr */
  osThreadDef(myTaskTarefa_tr, Tarefa_tres, osPriorityIdle, 0, 128);
  myTaskTarefa_trHandle = osThreadCreate(osThread(myTaskTarefa_tr), NULL);

  /* definition and creation of myTaskTarefa_qu */
  osThreadDef(myTaskTarefa_qu, Tarefa_quatro, osPriorityIdle, 0, 128);
  myTaskTarefa_quHandle = osThreadCreate(osThread(myTaskTarefa_qu), NULL);

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

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */


/* USER CODE BEGIN Header_Tarefa_um */
/**
* @brief Function implementing the myTaskTarefa_um thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tarefa_um */
void Tarefa_um(void const * argument)
{
  /* USER CODE BEGIN Tarefa_um */
  /* Infinite loop */
  for(;;)
  {
	  if (osMutexWait(myMutexSaida1Handle, osWaitForever) == osOK) {
	      GPIOA->ODR |= LED1_PIN;
	      GPIOA->ODR &= ~LED2_PIN;
	      GPIOA->ODR &= ~LED3_PIN;
	      GPIOA->ODR &= ~LED4_PIN;
	      osMutexRelease(myMutexSaida1Handle);
	   }
    osDelay(1200);
  }
  /* USER CODE END Tarefa_um */
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
  /* Infinite loop */
  for(;;)
  {
	  if (osMutexWait(myMutexSaida2Handle, osWaitForever) == osOK) {
		             // Apagar todos os LEDs
		          //  GPIOA->ODR &= ~(LED1_PIN | LED3_PIN | LED4_PIN );
		             // Acender LED2
		             GPIOA->ODR |= LED2_PIN;
		             GPIOA->ODR &= ~LED1_PIN;
		         	 GPIOA->ODR &= ~LED3_PIN;
		         	 GPIOA->ODR &= ~LED4_PIN;
		             osMutexRelease(myMutexSaida2Handle);
		         }
	    osDelay(1800);
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

	    if (osMutexWait(myMutexSaida3Handle, osWaitForever) == osOK) {
	            // Apagar todos os LEDs
	        //    GPIOA->ODR &= ~(LED1_PIN | LED2_PIN | LED4_PIN );
	            // Acender LED3
	            GPIOA->ODR |= LED3_PIN;
	            GPIOA->ODR &= ~LED1_PIN;
	        	GPIOA->ODR &= ~LED2_PIN;
	        	GPIOA->ODR &= ~LED4_PIN;
	            osMutexRelease(myMutexSaida3Handle);
	        }
	        osDelay(2500); // Delay para controlar a frequência
  }
  /* USER CODE END Tarefa_tres */
}

/* USER CODE BEGIN Header_Tarefa_quatro */
/**
* @brief Function implementing the myTaskTarefa_qu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tarefa_quatro */
void Tarefa_quatro(void const * argument)
{
  /* USER CODE BEGIN Tarefa_quatro */
  /* Infinite loop */
  for(;;)
  {
	    if (osMutexWait(myMutexSaida4Handle, osWaitForever) == osOK) {
	            // Apagar todos os LEDs
	           // GPIOA->ODR &= ~(LED1_PIN | LED2_PIN | LED3_PIN );
	            // Acender LED4
	            GPIOA->ODR |= LED4_PIN;
	            GPIOA->ODR &= ~LED1_PIN;
	        	GPIOA->ODR &= ~LED2_PIN;
	        	GPIOA->ODR &= ~LED3_PIN;
	            osMutexRelease(myMutexSaida4Handle);
	        }
  osDelay(3900);

  }
  /* USER CODE END Tarefa_quatro */
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
