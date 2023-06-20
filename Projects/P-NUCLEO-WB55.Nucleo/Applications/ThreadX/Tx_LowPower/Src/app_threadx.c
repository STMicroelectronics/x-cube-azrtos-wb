/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
TX_SEMAPHORE tx_app_semaphore;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void SystemClock_Config(void);
static VOID App_Delay(ULONG Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for Main Thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create Main Thread.  */
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create Semaphore.  */
  if (tx_semaphore_create(&tx_app_semaphore, "Semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */

  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the MainThread_Entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_Entry */
  (void) thread_input;
  UINT i = 0;
  /* Infinite loop */
  while (1)
  {
    if (tx_semaphore_get(&tx_app_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      for (i=0; i<10; i++)
      {
      /* Toggle LED to indicate status*/
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      App_Delay(50);
      }
    }
  }
  /* USER CODE END MainThread_Entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/**
  * @brief  App_ThreadX_LowPower_Enter
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Enter(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Enter */
    GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Set all GPIO in analog state to reduce power consumption,                */
  /*   except GPIOC to keep user button interrupt enabled                     */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

  /* GPIO Ports Clock Disable */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();

  /* Enter to the stop mode */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
  /* USER CODE END  App_ThreadX_LowPower_Enter */
}

/**
  * @brief  App_ThreadX_LowPower_Exit
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Exit(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */
  GPIO_InitTypeDef  GPIO_InitStruct= {0};

  /* Reconfigure the system clock*/
  HAL_RCC_DeInit();
  SystemClock_Config();

  /* GPIOB Port Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Reconfigure GPIO pin : GREEN & RED LEDs Pins */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Hold the CPU2 and its allocated peripherals after wakeup from Stop 2 Mode */
  HAL_PWREx_HoldCore(PWR_CORE_CPU2);
  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/* USER CODE BEGIN 1 */
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  ULONG currentValue = 0;
  if (GPIO_Pin == GPIO_PIN_4)
  {
    /* Add additional checks to avoid multiple semaphore puts by successively
    clicking on the user button */
    tx_semaphore_info_get(&tx_app_semaphore, NULL, &currentValue, NULL, NULL, NULL);
    if (currentValue == 0)
    {
      /* Put the semaphore to release the MainThread */
      tx_semaphore_put(&tx_app_semaphore);
    }
  }
}
/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(ULONG Delay)
{
  ULONG initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/* USER CODE END 1 */
