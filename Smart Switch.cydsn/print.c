/* ========================================
 * 文件：print.c
 * 作者：Helon
 * 日期：2016-05-14
 * 版本：Ver1.0
 * 描述：这个文件是相关Print函数重映射到串口的函数定义
 * ========================================
*/
/* ****************************************
 * 头文件包含
 * ****************************************
*/
#include "print.h"

/* ****************************************
 * 各功能函数地具体实现
 * ****************************************
*/

/******************************************************************************
* Function Name: Printf函数重映射
***************************************************************************//**
* 
*  重写_write()函数，为了使能Printf函数打印串口信息
* 
* \return
*  None
* 
******************************************************************************/
#if defined(__ARMCC_VERSION)
/* For MDK/RVDS compiler revise fputc function for printf functionality */
struct __FILE
{
    int handle;
};
enum
{
    STDIN_HANDLE,
    STDOUT_HANDLE,
    STDERR_HANDLE
};
FILE __stdin = {STDIN_HANDLE};
FILE __stdout = {STDOUT_HANDLE};
FILE __stderr = {STDERR_HANDLE};
int fputc(int ch, FILE *file)
{
    int ret = EOF;
    switch( file->handle )
    {
        case STDOUT_HANDLE:
            UART_DEB_UartPutChar(ch);
            ret = ch ;
            break ;
        case STDERR_HANDLE:
            ret = ch ;
            break ;
        default:
            file = file;
            break ;
    }
    return ret ;
}
#elif defined (__ICCARM__)      /* IAR */
/* For IAR compiler revise __write() function for printf functionality */
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;
    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return (0);
    }
    for (/* Empty */; size != 0; --size)
    {
        UART_DEB_UartPutChar(*buffer++);
        ++nChars;
    }
    return (nChars);
}
#else  /* (__GNUC__)  GCC */
/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len)
{
    int i;
    file = file;
    for (i = 0; i < len; i++)
    {
        UART_UartPutChar(*ptr++);//这里替代成UART的发送单字节的函数即可
    }
    return len;
}
#endif  /* (__ARMCC_VERSION) */ 
/* [] END OF FILE */
