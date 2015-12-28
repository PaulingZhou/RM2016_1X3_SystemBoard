# include "mpu9250.h"
# include "spi1.h"

S_INT16_XYZ MPU9250_ACC_LAST;
S_INT16_XYZ MPU9250_GYRO_LAST;
S_INT16_XYZ MPU9250_MAG_LAST;
int16_t			MPU9250_TEMP_LAST;

//PB8 --> nCS
uint8_t MPU9250_Init(void)
{
//	uint8_t readData = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	SPI1_Config();	//MPU9250 SPI初始化
	if(MPU9250_Read_Reg(WHO_AM_I)==0x73){
		MPU9250_Write_Reg(USER_CTRL,0X10); //使能MPU9250SPI
		MPU9250_Write_Reg(PWR_MGMT_1,0X80);  //电源管理，复位MPU9250
		MPU9250_Write_Reg(SMPLRT_DIV,0x07);//采样率1000/(1+7)=125HZ
		MPU9250_Write_Reg(CONFIG,0X06);				//低通滤波器 0x06 5hz
		MPU9250_Write_Reg(GYRO_CONFIG,0X18);  //陀螺仪测量范围 0X18 ??2000?
		MPU9250_Write_Reg(ACCEL_CONFIG,0x18); //加速度计测量范围 0X18 ??16g
		return 1;
	}
	return 0;
}

//SPI读取指定寄存器
//reg:指定寄存器的地址
uint8_t MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	reg_val = SPI1_ReadWriteByte(reg|0x80);	 //发送读命令+寄存器号
	reg_val = SPI1_ReadWriteByte(0xFF);//读取寄存器值
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	return(reg_val);
}

//SPI写寄存器
//reg:指定的寄存器地址
//value:写入的值
uint8_t MPU9250_Write_Reg(uint8_t reg,uint8_t value)
{
	u8 status;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //使能SPI传输
	status=SPI1_ReadWriteByte(reg); //发送写命令+寄存器号
	SPI1_ReadWriteByte(value);//写入寄存器值
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //禁止MPU9250
	return(status);//返回状态值
}

//读取MPU9250数据
void MPU9250_ReadValue(void)
{
	u8 MPU9250_buf[14],i;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //使能SPI传输
	SPI1_ReadWriteByte(ACCEL_XOUT_H|0x80); //发送读命令+寄存器号
	for(i=0;i<14;i++)//一共读取14字节的数据
	{
		MPU9250_buf[i]=SPI1_ReadWriteByte(0xff); //循环读取
		
	}
	
	/*MPU9250_buf[0]=SPI2_ReadWriteByte(0xff);
	
	SPI2_ReadWriteByte(ACCEL_XOUT_L|0x80); 
	MPU9250_buf[1]=SPI2_ReadWriteByte(0xff);
	MPU9250_ACC_LAST.X = ((int16_t)MPU9250_buf[0]<<8) + MPU9250_buf[1];*/
	
	MPU9250_ACC_LAST.X = ((int16_t)MPU9250_buf[0]<<8) | MPU9250_buf[1];
	MPU9250_ACC_LAST.Y = ((int16_t)MPU9250_buf[2]<<8) | MPU9250_buf[3];
	MPU9250_ACC_LAST.Z = ((int16_t)MPU9250_buf[4]<<8) | MPU9250_buf[5];
	
	MPU9250_TEMP_LAST =  ((int16_t)MPU9250_buf[6]<<8) | MPU9250_buf[7];
	
	MPU9250_GYRO_LAST.X = ((int16_t)MPU9250_buf[8]<<8) | MPU9250_buf[9];
	MPU9250_GYRO_LAST.Y = ((int16_t)MPU9250_buf[10]<<8) | MPU9250_buf[11];
	MPU9250_GYRO_LAST.Z = ((int16_t)MPU9250_buf[12]<<8) | MPU9250_buf[13];
	USART_SendData(USART1,MPU9250_GYRO_LAST.X);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //禁止MPU9250
}
