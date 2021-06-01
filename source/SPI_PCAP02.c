/*****************************************************************************

  * @file    SPI_interface.c
  * @author  陆阳天宇
  * @version V1.0
  * @date    2020-09-23
  * @brief   SPI_PCAP02 底层应用函数 

******************************************************************************/


/************* 添加头文件 ******************/ 
#include "SPI_PCAP02.h"
#include "stm32f4xx_gpio.h"
#include <math.h>



/******************** 静态预处理宏定义 **********************/
#define Dummy_Byte							0xFF           //为了产生时钟发送的假数据


/****************************************************************************/
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT; 
 
// static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/************************** 声明全局计数变量 *****************************************/
int         i;
int         j;

					
		
/******************************以下是SPI相关的全局函数*************************************/


/********************************************************
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  *********************************************************
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  // 等待超时后的处理,输出错误信息 
  PCAP02_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
*/
/**************************************************************************/



/****************************************************************************/


 /***************************************************************************
  * @brief  单片机时钟初始化
  * @param  无
  * @retval 无
  ***************************************************************************/
/*
void RCC_Init (void)
{
//	uint8_t HSEStartUpStatus = 0;
		//	配置系统时钟 
	RCC_DeInit ();
	//清空标志位
	RCC_ClearFlag  ();
	RCC_ClearITPendingBit  (RCC_IT_LSERDY | RCC_IT_HSERDY | RCC_IT_PLLRDY);
	RCC_ClockSecuritySystemCmd  (ENABLE); 
	
	//开启外部时钟源
	RCC_HSEConfig (RCC_HSE_ON);
//	HSEStartUpStatus = RCC_WaitForHSEStartUp ();
//	SPITimeout = SPIT_FLAG_TIMEOUT;
//  while (HSEStartUpStatus == RESET)
//   {
//    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
//   }
//	printf("HSE configur %s\n!",RCC_WaitForHSEStartUp ());
	//设置AHB时钟
	
	RCC_LSEConfig (RCC_LSE_ON);
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  while (RCC_GetFlagStatus  (RCC_FLAG_LSERDY) == RESET)
   {
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 
	 //使能锁相环
	 RCC_PLLCmd  (ENABLE);
	 	SPITimeout = SPIT_FLAG_TIMEOUT;
  while (RCC_GetFlagStatus  (RCC_FLAG_PLLRDY) == RESET)
   {
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 
	RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK); 
	RCC_HCLKConfig (RCC_SYSCLK_Div1);
	while(RCC_GetSYSCLKSource() != 0x08)
	 {
			SPITimeout = SPIT_FLAG_TIMEOUT;
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 RCC_PCLK2Config (RCC_HCLK_Div2);

}
*/
/****************************************************************************/

/***************************************************************************
  * @brief  GPIO初始化
  * @param  无
  * @retval 无
  ***************************************************************************/
void GPIOs_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			 
	// GPIO使能 
	RCC_AHB1PeriphResetCmd (PCAP02_SPI_SCK_GPIO_CLK | PCAP02_SPI_MISO_GPIO_CLK|
	                        PCAP02_SPI_MOSI_GPIO_CLK| PCAP02_CS_GPIO_CLK | 
													PCAP02_PG3_GPIO_CLK | RCC_AHB1Periph_GPIOD,DISABLE);
  RCC_AHB1PeriphClockCmd (PCAP02_SPI_SCK_GPIO_CLK | PCAP02_SPI_MISO_GPIO_CLK|
	                        PCAP02_SPI_MOSI_GPIO_CLK| PCAP02_CS_GPIO_CLK | 
													PCAP02_PG3_GPIO_CLK | RCC_AHB1Periph_GPIOD, ENABLE);

  //!< SPI_PCAP02_SPI 时钟使能 
	RCC_APB2PeriphResetCmd (RCC_APB2Periph_SPI1,DISABLE);
  PCAP02_SPI_CLK_INIT(PCAP02_SPI_CLK, ENABLE);
 
  //设置引脚复用
  GPIO_PinAFConfig(PCAP02_SPI_SCK_GPIO_PORT,PCAP02_SPI_SCK_PINSOURCE,PCAP02_SPI_SCK_AF); 
	GPIO_PinAFConfig(PCAP02_SPI_MISO_GPIO_PORT,PCAP02_SPI_MISO_PINSOURCE,PCAP02_SPI_MISO_AF); 
	GPIO_PinAFConfig(PCAP02_SPI_MOSI_GPIO_PORT,PCAP02_SPI_MOSI_PINSOURCE,PCAP02_SPI_MOSI_AF); 
