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
extern uint8_t THROUGH_STAUS;
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

    CyGlobalIntEnable;  /* Uncomment this line to enable global interrupts. */    
    CySysClkIloStop();
    CySysClkWriteEcoDiv(CY_SYS_CLK_ECO_DIV8);
    CyBle_Start(StackEventHandler);//BLE协议栈初始化
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
//        if(THROUGH_STAUS)
//        {
//           THROUGH_STAUS=FALSE;
//           ServiceToClient((uint8_t*)"Helon Test",sizeof("Helon Test")); 
//        }
        
        #if LOWPOWER
            CyBle_ProcessEvents();
            LowPowerManagement();
        #endif
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
