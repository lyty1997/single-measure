/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include <stdio.h>
#include <stdlib.h>
#include "bsp_debug_usart.h"
#include "bsp_led.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
// #include "bsp_led.h"

 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串DEBUG_USART 模式 */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
  USART_Cmd(DEBUG_USART, ENABLE);
}

//printf重定向到串口函数
/********************************************************************************************************/
// int _write (int file, char *pBuffer, int size)  

// {  

//     for (int i = 0; i < size; i++)  

//     {  

//         while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);//等待上一次串口数据发送完成  

//         USART1->DR = (uint8_t) *(pBuffer+i);       //写DR,串口1将发送数据
        // USART_SendData(DEBUG_USART, (uint8_t) fd);

//     }  

//     return size;  

// }
/********************************************************************************************************/
//重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
// int fputc(int ch, FILE *f)
// {
// 		/* 发送一个字节数据到串口DEBUG_USART */
// 		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
// 		/* 等待发送完毕 */
// 		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
// 	// LED_RED;
// 		return (ch);
// }

//重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
// int fgetc(FILE *f)
// {
// 		/* 等待串口输入数据 */
// 		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

// 		return (int)USART_ReceiveData(DEBUG_USART);
// }

/*
 * To implement the STDIO functions you need to create
 * the _read and _write functions and hook them to the
 * USART you are using. This example also has a buffered
 * read function for basic line editing.
 */
// int _write(int fd, char *ptr, int len);
// int _read(int fd, char *ptr, int len);
// void get_buffered_line(void);

// /*
//  * This is a pretty classic ring buffer for characters
//  */
// #define BUFLEN 127

// static uint16_t start_ndx;
// static uint16_t end_ndx;
// static char buf[BUFLEN + 1];
// #define buf_len ((end_ndx - start_ndx) % BUFLEN)
// static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
// static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

// /* back up the cursor one space */
// static inline void back_up(void)
// {
//     end_ndx = dec_ndx(end_ndx);
//     USART_SendData(USART1, '\010');
//     USART_SendData(USART1, ' ');
//     USART_SendData(USART1, '\010');
// }

// /*
//  * A buffered line editing function.
//  */
// void get_buffered_line(void)
// {
//     char c;

//     if (start_ndx != end_ndx)
//     {
//         return;
//     }

//     while (1)
//     {
//         c = USART_ReceiveData(USART1);

//         if (c == '\r')
//         {
//             buf[end_ndx] = '\n';
//             end_ndx = inc_ndx(end_ndx);
//             buf[end_ndx] = '\0';
//             USART_SendData(USART1, '\r');
//             USART_SendData(USART1, '\n');
//             return;
//         }

//         /* or DEL erase a character */
//         if ((c == '\010') || (c == '\177'))
//         {
//             if (buf_len == 0)
//             {
//                 USART_SendData(USART1, '\a');
//             }

//             else
//             {
//                 back_up();
//             }

//             /* erases a word */
//         }

//         else if (c == 0x17)
//         {
//             while ((buf_len > 0) &&
//                     (!(isspace((int) buf[end_ndx]))))
//             {
//                 back_up();
//             }

//             /* erases the line */
//         }

//         else if (c == 0x15)
//         {
//             while (buf_len > 0)
//             {
//                 back_up();
//             }

//             /* Non-editing character so insert it */
//         }

//         else
//         {
//             if (buf_len == (BUFLEN - 1))
//             {
//                 USART_SendData(USART1, '\a');
//             }

//             else
//             {
//                 buf[end_ndx] = c;
//                 end_ndx = inc_ndx(end_ndx);
//                 USART_SendData(USART1, c);
//             }
//         }
//     }
// }

/********************************************************************************************************/
/*
 * Called by libc stdio fwrite functions
 */
// int _write(int fd, char *ptr, int len)
// {
//     int i = 0;

//     /*
//      * write "len" of char from "ptr" to file id "fd"
//      * Return number of char written.
//      *
//     * Only work for STDOUT, STDIN, and STDERR
//      */
//     if (fd > 2)
//     {
//         return -1;
//     }

//     while (*ptr && (i < len))
//     {
//         USART_SendData(USART1, *ptr);

//         if (*ptr == '\n')
//         {
//             USART_SendData(USART1, '\r');
//         }

//         i++;
//         ptr++;
//     }
//     // LED_YELLOW;

//     return i;
// }
/********************************************************************************************************/

/*
 * Called by the libc stdio fread fucntions
 *
 * Implements a buffered read with line editing.
 */
// int _read(int fd, char *ptr, int len)
// {
//     int my_len;

//     if (fd > 2)
//     {
//         return -1;
//     }

//     get_buffered_line();
//     my_len = 0;

//     while ((buf_len > 0) && (len > 0))
//     {
//         *ptr++ = buf[start_ndx];
//         start_ndx = inc_ndx(start_ndx);
//         my_len++;
//         len--;
//     }

//     return my_len; /* return the length we got */
// }

/*********************************************END OF FILE**********************/