//		GPIO_PinAFConfig(PCAP02_CS_GPIO_PORT,PCAP02_SPI_CS_PINSOURCE,PCAP02_SPI_CS_AF); 		//不用硬件启动记得注释掉
  
  //!< 配置 SPI_PCAP02_SPI 引脚: SCK 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(PCAP02_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	//!< 配置 SPI_PCAP02_SPI 引脚: MISO 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PCAP02_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	//!< 配置 SPI_PCAP02_SPI 引脚: MOSI 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PCAP02_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	//!< 配置 SPI_PCAP02_SPI 引脚: CS 
  GPIO_InitStructure.GPIO_Pin = PCAP02_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//悬空
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP；					//上拉
  GPIO_Init(PCAP02_CS_GPIO_PORT, &GPIO_InitStructure);
	
/************************************************************************/
// PG3硬件触发转换
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //上升沿触发，下拉
  GPIO_InitStructure.GPIO_Pin = PCAP02_PG3_PIN;
  GPIO_Init(PCAP02_PG3_GPIO_PORT, &GPIO_InitStructure);
	
//来自PCAP02的外部中断INTN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//悬空输入
  GPIO_InitStructure.GPIO_Pin = PCAP02_INTN_PIN;
  GPIO_Init(PCAP02_INTN_GPIO_PORT, &GPIO_InitStructure);

// SPI 使能IIC_EN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = PCAP02_IIC_EN_PIN;
  GPIO_Init(PCAP02_IIC_EN_GPIO_PORT, &GPIO_InitStructure);
  // 置低使能SPI
  GPIO_WriteBit(PCAP02_IIC_EN_GPIO_PORT, PCAP02_IIC_EN_PIN, Bit_RESET); // SPI1 - PD3
	
// 停止信号 PCAP02: CS引脚高电平
  SPI_PCAP02_CS_HIGH();
}
/****************************************************************************/

 /***************************************************************************
  * @brief  SPI_PCAP02初始化
  * @param  无
  * @retval 无
  ***************************************************************************/
void SPI_PCAP02_Init(void)
{
  
/********************************************************************/
	SPI_InitTypeDef  SPI_InitStructure;
  // PCAP02_SPI 模式配置 
  // PCAP02芯片的SPI模式配置：CPOL=0 CPHA=1 ， Mode1 , D0RD=0,MSB first
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//数据长度8位，PCAP02结果寄存器有24位，407按照每8位读取
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; //设置时钟速度
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(PCAP02_SPI, &SPI_InitStructure);

	
  // 使能 PCAP02_SPI  
  SPI_Cmd(PCAP02_SPI, ENABLE);
}
/**********************************************************************/



/**************************************************************
  * @brief  使用SPI读取一个字节的数据
  * @param  none
  * @retval 返回接收到的数据
  *************************************************************/
uint8_t SPI_PCAP02_ReadData(void)
{
	/* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE) == 0)	{}
		/* 发送无意义字节产生时钟脉冲 */
  SPI_I2S_SendData(PCAP02_SPI, 0x00);
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
  /* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(PCAP02_SPI);
}


/**************************************************************
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的8位一字节数据
  * @retval none
  *************************************************************/
void SPI_PCAP02_SendByte(uint8_t byte)
{
	/* CS 置低开始通信 */
  SPI_PCAP02_CS_LOW();
	
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE) == 0)	{}
  SPI_I2S_SendData(PCAP02_SPI, byte);

  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);

	/* 停止通信 CS 置高 */
		while(SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();
}



