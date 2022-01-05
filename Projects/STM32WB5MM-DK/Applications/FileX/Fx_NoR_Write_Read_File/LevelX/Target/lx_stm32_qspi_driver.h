/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef LX_STM32_QSPI_DRIVER_H
#define LX_STM32_QSPI_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"
#include "stm32wbxx_hal.h"
#include "s25fl128s.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* The following semaphore is being to notify about RX/TX completion. It needs to be released in the transfer callbacks */
extern TX_SEMAPHORE qspi_tx_semaphore;
extern TX_SEMAPHORE qspi_rx_semaphore;

/* Exported constants --------------------------------------------------------*/

/* the QuadSPI instance, default value set to 0 */
#define LX_STM32_QSPI_INSTANCE                           0
#define LX_STM32_QSPI_DEFAULT_TIMEOUT                    10 * TX_TIMER_TICKS_PER_SECOND
#define LX_STM32_DEFAULT_SECTOR_SIZE                     LX_STM32_QSPI_SECTOR_SIZE

/* Use the QSPI DMA API */
#define LX_STM32_QSPI_DMA_API                            1

/* when set to 1 LevelX is initializing the QuadSPI memory,
 * otherwise it is the up to the application to perform it.
 */
#define LX_STM32_QSPI_INIT                               1

/* allow the driver to fully erase the QuadSPI chip. This should be used carefully.
 * the call is blocking and takes a while. by default it is set to 0.
 */
#define LX_STM32_QSPI_ERASE                              0

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

#define LX_STM32_QSPI_CURRENT_TIME                      tx_time_get

/* Macro called after initializing the QSPI driver
 * e.g. create a semaphore used for transfer notification */
/* USER CODE BEGIN LX_STM32_QSPI_POST_INIT */

#define LX_STM32_QSPI_POST_INIT()                       do { \
                                                           if (tx_semaphore_create(&qspi_tx_semaphore, "qspi rx transfer semaphore", 0) != TX_SUCCESS || \
                                                               tx_semaphore_create(&qspi_rx_semaphore, "qspi tx transfer semaphore", 0) != TX_SUCCESS ) \
                                                           { \
                                                             return LX_ERROR; \
                                                           } \
                                                        } while(0)

/* USER CODE END LX_STM32_QSPI_POST_INIT */

/* Macro called before performing read operation */
/* USER CODE BEGIN LX_STM32_QSPI_PRE_READ_TRANSFER */

#define LX_STM32_QSPI_PRE_READ_TRANSFER(__status__)

/* USER CODE END LX_STM32_QSPI_PRE_READ_TRANSFER */

/* Define how to notify about Read completion operation */
/* USER CODE BEGIN LX_STM32_QSPI_READ_CPLT_NOTIFY */

#define LX_STM32_QSPI_READ_CPLT_NOTIFY(__status__)      do { \
                                                          if(tx_semaphore_get(&qspi_rx_semaphore, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != TX_SUCCESS) \
                                                          { \
                                                            __status__ = LX_ERROR; \
                                                          } \
                                                        } while(0)

/* USER CODE END LX_STM32_QSPI_READ_CPLT_NOTIFY */

/* Macro called after performing read operation */
/* USER CODE BEGIN LX_STM32_QSPI_POST_READ_TRANSFER */

#define LX_STM32_QSPI_POST_READ_TRANSFER(__status__)

/* USER CODE END LX_STM32_QSPI_POST_READ_TRANSFER */

/* Macro for read error handling */
/* USER CODE BEGIN LX_STM32_QSPI_READ_TRANSFER_ERROR */

#define LX_STM32_QSPI_READ_TRANSFER_ERROR(__status__)

/* USER CODE END LX_STM32_QSPI_READ_TRANSFER_ERROR */

/* Macro called before performing write operation */
/* USER CODE BEGIN LX_STM32_QSPI_PRE_WRITE_TRANSFER */

#define LX_STM32_QSPI_PRE_WRITE_TRANSFER(__status__)

/* USER CODE END LX_STM32_QSPI_PRE_WRITE_TRANSFER */

/* Define how to notify about write completion operation */
/* USER CODE BEGIN LX_STM32_QSPI_WRITE_CPLT_NOTIFY */

