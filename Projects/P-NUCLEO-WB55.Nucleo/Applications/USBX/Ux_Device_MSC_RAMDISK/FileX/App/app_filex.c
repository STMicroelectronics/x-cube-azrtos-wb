/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_filex.c
  * @author  MCD Application Team
  * @brief   FileX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_filex.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define RAM_DISK_SIZE                           ((300 * 1024) - 512)
#define FX_RAM_DISK_TOTAL_SECTORS               (RAM_DISK_SIZE / 512)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_STACK_SIZE               (1 * 1024)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
FX_MEDIA        ram_disk;
TX_THREAD       fx_ramdisk_thread;
uint32_t media_memory[FX_RAM_DISK_TOTAL_SECTORS / sizeof(uint32_t)];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void fx_ramdisk_thread_entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application FileX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_FileX_Init(VOID *memory_ptr)
{
  UINT ret = FX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_FileX_MEM_POOL */

  /* USER CODE END MX_FileX_MEM_POOL */

  /* USER CODE BEGIN MX_FileX_Init */
  VOID *pointer;

  /* Allocate memory for the main thread's stack */
  if (tx_byte_allocate(byte_pool, &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread.  */
   if (tx_thread_create(&fx_ramdisk_thread, "FileX App Thread",
                        fx_ramdisk_thread_entry, 0, pointer, DEFAULT_STACK_SIZE,
                        10, 10, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Initialize FileX. */
  fx_system_initialize();

  /* USER CODE END MX_FileX_Init */
  return ret;
}

/* USER CODE BEGIN 1 */
void fx_ramdisk_thread_entry(ULONG thread_input)
{
  UINT        status;


  /* Format the RAM disk */
  status = fx_media_format(&ram_disk,
                           fx_stm32_sram_driver,        /* Driver entry */
                           (VOID *)FX_NULL,             /*Device info pointer */
                           (UCHAR *) media_memory,      /* Media buffer pointer */
                           sizeof(media_memory),        /* Media buffer size */
                           "MY_RAM_DISK",               /* Volume Name */
                           2,                           /* Number of FATs */
                           512,                         /* Directory Entries */
                           0,                           /* Hidden sectors */
                           FX_RAM_DISK_TOTAL_SECTORS,   /* Total sectors */
                           512,                         /* Sector size */
                           4,                           /* Sectors per cluster */
                           1,                           /* Heads */
                           1);                          /* Sectors per track */

  if (status != FX_SUCCESS)
  {
    Error_Handler();
  }

  /* Open the RAM disk */
  status = fx_media_open(&ram_disk, "RAM DISK", fx_stm32_sram_driver,
                         NULL, media_memory, sizeof(media_memory));

  /* Check the media open status */
  if (status != FX_SUCCESS)
  {
    Error_Handler();
  }

}
/* USER CODE END 1 */