/**************************************************************
  * @brief  使用SPI读取PCAP02的状态标志寄存器，返回CDC当前的状态，调试用
  * @param  none
  * @retval 返回状态标志位
  *************************************************************/
uint32_t SPI_PCAP02_Status(void)
{
	uint32_t  STATUS;	
	uint8_t   Status_content[3];
//	uint8_t		RunBit;
//	读取状态寄存器
	Write_Incremental(PCAP02_SPI, Read_Results, 0x24, Status_content, 3);
	STATUS = Status_content[2];
	STATUS = STATUS<<8 | Status_content[1];
	STATUS = STATUS<<8 | Status_content[0];
	//	显示RunBit的状态
//	RunBit = Status_content[0];
//	 printf("STATUS is 0x%08X\r\n RunBit is 0x%02X\n", STATUS, RunBit);	
	return STATUS;
}



/**************************************************************
  * @brief  封装一个使用SPI发送24位操作数据的函数
  * @param  bus_type：PCAP02_SPI，已经在宏里面定义
						command：指令操作码
						address：相应的寄存器地址
						data：要写入的数据
  * @retval 返回接收到的数据
  *************************************************************/
uint8_t Send_24Bit_Opcode( void* bus_type, uint8_t command, uint16_t address, uint8_t data)
{ 
	uint8_t ReadData = 0;
  uint8_t Byte_0  = data;    
  uint8_t Byte_1  = address; 	//地址低8位
//	uint8_t opcode = command & 0xF0;	//指令操作码
  uint8_t Byte_2  = command | (address>>8);	//指令操作码+高4位地址
	
  // SPIx 数据传输 ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS置低，开始通信
  //发送操作码指令和寄存器地址，再执行后续的写入操作	 
  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0)	{}    
		SPI_I2S_SendData(bus_type, Byte_2);   // 发送 byte 2操作码
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);

  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
		SPI_I2S_SendData(bus_type, Byte_1);     // 发送 byte 1寄存器地址
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);

	//--------------------------------------------------------------------------

	if ((command==0x90) || (command==0xA0)) // 如果是写指令
		{
			while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}	 
			SPI_I2S_SendData(bus_type, Byte_0);     // 发送 byte 0
			while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		  SPI_I2S_ReceiveData(bus_type);
		}
		
	if ((command==0x10) || (command==0x20)) // 如果是读SRAM/OTP指令
		{
			ReadData = SPI_PCAP02_ReadData();		//SRAM 前面有1个空字节
//			ReadData = SPI_PCAP02_ReadData();
		}
	// CS置高，结束通信，若SPI正忙，等待，避免提前拉高
	while(SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	
		return ReadData;
}


/************************************************************
	* @brief  使用SPI按字节逐步发送任意长度的数据
  * @param  bus_type：PCAP02_SPI，已经在宏里面定义
						command：指令操作码
						address：相应的寄存器地址
						data：要写入的数据
						quantity：要写入的数据的大小
  * @retval 
  *************************************************************/
void Write_Incremental(void* bus_type, uint8_t command, uint16_t address, uint8_t *data, uint8_t quantity)
{
  int8_t    i = 0;
//  uint8_t    Result_read = 0;

  // SPIx 数据传输 ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS置低，开始通信
		 
	while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0)	{}    
	SPI_I2S_SendData(bus_type, command);   // 发送 byte 2操作码
		while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
			 
  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(bus_type, address);     // 发送 byte 1寄存器地址
		while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
	
	if ((command==0x90) || (command==0xA0) || (command==0xC0)) // 如果是写指令
	{
	// 向BYTE0的位置一字节一字节发送data
		do{
				while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
				SPI_I2S_SendData(bus_type, data[i]); 
					while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
				i++;
			}
		while(i<quantity);
	}
//----------------------------------------------------------------------------

	if ((command==0x10) || (command==0x20)) // 如果是读SRAM/OTP指令
	{
			SPI_PCAP02_ReadData(); //  读SRAM/OTP内容（测试用8位数据）
			data[0] = SPI_PCAP02_ReadData();
	}

