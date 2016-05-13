/* ========================================
 * 文件：application.h
 * 作者：Helon
 * 日期：2016-04-23
 * 版本：Ver1.0
 * 描述：这个文件包括了所有在应用层上使用的函数声明
 * ========================================
*/

#ifndef APPLICATION_H
#define APPLICATION_H
    
/* ****************************************
 * 头文件包含
 * ****************************************
*/
#include <project.h>  
    
/* ****************************************
 * 函数原型声明
 * ****************************************
*/      
void StackEventHandler(uint32 eventCode, void *eventParam);
void LowPowerManagement(void);
void ServiceToClient(char* TxData,uint16_t Len);
void ClientData_Handler(const char* RxData);
CY_ISR_PROTO(BT1_IntHandler);
CY_ISR_PROTO(BT2_IntHandler);
CY_ISR_PROTO(BT3_IntHandler);
CY_ISR_PROTO(BT4_IntHandler);
/* ****************************************
 * 宏定义
 * ****************************************
*/
   
#define  TURE       (1)
#define  FALSE      (0)
#define  ON         (0)
#define  OFF        (1)
#define  BUFFERLEN  (23)
/* ****************************************
 * 控制命令枚举
 * ****************************************
*/
enum
{
    SWT=0x00u
};

enum
{
    BUTTON1_DOWN=0x00u,
    BUTTON2_DOWN,
    BUTTON3_DOWN,
    BUTTON4_DOWN,
    BUTTON_UP
};
#endif
/* [] END OF FILE */
