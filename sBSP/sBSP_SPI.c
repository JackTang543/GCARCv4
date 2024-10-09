#include "sBSP_SPI.h"

#include "sDBG_Debug.h"

//sBSP_SPI.c 20241009 v1
//used for sGCARCv4



/*SPI1 -> OLED*/
#define OLED_SCK_GPIO_Port      GPIOA
#define OLED_SCK_Pin            GPIO_PIN_5
#define OLED_DC_GPIO_Port       GPIOA
#define OLED_DC_Pin             GPIO_PIN_6
#define OLED_SDA_GPIO_Port      GPIOA
#define OLED_SDA_Pin            GPIO_PIN_7
#define OLED_CS_GPIO_Port       GPIOA
#define OLED_CS_Pin             GPIO_PIN_15
SPI_HandleTypeDef hspi1;

/*SPI2 -> IMU*/
#define IMU_SCK_GPIO_Port       GPIOB
#define IMU_SCK_Pin             GPIO_PIN_13
#define IMU_MISO_GPIO_Port      GPIOC
#define IMU_MISO_Pin            GPIO_PIN_2
#define IMU_MOSI_GPIO_Port      GPIOC
#define IMU_MOSI_Pin            GPIO_PIN_3

SPI_HandleTypeDef hspi2;
#define IMU_SPI_HANDLE          hspi2


/*SPI3 -> TRACK*/
#define TRACK_SCK_GPIO_Port     GPIOC
#define TRACK_SCK_Pin           GPIO_PIN_10
#define TRACK_MISO_GPIO_Port    GPIOC
#define TRACK_MISO_Pin          GPIO_PIN_11
#define TRACK_MOSI_GPIO_Port    GPIOC
#define TRACK_MOSI_Pin          GPIO_PIN_12
#define TRACK_CS_GPIO_Port      GPIOD
#define TRACK_CS_Pin            GPIO_PIN_2
SPI_HandleTypeDef hspi3;


//SPI_BAUDRATEPRESCALER_2 ~ SPI_BAUDRATEPRESCALER_256
void sBSP_SPI_IMU_Init(uint32_t SPI_BAUDRATE){
    IMU_SPI_HANDLE.Instance              = SPI2;
    IMU_SPI_HANDLE.Init.Mode             = SPI_MODE_MASTER;
    IMU_SPI_HANDLE.Init.Direction        = SPI_DIRECTION_2LINES;
    IMU_SPI_HANDLE.Init.DataSize         = SPI_DATASIZE_8BIT;
    IMU_SPI_HANDLE.Init.CLKPolarity      = SPI_POLARITY_LOW;
    IMU_SPI_HANDLE.Init.CLKPhase         = SPI_PHASE_1EDGE;
    IMU_SPI_HANDLE.Init.NSS              = SPI_NSS_SOFT;
    IMU_SPI_HANDLE.Init.BaudRatePrescaler= SPI_BAUDRATE;
    IMU_SPI_HANDLE.Init.FirstBit         = SPI_FIRSTBIT_MSB;
    IMU_SPI_HANDLE.Init.TIMode           = SPI_TIMODE_DISABLE;
    IMU_SPI_HANDLE.Init.CRCCalculation   = SPI_CRCCALCULATION_DISABLE;
    IMU_SPI_HANDLE.Init.CRCPolynomial    = 10;

    if (HAL_SPI_Init(&IMU_SPI_HANDLE) != HAL_OK){
        Error_Handler();
    }
}

void sBSP_SPI_IMU_SendByte(uint8_t byte){
    HAL_SPI_Transmit(&IMU_SPI_HANDLE,&byte,1,100);
}

uint8_t sBSP_SPI_IMU_RecvByte(){
    uint8_t send_byte = 0;
    HAL_SPI_Receive (&IMU_SPI_HANDLE,&send_byte,1,100);
    return send_byte;
}

void sBSP_SPI_IMU_SendBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Transmit(&hspi1,pData,Size,1000);
}

void sBSP_SPI_IMU_RecvBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Receive(&hspi1,pData,Size,1000);
}



void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    PA15     ------> SPI1_NSS
    */
    GPIO_InitStruct.Pin = OLED_SCK_Pin|OLED_SDA_Pin|OLED_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
  else if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    PB13     ------> SPI2_SCK
    */
    GPIO_InitStruct.Pin = IMU_MISO_Pin|IMU_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IMU_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(IMU_SCK_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
  else if(spiHandle->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspInit 0 */

  /* USER CODE END SPI3_MspInit 0 */
    /* SPI3 clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    GPIO_InitStruct.Pin = TRACK_SCK_Pin|TRACK_MISO_Pin|TRACK_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI3_MspInit 1 */

  /* USER CODE END SPI3_MspInit 1 */
  }
}

void sBSP_SPI1M_SetCS(uint8_t cs_en){
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(GPIO_PinState)cs_en);
}

void sBSP_SPI1M_SendBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Transmit(&hspi1,pData,Size,1000);
}

void sBSP_SPI1M_ReciBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Receive(&hspi1,pData,Size,1000);
}

void sBSP_SPI1M_SendByte(uint8_t byte){
    HAL_SPI_Transmit(&hspi1,&byte,1,100);
}

uint8_t sBSP_SPI1M_ReciByte(){
    uint8_t send_byte = 0;
    HAL_SPI_Receive (&hspi1,&send_byte,1,100);
    return send_byte;
}

void sBSP_SPI1M_SetEN(uint8_t en){
    if(en){
        __HAL_SPI_ENABLE(&hspi1);
    }else{
        __HAL_SPI_DISABLE(&hspi1);
    }
}


void sBSP_SPI2M_SendBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Transmit(&hspi2,pData,Size,1000);
}

void sBSP_SPI2M_ReciBytes(uint8_t *pData,uint16_t Size){
    HAL_SPI_Receive(&hspi2,pData,Size,1000);
}

void sBSP_SPI2M_SendByte(uint8_t byte){
    HAL_SPI_Transmit(&hspi2,&byte,1,100);
}

uint8_t sBSP_SPI2M_ReciByte(){
    uint8_t send_byte = 0;
    HAL_SPI_Receive (&hspi2,&send_byte,1,100);
    return send_byte;
}


