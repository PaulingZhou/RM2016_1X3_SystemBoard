#include <stdlib.h> 
#include <stdio.h> 
# include "mpu9250.h"
# include "spi1.h"

//S_INT16_XYZ MPU9250_ACC_LAST;
//S_INT16_XYZ MPU9250_GYRO_LAST;
//S_INT16_XYZ MPU9250_MAG_LAST;
//int16_t			MPU9250_TEMP_LAST;

//PB8 --> nCS
uint8_t MPU9250_Init(void)
{
//	uint8_t readData = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	//ʹ��GPIOBʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	SPI1_Config();	//MPU9250 SPI��ʼ��
	if(MPU9250_Read_Reg(WHO_AM_I)==0x73){
		MPU9250_Write_Reg(USER_CTRL,0X10); //ʹ��MPU9250SPI
		MPU9250_Write_Reg(PWR_MGMT_1,0X80);  //��Դ������λMPU9250
		MPU9250_Write_Reg(SMPLRT_DIV,0x07);//������1000/(1+7)=125HZ
		MPU9250_Write_Reg(CONFIG,0X06);				//���������¶ȵ�ͨ�˲��� 0x06 5hz
		MPU9250_Write_Reg(GYRO_CONFIG,0X18);  //�����ǲ�����Χ 0X18 +2000 dps
		MPU9250_Write_Reg(ACCEL_CONFIG,0x18); //���ٶȼƲ�����Χ 0X18 +-16g
		MPU9250_Write_Reg(ACCEL_CONFIG2,0x0E);	//���ٶȼƵ�ͨ�˲�����5Hz
		return 1;
	}
	return 0;
}

//SPI��ȡָ���Ĵ���
//reg:ָ���Ĵ����ĵ�ַ
uint8_t MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);					//ʹ��SPI����
	reg_val = SPI1_ReadWriteByte(reg|0x80);	 	//���Ͷ�����+�Ĵ�����
 	reg_val = SPI1_ReadWriteByte(0xFF);				//��ȡ�Ĵ���ֵ
	GPIO_SetBits(GPIOB,GPIO_Pin_8);						//��ֹSPI����
	return(reg_val);
}

//SPIд�Ĵ���
//reg:ָ���ļĴ�����ַ
//value:д���ֵ
uint8_t MPU9250_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //ʹ��SPI����
	status=SPI1_ReadWriteByte(reg); //����д����+�Ĵ�����
	SPI1_ReadWriteByte(value);//д��Ĵ���ֵ
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //��ֹMPU9250
	return(status);//����״ֵ̬
}

////��ȡMPU9250����
//void MPU9250_ReadValue(void)
//{
//	uint8_t MPU9250_buf[14],i;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //ʹ��SPI����
//	SPI1_ReadWriteByte(ACCEL_XOUT_H|0x80);
//	for(i=0;i<14;i++)//һ����ȡ14�ֽڵ�����
//	{
//		MPU9250_buf[i]=SPI1_ReadWriteByte(0xFF); //ѭ����ȡ
//	}
//	//���ٶȼ�
//	MPU9250_ACC_LAST.X = ((int16_t)MPU9250_buf[0]<<8) | MPU9250_buf[1];
//	MPU9250_ACC_LAST.Y = ((int16_t)MPU9250_buf[2]<<8) | MPU9250_buf[3];
//	MPU9250_ACC_LAST.Z = ((int16_t)MPU9250_buf[4]<<8) | MPU9250_buf[5];
//	//�¶�
//	MPU9250_TEMP_LAST =  ((int16_t)MPU9250_buf[6]<<8) | MPU9250_buf[7];
//	//������
//	MPU9250_GYRO_LAST.X = ((int16_t)MPU9250_buf[8]<<8) | MPU9250_buf[9];
//	MPU9250_GYRO_LAST.Y = ((int16_t)MPU9250_buf[10]<<8) | MPU9250_buf[11];
//	MPU9250_GYRO_LAST.Z = ((int16_t)MPU9250_buf[12]<<8) | MPU9250_buf[13];
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //��ֹMPU9250
//}

void MPU9250_ReadValue(uint8_t *dataPointer)
{
	uint8_t i;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //ʹ��SPI����
	SPI1_ReadWriteByte(ACCEL_XOUT_H|0x80);
	for(i=0;i<14;i++)//һ����ȡ14�ֽڵ�����
	{
		*dataPointer = SPI1_ReadWriteByte(0xFF); //ѭ����ȡ
		dataPointer = dataPointer+1;
	}
	//	//���ٶȼ�
//	MPU9250_ACC_LAST.X = ((int16_t)(*MPU9250_buf)[0]<<8) | (*MPU9250_buf)[1];
//	MPU9250_ACC_LAST.Y = ((int16_t)(*MPU9250_buf)[2]<<8) | (*MPU9250_buf)[3];
//	MPU9250_ACC_LAST.Z = ((int16_t)(*MPU9250_buf)[4]<<8) | (*MPU9250_buf)[5];
//	//�¶�
//	MPU9250_TEMP_LAST =  ((int16_t)(*MPU9250_buf)[6]<<8) | (*MPU9250_buf)[7];
//	//������
//	MPU9250_GYRO_LAST.X = ((int16_t)(*MPU9250_buf)[8]<<8) | (*MPU9250_buf)[9];
//	MPU9250_GYRO_LAST.Y = ((int16_t)(*MPU9250_buf)[10]<<8) | (*MPU9250_buf)[11];
//	MPU9250_GYRO_LAST.Z = ((int16_t)(*MPU9250_buf)[12]<<8) | (*MPU9250_buf)[13];
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //��ֹMPU9250
}
