#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "bsp_led.h"
/*
 * To implement the STDIO functions you need to create
 * the _read and _write functions and hook them to the
 * USART you are using. This example also has a buffered
 * read function for basic line editing.
 */
int _write(int fd, char *ptr, int len);
// int _read(int fd, char *ptr, int len);
// void get_buffered_line(void);

/*
 * This is a pretty classic ring buffer for characters
 */
// #define BUFLEN 127

// static uint16_t start_ndx;
// static uint16_t end_ndx;
// static char buf[BUFLEN + 1];
// #define buf_len ((end_ndx - start_ndx) % BUFLEN)
// static inline int inc_ndx(int n) { return ((n + 1) % BUFLEN); }
// static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

/* back up the cursor one space */
// static inline void back_up(void)
// {
//     end_ndx = dec_ndx(end_ndx);
//     USART_SendData(USART1, '\010');
//     USART_SendData(USART1, ' ');
//     USART_SendData(USART1, '\010');
// }

/*
 * A buffered line editing function.
 */

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

        /* or DEL erase a character */
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

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    // if (fd > 2)
    // {
    //     return -1;
    // }
    // LED_RED;
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    // while (*ptr && (i < len))
    // {
    //     USART_SendData(USART1, *ptr);

    //     if (*ptr == '\n')
    //     {
    //         USART_SendData(USART1, '\r');
    //     }

    //     i++;
    //     ptr++;
    // }
    // LED_YELLOW;

//     return i;
// }
/********************************************************************************************************/
//printf重定向到串口函数
/********************************************************************************************************/
int _write (int fd, char *pBuffer, int size){  
    // if (fd > 2)
    // {
    //     return -1;
    // }
    
    for (int i = 0; i < size; i++){  
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待上一次串口数据发送完成  

        USART1->DR = (uint8_t) *(pBuffer+i);       //写DR,串口1将发送数据
    }  

    return size;  
}
/********************************************************************************************************/
/********************************************************************************************************/
// int _write_r (struct _reent *r, int file, char * ptr, int len)
// {  
//   r = r;
//   file = file;
//   ptr = ptr;
 
// #if 1
//   int index;
//   /* For example, output string by UART or ITM*/
//   for(index=0; index<len; index++)
//   {
//     if (ptr[index] == '\n')
//     {
//       USART_SendData(USART1,'\r');
//     }  
 
//     USART_SendData(USART1,ptr[index]);
//   }  
// #endif   
  
//   return len;
// }


/*********************************************************************************************/
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
