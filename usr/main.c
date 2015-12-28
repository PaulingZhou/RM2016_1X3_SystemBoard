/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "spi1.h"
#include "mpu9250.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void NVIC_Config(void);  
void STM_EVAL_COMInit(void);
void USART_Configuration(int BaudRate);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
	NVIC_Config();
	STM_EVAL_COMInit();
	USART_Configuration(9600);
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	MPU9250_Init();
//	Delay(0x3FFFFF);
//	read_mpu9250++;
	//SPI1_Config();

  while (1)
  {
	//	MPU9250_Read_Reg(WHO_AM_I);
		//SPI1_ReadWriteByte(0x25);
//		USART_SendData(USART1,MPU9250_Read_Reg(WHO_AM_I));
		MPU9250_ReadValue();
		Delay(0xFFFF);
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}


void NVIC_Config(void)  
{  
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  /* Enable the USARTx Interrupt */  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);  
} 

/** 
  * @brief  Configures COM port. 
  * @param  COM: Specifies the COM port to be configured. 
  *   This parameter can be one of following parameters:     
  *     @arg COM1 
  *     @arg COM2   
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that 
  *   contains the configuration information for the specified USART peripheral. 
  * @retval None 
  */  
void STM_EVAL_COMInit(void)  
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
  
  /* Enable GPIO clock */  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
  /* Enable UART clock */  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
   
  /* Connect PXx to USARTx_Tx*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);  
  /* Connect PXx to USARTx_Rx*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);  
  
  /* Configure USART Tx as alternate function  */  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  
  /* Configure USART Rx as alternate function  */  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
} 

/******************************************************************************* 
* Function Name  : USART_Configuration 
* Description    : Configures the USART1. 
* Input          : BaudRate 
* Output         : None 
* Return         : None 
*******************************************************************************/ 
void USART_Configuration(int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

#ifdef false 
/******************************************************************************* 
* Function Name  : I2C_Config 
* Description    : Config the I2C1	
* Pin Map				 : PB8-->I2C1_SCL	PB9-->I2C1_SDA 
* Input          : None 
* Output         : None 
* Return         : None 
*******************************************************************************/
void I2C_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	//使能GPIOB&I2C时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_APB1Periph_I2C1, ENABLE);
	
	//使PB8复用功能为I2C的SCL;PB9复用功能为I2C的SDA
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
	//GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		
	//在使用之前先将I2C功能复位
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = ;
	
}
#endif




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
