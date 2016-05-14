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
#include <malloc.h>
/* ****************************************
 * 变量声明定义
 * ****************************************
*/
CYBLE_GAP_BONDED_DEV_ADDR_LIST_T BondList;//保存绑定的信息
uint8_t THROUGH_STAUS=FALSE;
char* CommandList[]={"AT+SWT"};//控制命令列表
uint8_t CommandListLen=sizeof(CommandList)/sizeof(CommandList[0]);//控制命令列表的命令个数
uint8_t Buffer[BUFFERLEN]={0};//临时存放主机透传给从机的数据
uint8_t RX_ISOVER=FALSE;
/* ****************************************
 * 各功能函数地具体实现
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
    CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T WriteCmd;//临时保存一些主机的写命令    
    CYBLE_GATTS_WRITE_REQ_PARAM_T WriteValue;//临时保存一些主机的写
    uint8_t i;
    switch(eventCode)
    {
        case CYBLE_EVT_STACK_ON://蓝牙初始化完成            
             CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);//初始化时立马发起广播
        break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED://蓝牙连接成功
//            CyBle_GapAuthReq(cyBle_connHandle.bdHandle,&cyBle_authInfo);//从机一旦建立连接马上发起配对请求
        break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED://蓝牙断开成功
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);//断开连接之后也立马发起广播
            THROUGH_STAUS=FALSE;
        break;
        case CYBLE_EVT_GATTS_WRITE_CMD_REQ://处理主机发起的写命令请求
            WriteCmd = *((CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *)eventParam);
            if(WriteCmd.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)//使能透传通道
            {
                if(WriteCmd.handleValPair.value.val[0] == 0x01)
                {
                    THROUGH_STAUS=TURE;//使能透传通道成功,从机可以给主机透传数据
                    if(0 == BLUE_LED_Read())
                    {
                        ServiceToClient("AT+SWT=1",sizeof("AT+SWT=1"));
                    }
                    else
                    {
                        ServiceToClient("AT+SWT=0",sizeof("AT+SWT=0"));
                    }
                    
                }
                else
                {
                    THROUGH_STAUS=FALSE;//关闭透传通道
                }
            }
            else if(WriteCmd.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_RX_CHAR_HANDLE)//接收主机给从机透传数据
            {
                for(i=0;i<WriteCmd.handleValPair.value.len;i++)
                {
                    Buffer[i] = WriteCmd.handleValPair.value.val[i];//存放主机透传给从机的数据
                }
                RX_ISOVER=TURE;
            }
        break;
        case CYBLE_EVT_GATTS_WRITE_REQ:
            WriteValue = *((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam);
            WriteValue = WriteValue;
//            if(WriteValue.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)//使能透传通道
//            {
//                if(WriteValue.handleValPair.value.val[0] == 0x01)
//                {
//                    THROUGH_STAUS=TURE;//使能透传通道成功,从机可以给主机透传数据  
////                    ServiceToClient((uint8_t*)"Helon Test",sizeof("Helon Test"));
//                }
//                else
//                {
//                    THROUGH_STAUS=FALSE;//关闭透传通道
//                }
//            }
            CyBle_GattsWriteRsp(cyBle_connHandle);
        break;            
        default:
        break;
    }
}

/******************************************
  * @函数名：General_discovery_mode_Init
  * @输入：  NULL             
  * @返回：  NULL            
  * @描述：  通用发现的广播模式初始化
  *****************************************
*/
void General_discovery_mode_Init(void)
{
    
}

/******************************************
  * @函数名：Limited_discovery_mode_Init
  * @输入：  NULL             
  * @返回：  NULL            
  * @描述：  限制发现的广播模式初始化
  *****************************************
*/
void Limited_discovery_mode_Init(void)
{
    
}

/******************************************
  * @函数名：Discovery_mode_Init
  * @输入：  NULL             
  * @返回：  NULL            
  * @描述：  发现模块的初始化
  *****************************************
*/
void Discovery_mode_Init(void)
{
//    cyBle_discoveryModeInfo.advTo=0x00;//广播没有超时
//    CyBle_GapGetBondedDevicesList(&BondList);//获取绑定设备的信息
    
}

/******************************************
  * @函数名：ServiceToClient
  * @输入：  TxData---指向需要透传的数据的首位地址 
             Len---所发送的数据长度
  * @返回：  NULL            
  * @描述：  从机给主机透传数据
  *****************************************
*/
void ServiceToClient(char* TxData,uint16_t Len)
{   
    CYBLE_API_RESULT_T API_RESULT;
    if(THROUGH_STAUS)//如果主机打开从机的透传通道,从机则马上给主机发送透传数据
    {
        CYBLE_GATTS_HANDLE_VALUE_NTF_T ServiceToClientData;//临时存放要发送给主机的信息
        ServiceToClientData.value.val=(uint8_t*)TxData;//发送数据的首地址
        ServiceToClientData.value.len=Len-1;//所要发送的数据的长度
        ServiceToClientData.attrHandle=CYBLE_TROUGHPUT_SERVICE_TX_CHAR_HANDLE;//所要发送数据的特征值句柄
        while(CyBle_GattGetBusStatus()!=CYBLE_STACK_STATE_FREE);//等待蓝牙空闲时才发送数据        
        do
        {
            API_RESULT=CyBle_GattsNotification(cyBle_connHandle,&ServiceToClientData);//发送数据给主机
            CyBle_ProcessEvents();
        }
        while((API_RESULT != CYBLE_ERROR_OK)&&(CYBLE_STATE_CONNECTED == cyBle_state));        
//        API_RESULT=API_RESULT+0;
    }
}

/******************************************
  * @函数名：ClientData_Handler
  * @输入：  RxData---指向接收到主机透传数据的首地址              
  * @返回：  NULL            
  * @描述：  处理主机透传给从机的数据
  *****************************************
*/
void ClientData_Handler(const char* RxData)
{
    uint8_t Location,Index;       
    //获取AT命令等号的位置，如“AT+RESET=1”中“=”号的位置是8,从0开始算起.
    Location=strchr((char*)RxData,'=')-RxData;
    //动态分配堆空间
    char* String=malloc(sizeof(char)*Location);
    //保存AT命令等号前的数据.
    strncpy(String,RxData,Location); 
    //匹配AT命令列表中AT命令的位置
    for(Index=0;Index<CommandListLen;Index++)
    {
        if(strncmp((char*)String,CommandList[Index],Location)==0)
        {
            break;
        }
    }    
    free(String);//释放之前分配的堆空间      
    switch(Index)//执行相应的命令
    {
        case SWT:
            if(RxData[Location+1]-0x30)//控制开关为开
            {
                BLUE_LED_Write(ON);
                ServiceToClient("AT+SWT=1",sizeof("AT+SWT=1"));
            }
            else if(0 == RxData[Location+1]-0x30)//控制开关为关
            {
                BLUE_LED_Write(OFF);
                ServiceToClient("AT+SWT=0",sizeof("AT+SWT=0"));
            }
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
