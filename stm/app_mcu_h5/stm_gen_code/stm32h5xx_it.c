/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    SPI/SPI_FullDuplex_ComIT_Master/Src/stm32h5xx_it.c
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32h5xx_it.h"
#include "stm32h5xx_hal.h"
#include "app_cfg.h"
#include "adi_evb.h"
#if APP_CFG_ENABLE_CF_SERVICE == 1
#include "met_cf_pulse.h"
#include "stm32h563xx_timer_config.h"
#endif
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
#if APP_CFG_ENABLE_SPI==1
extern SPI_HandleTypeDef hSPI;
#endif
#if APP_CFG_ENABLE_SPI == 1
extern SPI_HandleTypeDef hFramSpi;
#endif
#if APP_CFG_ENABLE_HOST_UART == 1
extern UART_HandleTypeDef hUART;
#endif
#if APP_CFG_ENABLE_DISPLAY == 1
extern I2C_HandleTypeDef hI2C;
#endif
extern DMA_HandleTypeDef hAdcDMASPITx;
extern DMA_HandleTypeDef hAdcDMASPIRx;
extern DMA_HandleTypeDef hFramDMASPITx;
extern DMA_HandleTypeDef hFramDMASPIRx;
extern DMA_HandleTypeDef hDMAUARTTx;
extern DMA_HandleTypeDef hDMAI2CTx;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

#ifndef USE_FREERTOS
/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}
#endif

/******************************************************************************/
/* STM32H5xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h5xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles SPI1 global interrupt.
 */
void SPI1_IRQHandler(void)
{
#if APP_CFG_ENABLE_SPI==1
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    HAL_SPI_IRQHandler(&hSPI);
    /* USER CODE BEGIN SPI1_IRQn 1 */

    /* USER CODE END SPI1_IRQn 1 */
#endif
}

/**
 * @brief This function handles SPI1 global interrupt.
 */
void SPI6_IRQHandler(void)
{
#if APP_CFG_ENABLE_NVM_SERVICE==1
    /* USER CODE BEGIN SPI1_IRQn 0 */

    /* USER CODE END SPI1_IRQn 0 */
    HAL_SPI_IRQHandler(&hFramSpi);
    /* USER CODE BEGIN SPI1_IRQn 1 */
    /* USER CODE END SPI1_IRQn 1 */
#endif
}

/**
  * @brief This function handles SPI3 global interrupt.
  */
void SPI3_IRQHandler(void)
{
  /* USER CODE BEGIN SPI3_IRQn 0 */
#if APP_CFG_ENABLE_NVM_SERVICE==1
  /* USER CODE END SPI3_IRQn 0 */
  HAL_SPI_IRQHandler(&hFramSpi);
  /* USER CODE BEGIN SPI3_IRQn 1 */
#endif
  /* USER CODE END SPI3_IRQn 1 */
}

/**
 * @brief This function handles UART5 global interrupt.
 */
void UART5_IRQHandler(void)
{
#if APP_CFG_ENABLE_HOST_UART == 1
    /* USER CODE BEGIN UART5_IRQn 0 */
    /* USER CODE END UART5_IRQn 0 */
    HAL_UART_IRQHandler(&hUART);
    /* USER CODE BEGIN UART5_IRQn 1 */

    /* USER CODE END UART5_IRQn 1 */
#endif
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    /* Toggle LED3 for error */
    while (1)
    {
        HAL_Delay(1000);
    }
}

#if APP_CFG_ENABLE_SPI == 1
/**
 * @brief This function handles GPDMA1 Channel 6 global interrupt.
 */
void GPDMA1_Channel6_IRQHandler(void)
{
    /* USER CODE BEGIN GPDMA1_Channel6_IRQn 0 */

    /* USER CODE END GPDMA1_Channel6_IRQn 0 */
    HAL_DMA_IRQHandler(&hAdcDMASPITx);
    /* USER CODE BEGIN GPDMA1_Channel6_IRQn 1 */

    /* USER CODE END GPDMA1_Channel6_IRQn 1 */
}

/**
 * @brief This function handles GPDMA1 Channel 7 global interrupt.
 */
void GPDMA1_Channel7_IRQHandler(void)
{
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 0 */

    /* USER CODE END GPDMA1_Channel7_IRQn 0 */
    HAL_DMA_IRQHandler(&hAdcDMASPIRx);
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 1 */

    /* USER CODE END GPDMA1_Channel7_IRQn 1 */
}
#endif

#if APP_CFG_ENABLE_SPI == 1
/**
 * @brief This function handles GPDMA1 Channel 7 global interrupt.
 */
void GPDMA2_Channel7_IRQHandler(void)
{
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 0 */

    /* USER CODE END GPDMA1_Channel7_IRQn 0 */
    HAL_DMA_IRQHandler(&hFramDMASPITx);
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 1 */

    /* USER CODE END GPDMA1_Channel7_IRQn 1 */
}

/**
 * @brief This function handles GPDMA1 Channel 7 global interrupt.
 */
void GPDMA2_Channel6_IRQHandler(void)
{
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 0 */

    /* USER CODE END GPDMA1_Channel7_IRQn 0 */
    HAL_DMA_IRQHandler(&hFramDMASPIRx);
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 1 */

    /* USER CODE END GPDMA1_Channel7_IRQn 1 */
}
#endif

/**
 * @brief This function handles GPDMA1 Channel 5 global interrupt.
 */
#if APP_CFG_ENABLE_HOST_UART == 1
void GPDMA1_Channel5_IRQHandler(void)
{
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 0 */
    /* USER CODE END GPDMA1_Channel7_IRQn 0 */
    HAL_DMA_IRQHandler(&hDMAUARTTx);
    /* USER CODE BEGIN GPDMA1_Channel7_IRQn 1 */

    /* USER CODE END GPDMA1_Channel7_IRQn 1 */
}
#endif

#if APP_CFG_ENABLE_CF_SERVICE == 1
/**
 * @brief IRQ Handler for CF1 timer
 *
 */
void TIM5_IRQHandler(void)
{
    if (StmCf1Handler())
    {
        Cf1Callback();
    }
}
#endif
/* USER CODE BEGIN 1 */

#if APP_CFG_ENABLE_DISPLAY == 1
/**
  * @brief This function handles GPDMA1 Channel 4 global interrupt.
  */
void GPDMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN GPDMA1_Channel4_IRQn 0 */

  /* USER CODE END GPDMA1_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&hDMAI2CTx);
  /* USER CODE BEGIN GPDMA1_Channel4_IRQn 1 */

  /* USER CODE END GPDMA1_Channel4_IRQn 1 */
}

/**
  * @brief This function handles I2C2 Event interrupt.
  */
void I2C2_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_EV_IRQn 0 */

  /* USER CODE END I2C2_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hI2C);
  /* USER CODE BEGIN I2C2_EV_IRQn 1 */
  EvbHandlerI2CTx();
  /* USER CODE END I2C2_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C2 Error interrupt.
  */
void I2C2_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_ER_IRQn 0 */

  /* USER CODE END I2C2_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hI2C);
  /* USER CODE BEGIN I2C2_ER_IRQn 1 */
  EvbHandlerI2CTx();
  /* USER CODE END I2C2_ER_IRQn 1 */
}
#endif
/* USER CODE END 1 */
