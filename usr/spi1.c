/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "spi1.h"


/******************************************************************************* 
* Function Name  : SPI_Config 
* Description    : Config the SPI1	
* Pin Map				 : PB3-->SPI1_SCK;	
*									 PB4-->SPI1_MISO; 	
* 								 PB5-->SPI1_MOSI; 
* Input          : None 
* Output         : None 
* Return         : None 
*******************************************************************************/
 void SPI1_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_I2S_DeInit(SPI1);

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);		//ʹ��SPI1ʱ��
	
	//GPIOPB3,4,5��ʼ�����ã����ù������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);	//PB3����ΪSPI1_SCK
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);	//PB4����ΪSPI1_MISO
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);	//PB5����ΪSPI1_MOSI
	
	//Init SPI1 & Set Working Mode
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;													//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												//�ڶ������������ݲ������½��أ�
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//Ԥ��Ƶ4�������ٶ�84M/32=2.625Mhz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
	
	SPI1_ReadWriteByte(0xFF);
}

/******************************************************************************* 
* Function Name  : SPI_SetSpeed 
* Description    : SPI Speed = fAPB2/��Ƶϵ��;fAPB2ʱ��һ��Ϊ84Mhz	 
* Input          : @ref SPI_BaudRate_Prescaler
*									 SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
* Output         : None 
* Return         : None 
*******************************************************************************/
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//�ж���Ч��
	SPI1->CR1 &= 0xFFC7;		//λ3~5���㣬�������ò�����
	SPI1->CR2 |= SPI_BaudRatePrescaler;		//����SPI1�ٶ�
	SPI_Cmd(SPI1,ENABLE);
}

/******************************************************************************* 
* Function Name  : SPI_ReadWriteByte(u8 TxData) 
* Description    : ��д����	 
* Input          : TxData
* Output         : RxData 
* Return         : RxData 
*******************************************************************************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
	uint8_t retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPI1,TxData);
	retry = 0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPI1);
}
