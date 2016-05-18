/* ========================================
 * 文件：main.c
 * 作者：Helon
 * 日期：2016-04-23
 * 版本：Ver1.0
 * 描述：这个文件是整个工程的入口,初始化各
         个外设,处理BLE协议栈的消息队列
 * ========================================
*/

/* ****************************************
 * 头文件包含
 * ****************************************
*/
#include <project.h>
#include "application.h"
extern uint8_t Buffer[BUFFERLEN];
extern uint8_t LOWPOWER;
extern uint8_t RX_ISOVER;
extern uint8_t WakeUp;
extern uint8_t ALL_ON_OFF;
/******************************************
  * @函数名：main
  * @输入：  NULL             
  * @返回：  无返回
  * @描述：  初始化各个外设,处理BLE协议栈的
             消息队列
  *****************************************
*/
int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    SystemInit();
    CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */  
    for(;;)
    {
        /* 保存绑定信息 */
        if((cyBle_pendingFlashWrite !=0))
        {
            /*  */
            if(CYBLE_ERROR_OK == CyBle_StoreBondingData(0))
            {
                //不做任何事情
            }
        }
        if(WakeUp)
        {
            WakeUp=FALSE;
//            CyBle_GapcStartScan(CYBLE_SCANNING_FAST); 
        }
        ButtonHandler();
        if(RX_ISOVER)
        {
            RX_ISOVER = FALSE;
            ServerData_Handler((char*)Buffer);            
                
        }
        if(ALL_ON_OFF)
        {
//            ALL_ON_OFF = FALSE;
            ALL_On_Off_Handler();
        }
        if(LOWPOWER)
        {
            CyBle_ProcessEvents();
            LowPowerManagement();
        }
        
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
