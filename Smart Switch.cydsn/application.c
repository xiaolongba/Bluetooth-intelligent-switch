/* ========================================
 * 文件：application.c
 * 作者：Helon
 * 日期：2016-04-23
 * 版本：Ver1.0
 * 描述：这个文件包括了所有在应用层上使用的函数定义
 * ========================================
*/

/* ****************************************
 * 头文件包含
 * ****************************************
*/
#include <project.h> 
#include "application.h"

/* ****************************************
 * 变量声明定义
 * ****************************************
*/

/******************************************
  * @函数名：StackEventHandler
  * @输入：  eventCode---表示不同的事件
             eventParam---表示不同的事件所携带的不同参数
  * @返回：  NULL            
  * @描述：  处理BLE协议栈产生的不同事件
  *****************************************
*/
void StackEventHandler(uint32 eventCode, void *eventParam)
{
    eventParam = eventParam;//防止编译时出现警告
    switch(eventCode)
    {
        case CYBLE_EVT_STACK_ON:
             CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        break;
        default:
        break;
    }
}

/******************************************
  * @函数名：LowPowerManagement
  * @输入：  NULL             
  * @返回：  NULL            
  * @描述：  低功耗控制，降低功耗
  *****************************************/
void LowPowerManagement(void)
{
    /* Local variable to store the status of BLESS Hardware block */
		CYBLE_LP_MODE_T sleepMode;
		CYBLE_BLESS_STATE_T blessState;
	
		/* Put BLESS into Deep Sleep and check the return status */
		sleepMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
		
		/* Disable global interrupt to prevent changes from any other interrupt ISR */
		CyGlobalIntDisable;
	
		/* Check the Status of BLESS */
		blessState = CyBle_GetBleSsState();

		if(sleepMode == CYBLE_BLESS_DEEPSLEEP)
		{
		    /* If the ECO has started or the BLESS can go to Deep Sleep, then place CPU 
			* to Deep Sleep */
			if(blessState == CYBLE_BLESS_STATE_ECO_ON || blessState == CYBLE_BLESS_STATE_DEEPSLEEP)
		    {

                CySysPmDeepSleep();
		 	}
		}
		else if((sleepMode == CYBLE_BLESS_SLEEP)||(sleepMode == CYBLE_BLESS_ACTIVE))
		{
		    if(blessState != CYBLE_BLESS_STATE_EVENT_CLOSE)
		    {
				/* If the BLESS hardware block cannot go to Deep Sleep and BLE Event has not 
				* closed yet, then place CPU to Sleep */ 
		        CySysPmSleep();  
		    }
		}
		/* Re-enable global interrupt mask after wakeup */
		CyGlobalIntEnable;           
        
}
/* [] END OF FILE */
