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
void ServiceToClient(uint8_t* TxData,uint16_t Len);
/* ****************************************
 * 宏定义
 * ****************************************
*/
#define  LOWPOWER   (1)   
#define  TURE       (1)
#define  FALSE      (0)

#endif
/* [] END OF FILE */
