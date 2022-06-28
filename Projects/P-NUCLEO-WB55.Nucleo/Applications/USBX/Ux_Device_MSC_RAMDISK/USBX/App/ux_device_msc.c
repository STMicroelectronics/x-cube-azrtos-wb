/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_msc.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
#include "ux_device_msc.h"

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern FX_MEDIA        ram_disk;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Obtain the status of the device.
  * @param  storage: storage instance
  * @param  lun: Logical unit number
  * @param  media_id: media id
  * @param  media_status: status
  * @retval status
  */
UINT STORAGE_Status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status)
{
  return (UX_SUCCESS);
}

/**
  * @brief  Reads data from the medium.
  * @param  storage : Not used
  * @param  lun: Logical unit number
  * @param  lba: Logical block address
  * @param  number_blocks: Blocks number
  * @param  data_pointer: Data
  * @param  media_status: Not used
  * @retval Status
  */
UINT STORAGE_Read(VOID *storage, ULONG lun, UCHAR *data_pointer,
                  ULONG number_blocks, ULONG lba, ULONG *media_status)
{
  UINT status = 0;

  while(number_blocks--)
  {
    status =  fx_media_read(&ram_disk, lba, data_pointer);
    data_pointer += 512;
    lba++;
  }

  return(status);
}

/**
  * @brief  Writes data into the medium.
  * @param  storage : Not used
  * @param  lun: Logical unit number
  * @param  lba: Logical block address
  * @param  number_blocks: Blocks number
  * @param  data_pointer: Data
  * @param  media_status: Not used
  * @retval Status
  */
UINT STORAGE_Write(VOID *storage, ULONG lun, UCHAR *data_pointer,
                   ULONG number_blocks, ULONG lba, ULONG *media_status)
{
  UINT status = 0;

  while(number_blocks--)
  {
    status =  fx_media_write(&ram_disk, lba, data_pointer);
    data_pointer += 512;
    lba++;
  }

  return(status);
}
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