#define LX_STM32_QSPI_WRITE_CPLT_NOTIFY(__status__)     do { \
                                                          if(tx_semaphore_get(&qspi_tx_semaphore, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != TX_SUCCESS) \
                                                          { \
                                                            __status__ = LX_ERROR; \
                                                          } \
                                                        } while(0)
/* USER CODE END LX_STM32_QSPI_WRITE_CPLT_NOTIFY */

/* Macro called after performing write operation */
/* USER CODE BEGIN LX_STM32_QSPI_POST_WRITE_TRANSFER */

#define LX_STM32_QSPI_POST_WRITE_TRANSFER(__status__)

/* USER CODE END LX_STM32_QSPI_POST_WRITE_TRANSFER */

/* Macro for write error handling */
/* USER CODE BEGIN LX_STM32_QSPI_WRITE_TRANSFER_ERROR */

#define LX_STM32_QSPI_WRITE_TRANSFER_ERROR(__status__)

/* USER CODE END LX_STM32_QSPI_WRITE_TRANSFER_ERROR */

/* Exported functions prototypes ---------------------------------------------*/
INT lx_stm32_qspi_lowlevel_init(UINT instance);
INT lx_stm32_qspi_lowlevel_deinit(UINT instance);

INT lx_stm32_qspi_get_status(UINT instance);
INT lx_stm32_qspi_get_info(UINT instance, ULONG *block_size, ULONG *total_blocks);

INT lx_stm32_qspi_read(UINT instance, ULONG *address, ULONG *buffer, ULONG words);
INT lx_stm32_qspi_write(UINT instance, ULONG *address, ULONG *buffer, ULONG words);

INT lx_stm32_qspi_erase(UINT instance, ULONG block, ULONG erase_count, UINT full_chip_erase);
INT lx_stm32_qspi_is_block_erased(UINT instance, ULONG block);

UINT lx_qspi_driver_system_error(UINT error_code);

UINT lx_stm32_qspi_initialize(LX_NOR_FLASH *nor_flash);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

#define LX_STM32_QSPI_SECTOR_SIZE                 S25FL128S_SECTOR_SIZE
#define LX_STM32_QSPI_FLASH_SIZE                  S25FL128S_FLASH_SIZE
#define LX_STM32_QSPI_STATUS_REG_READY            S25FL128S_FSR_READY
#define LX_STM32_QSPI_DUMMY_CYCLES_READ_QUAD      S25FL128S_DUMMY_CYCLES_READ_QUAD
#define LX_STM32_QSPI_PAGE_SIZE                   S25FL128S_PAGE_SIZE
#define LX_STM32_QSPI_BULK_ERASE_MAX_TIME         S25FL128S_BULK_ERASE_MAX_TIME
#define LX_STM32_QSPI_SECTOR_ERASE_MAX_TIME       S25FL128S_SECTOR_ERASE_MAX_TIME
#define LX_STM32_QSPI_VCR_NB_DUMMY                S25FL128S_VCR_NB_DUMMY
#define LX_STM32_QSPI_SR_WREN                     S25FL128S_SR1_WREN
#define LX_STM32_QSPI_SR_WIP                      S25FL128S_SR1_WIP

#define LX_STM32_QSPI_QUAD_INOUT_FAST_READ_CMD    S25FL128S_QUAD_OUT_FAST_READ_CMD
#define LX_STM32_QSPI_QUAD_IN_FAST_PROG_CMD       S25FL128S_QUAD_IN_FAST_PROG_CMD
#define LX_STM32_QSPI_BULK_ERASE_CMD              S25FL128S_BULK_ERASE_ALTERNATE_CMD
#define LX_STM32_QSPI_SECTOR_ERASE_CMD            S25FL128S_SUBSECTOR_ERASE_CMD_4K
#define LX_STM32_QSPI_RESET_MEMORY_CMD            S25FL128S_SOFTWARE_RESET_CMD
#define LX_STM32_QSPI_WRITE_ENABLE_CMD            S25FL128S_WRITE_ENABLE_CMD
#define LX_STM32_QSPI_READ_STATUS_REG_CMD         S25FL128S_READ_STATUS_REG1_CMD

/* USER CODE BEGIN SECTOR_SIZE */

/* USER CODE END SECTOR_SIZE */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* LX_STM32_QSPI_DRIVER_H */