//--------------------------------------------------------------------------
	if (command==Read_Results) // 如果是读取结果的指令
	{
		do{
					data[i] = SPI_PCAP02_ReadData();
				i++;
      }
		while(i<quantity);
//			while(i<12);
	}
	while(SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	// CS置高，结束通信
}



/************************************************************
	* @brief  使用SPI写入RunBit位
  * @param  
  * @retval none
  *************************************************************/
void Write_RunBit(uint8_t data)
{
  // SPIx 数据传输 ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS置低，开始通信
		 
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0)	{}    
	SPI_I2S_SendData(PCAP02_SPI, Write_Config);   // 发送 byte 2操作码
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
			 
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(PCAP02_SPI, 77);     // 发送 byte 1寄存器地址
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
//----------------------------------------------------------------------------

	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(PCAP02_SPI, data); 
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
		
//		printf("%d\n", SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_OVR));

	while(SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	// CS置高，结束通信
}





/********************************************************
	* @brief INTN中断，每次转换，INTN引脚产生一次下降沿中断，
					 用于判断是否转换
  * @param  
  * @retval 
**********************************************************/
void Wait_For_Interrupt(void)
{
	while (GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_11)==1)
	{	}
}
/**************************************************************/


/********************************************************
	* @brief 二进制结果转换为十进制
  * @param  
  * @retval 
**********************************************************/
int convertBinaryToDecimal(long long n)
{
  int decimalNumber = 0,i=0,remainder;
  while(n!=0){
    remainder = n%10;   //取余数，
    n /=10;    //取剔除最后一位的数。
    decimalNumber += remainder*pow(2,i); //1111,result = 1+1*2+1*2*2+1*2*2*2
    ++i;
  }
  return decimalNumber;
}
/**************************************************************/


/********************************************************
	* @brief  直接写入和读出测试
  * @param  
  * @retval 
**********************************************************/
void Direct_R_and_W (void)
{
			uint8_t RE=0;
			uint16_t adr = 0x3FF;
			uint8_t dat = 0xCA;
			puts("Test the SRAM Communication");
      printf(" Writing 0x%02X at Address 0x%04X\n",dat,adr);
      												
			SPI_PCAP02_CS_LOW();
				
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, 0x93);  
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}				 // 写入EEPROM

//    	  SPI_PCAP02_CS_HIGH();
//				 
//				SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
			  SPI_I2S_SendData(PCAP02_SPI, 0xFF); 
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 // 发送地址

//			 SPI_PCAP02_CS_HIGH();
//			 
//			 SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, dat); 
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}				// 发送数据

     
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 

			  SPI_PCAP02_CS_HIGH();
//			
//			
			  SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, 0x13);     // 读取EEPROM


//   		  SPI_PCAP02_CS_HIGH();
//				
//				SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
		 	 SPI_I2S_SendData(PCAP02_SPI, 0xFF);     // 发送地址


			SPI_PCAP02_CS_HIGH();
		
			SPI_PCAP02_CS_LOW();
				while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==RESET) {};

  
     //Compulsory reads to DR and SR to clear OVR,
     //so that next incoming data is saved
     SPI_I2S_ReceiveData(PCAP02_SPI);                     // To clear OVR
     SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE); // To clear OVR

     //Reading byte1
     SPI_I2S_SendData(PCAP02_SPI, 0xFF);  // DUMMY WRITE
					while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE)==0) {}

     // Wait until RX buffer is not empty, then read the received data
//					SPI_PCAP02_CS_HIGH();
//					
//					SPI_PCAP02_CS_LOW();
     while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE)==0) {}

     RE = SPI_I2S_ReceiveData(PCAP02_SPI); //  Read

			 	SPI_PCAP02_CS_HIGH();

      printf(" Reading at Address 0x%04X, Data is 0x%02X\n\n", adr, RE );
		
}
/*****************************************************************************/


/********************************************************
	* @brief  打印传参测试
  * @param  
  * @retval 
**********************************************************/
uint8_t TraPar ( uint8_t PutIn)
{
	uint8_t test = 0;
	test = 0xEB & PutIn;
	return test;
}
/**************************************************************/


