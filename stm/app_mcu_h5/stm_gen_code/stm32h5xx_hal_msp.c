/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h5xx_hal_msp.c
 * @author  MCD Application Team
 * @brief   HAL MSP module.
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
#include "stm32h5xx_hal.h"
#include "stm32h5xx_it.h"
/* USER CODE BEGIN Includes */
#include "app_cfg.h"
extern void ErrorHandler(void);
extern DMA_HandleTypeDef hDMAUARTTx;
extern DMA_HandleTypeDef hAdcDMASPITx;
extern DMA_HandleTypeDef hAdcDMASPIRx;
extern DMA_HandleTypeDef hFramDMASPITx;
extern DMA_HandleTypeDef hFramDMASPIRx;
extern DMA_HandleTypeDef hDMAI2CTx;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    /* System interrupt init*/

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (hspi->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */
#if APP_CFG_ENABLE_SPI == 1
        /* USER CODE END SPI1_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
        PeriphClkInitStruct.Spi1ClockSelection = RCC_SPI1CLKSOURCE_PLL1Q;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI1 DMA Init */
        /* GPDMA1_REQUEST_SPI1_RX Init */
        hAdcDMASPIRx.Instance = GPDMA1_Channel7;
        hAdcDMASPIRx.Init.Request = GPDMA1_REQUEST_SPI1_RX;
        hAdcDMASPIRx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hAdcDMASPIRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hAdcDMASPIRx.Init.SrcInc = DMA_SINC_FIXED;
        hAdcDMASPIRx.Init.DestInc = DMA_DINC_INCREMENTED;
        hAdcDMASPIRx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hAdcDMASPIRx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hAdcDMASPIRx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hAdcDMASPIRx.Init.SrcBurstLength = 1;
        hAdcDMASPIRx.Init.DestBurstLength = 1;
        hAdcDMASPIRx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hAdcDMASPIRx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hAdcDMASPIRx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hAdcDMASPIRx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmarx, hAdcDMASPIRx);

        if (HAL_DMA_ConfigChannelAttributes(&hAdcDMASPIRx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* GPDMA1_REQUEST_SPI1_TX Init */
        hAdcDMASPITx.Instance = GPDMA1_Channel6;
        hAdcDMASPITx.Init.Request = GPDMA1_REQUEST_SPI1_TX;
        hAdcDMASPITx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hAdcDMASPITx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hAdcDMASPITx.Init.SrcInc = DMA_SINC_INCREMENTED;
        hAdcDMASPITx.Init.DestInc = DMA_DINC_FIXED;
        hAdcDMASPITx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hAdcDMASPITx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hAdcDMASPITx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hAdcDMASPITx.Init.SrcBurstLength = 1;
        hAdcDMASPITx.Init.DestBurstLength = 1;
        hAdcDMASPITx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hAdcDMASPITx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hAdcDMASPITx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hAdcDMASPITx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmatx, hAdcDMASPITx);

        if (HAL_DMA_ConfigChannelAttributes(&hAdcDMASPITx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* SPI1 interrupt Init */
        HAL_NVIC_SetPriority(SPI1_IRQn, APP_CFG_ADC_SPI_INTR_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
#endif
    }
#if APP_CFG_ENABLE_NVM_SERVICE == 1
#if APP_CFG_USE_ANSI_FRAM != 1
    else if (hspi->Instance == SPI6)
    {
        /* USER CODE BEGIN SPI6_MspInit 0 */

        /* USER CODE END SPI6_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
        PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_PCLK2;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_SPI6_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI6 GPIO Configuration
        PC12     ------> SPI6_SCLK
        PB4     ------> SPI6_MISO
        PB5     ------> SPI6_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* SPI6 DMA Init */
        /* GPDMA1_REQUEST_SPI6_TX Init */
        hFramDMASPITx.Instance = GPDMA2_Channel7;
        hFramDMASPITx.Init.Request = GPDMA2_REQUEST_SPI6_TX;
        hFramDMASPITx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hFramDMASPITx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hFramDMASPITx.Init.SrcInc = DMA_SINC_INCREMENTED;
        hFramDMASPITx.Init.DestInc = DMA_DINC_FIXED;
        hFramDMASPITx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hFramDMASPITx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hFramDMASPITx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hFramDMASPITx.Init.SrcBurstLength = 1;
        hFramDMASPITx.Init.DestBurstLength = 1;
        hFramDMASPITx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hFramDMASPITx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hFramDMASPITx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hFramDMASPITx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmatx, hFramDMASPITx);

        if (HAL_DMA_ConfigChannelAttributes(&hFramDMASPITx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* GPDMA1_REQUEST_SPI6_RX Init */
        hFramDMASPIRx.Instance = GPDMA2_Channel6;
        hFramDMASPIRx.Init.Request = GPDMA2_REQUEST_SPI6_RX;
        hFramDMASPIRx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hFramDMASPIRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hFramDMASPIRx.Init.SrcInc = DMA_SINC_FIXED;
        hFramDMASPIRx.Init.DestInc = DMA_DINC_INCREMENTED;
        hFramDMASPIRx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hFramDMASPIRx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hFramDMASPIRx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hFramDMASPIRx.Init.SrcBurstLength = 1;
        hFramDMASPIRx.Init.DestBurstLength = 1;
        hFramDMASPIRx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hFramDMASPIRx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hFramDMASPIRx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hFramDMASPIRx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmarx, hFramDMASPIRx);

        if (HAL_DMA_ConfigChannelAttributes(&hFramDMASPIRx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* SPI6 interrupt Init */
        HAL_NVIC_SetPriority(SPI6_IRQn, APP_CFG_FRAM_SPI_INTR_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(SPI6_IRQn);
        /* USER CODE BEGIN SPI6_MspInit 1 */
        /* USER CODE END SPI6_MspInit 1 */
    }
#else
    else if (hspi->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI3;
        PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI3CLKSOURCE_PLL1Q;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PD6     ------> SPI3_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* SPI3 DMA Init */
        /* GPDMA1_REQUEST_SPI3_TX Init */
        hFramDMASPITx.Instance = GPDMA2_Channel7;
        hFramDMASPITx.Init.Request = GPDMA2_REQUEST_SPI3_TX;
        hFramDMASPITx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hFramDMASPITx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hFramDMASPITx.Init.SrcInc = DMA_SINC_INCREMENTED;
        hFramDMASPITx.Init.DestInc = DMA_DINC_FIXED;
        hFramDMASPITx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hFramDMASPITx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hFramDMASPITx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hFramDMASPITx.Init.SrcBurstLength = 1;
        hFramDMASPITx.Init.DestBurstLength = 1;
        hFramDMASPITx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hFramDMASPITx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hFramDMASPITx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hFramDMASPITx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmatx, hFramDMASPITx);

        if (HAL_DMA_ConfigChannelAttributes(&hFramDMASPITx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* GPDMA1_REQUEST_SPI3_RX Init */
        hFramDMASPIRx.Instance = GPDMA2_Channel6;
        hFramDMASPIRx.Init.Request = GPDMA2_REQUEST_SPI3_RX;
        hFramDMASPIRx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hFramDMASPIRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hFramDMASPIRx.Init.SrcInc = DMA_SINC_FIXED;
        hFramDMASPIRx.Init.DestInc = DMA_DINC_INCREMENTED;
        hFramDMASPIRx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hFramDMASPIRx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hFramDMASPIRx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hFramDMASPIRx.Init.SrcBurstLength = 1;
        hFramDMASPIRx.Init.DestBurstLength = 1;
        hFramDMASPIRx.Init.TransferAllocatedPort =
            DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hFramDMASPIRx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hFramDMASPIRx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hFramDMASPIRx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmarx, hFramDMASPIRx);

        if (HAL_DMA_ConfigChannelAttributes(&hFramDMASPIRx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            Error_Handler();
        }

        /* SPI3 interrupt Init */
        HAL_NVIC_SetPriority(SPI3_IRQn, APP_CFG_FRAM_SPI_INTR_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
        /* USER CODE BEGIN SPI3_MspInit 1 */
        /* USER CODE END SPI3_MspInit 1 */
    }
#endif
#endif
}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
#if APP_CFG_ENABLE_HOST_UART == 1
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (huart->Instance == UART5)
    {
        /* USER CODE BEGIN UART5_MspInit 0 */

        /* USER CODE END UART5_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5;
        PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_UART5_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**UART5 GPIO Configuration
        PB13     ------> UART5_TX
        PB15     ------> UART5_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF14_UART5;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* UART5 interrupt Init */
        HAL_NVIC_SetPriority(UART5_IRQn, APP_CFG_UART_INTR_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(UART5_IRQn);

        /* UART5 DMA Init */
        hDMAUARTTx.Instance = GPDMA1_Channel5;
        hDMAUARTTx.Init.Request = GPDMA1_REQUEST_UART5_TX;
        hDMAUARTTx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
        hDMAUARTTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hDMAUARTTx.Init.SrcInc = DMA_SINC_INCREMENTED;
        hDMAUARTTx.Init.DestInc = DMA_DINC_FIXED;
        hDMAUARTTx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
        hDMAUARTTx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
        hDMAUARTTx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
        hDMAUARTTx.Init.SrcBurstLength = 1;
        hDMAUARTTx.Init.DestBurstLength = 1;
        hDMAUARTTx.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT0;
        hDMAUARTTx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
        hDMAUARTTx.Init.Mode = DMA_NORMAL;
        if (HAL_DMA_Init(&hDMAUARTTx) != HAL_OK)
        {
            ErrorHandler();
        }
        __HAL_LINKDMA(huart, hdmatx, hDMAUARTTx);
        if (HAL_DMA_ConfigChannelAttributes(&hDMAUARTTx, DMA_CHANNEL_NPRIV) != HAL_OK)
        {
            ErrorHandler();
        }
    }
}
#endif

/* USER CODE BEGIN 1 */
/**
 * @brief CRC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
    if (hcrc->Instance == CRC)
    {
        /* USER CODE BEGIN CRC_MspInit 0 */

        /* USER CODE END CRC_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_CRC_CLK_ENABLE();
        /* USER CODE BEGIN CRC_MspInit 1 */

        /* USER CODE END CRC_MspInit 1 */
    }
}

/**
 * @brief TIM_PWM MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_pwm: TIM_PWM handle pointer
 * @retval None
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim_pwm)
{
    if (htim_pwm->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else if (htim_base->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

#if APP_CFG_ENABLE_DISPLAY == 1
/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB12     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C2 DMA Init */
    /* GPDMA1_REQUEST_I2C2_TX Init */
    hDMAI2CTx.Instance = GPDMA1_Channel4;
    hDMAI2CTx.Init.Request = GPDMA1_REQUEST_I2C2_TX;
    hDMAI2CTx.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    hDMAI2CTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hDMAI2CTx.Init.SrcInc = DMA_SINC_INCREMENTED;
    hDMAI2CTx.Init.DestInc = DMA_DINC_FIXED;
    hDMAI2CTx.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
    hDMAI2CTx.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    hDMAI2CTx.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    hDMAI2CTx.Init.SrcBurstLength = 1;
    hDMAI2CTx.Init.DestBurstLength = 1;
    hDMAI2CTx.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT0;
    hDMAI2CTx.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    hDMAI2CTx.Init.Mode = DMA_NORMAL;
    if (HAL_DMA_Init(&hDMAI2CTx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2c, hdmatx, hDMAI2CTx);

    if (HAL_DMA_ConfigChannelAttributes(&hDMAI2CTx, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
      Error_Handler();
    }

    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, APP_CFG_I2C_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, APP_CFG_I2C_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
    /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */

  }

}
#endif
/* USER CODE END 1 */
