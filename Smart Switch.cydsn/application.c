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
#include "print.h"
/* ****************************************
 * 变量声明定义
 * ****************************************
*/
CYBLE_GAP_BONDED_DEV_ADDR_LIST_T BondList;//保存绑定的信息
uint8_t NotifyValue[]={0x01,0x00};
uint8_t WakeUp=FALSE;
uint8_t DeviceCounts=0;
uint8_t DeviceNum=INVALID;
uint8_t BondNum=BONDNUM;
//uint8 testAddr[]={0x8F,0xAA,0x1C,0x50,0xA0,0x00};
//uint8_t addrtype=0x00;
CYBLE_GAP_BD_ADDR_T TestDevice=
{
    {0x8F,0xAA,0x1C,0x50,0xA0,0x00},
    0x00

};
CYBLE_GAP_BD_ADDR_T Device1=
{
    {0xCB,0x83,0x6D,0x50,0xA0,0x00},
    0x00

};

CYBLE_GAP_BD_ADDR_T Device2=
{
    {0xA5,0x83,0x6D,0x50,0xA0,0x00},
    0x00

};

CYBLE_GAP_BD_ADDR_T Device3=
{
    {0x2F,0x84,0x6D,0x50,0xA0,0x00},
    0x00

};

CYBLE_GAP_BD_ADDR_T Device4=
{
    {0x25,0x84,0x6D,0x50,0xA0,0x00},
    0x00

};

CYBLE_GAP_BD_ADDR_T Device5=
{
    {0xD1,0x83,0x6D,0x50,0xA0,0x00},
    0x00

};
CYBLE_GATTC_WRITE_CMD_REQ_T WriteCmd=
{
    {
        NotifyValue,
        2,
        0
    },
    CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE
};//初始化打开透传通道的数据
uint8_t THROUGH_STAUS=FALSE;
char* CommandList[]={"AT+SWT=1","AT+SWT=0"};//控制命令列表
//char* ReceviceCmdList[]={"AT+ON","AT+OFF"};
uint8_t CommandListLen=sizeof(CommandList)/sizeof(CommandList[0]);//控制命令列表的命令个数
uint8_t Buffer[BUFFERLEN]={0};//临时存放从机透传给主机的数据
uint8_t RX_ISOVER=FALSE;
uint8_t LOWPOWER;
uint8_t ALL_ON_OFF=FALSE;
uint8_t ButtonStatus=BUTTON_UP;
uint8_t DisonnectedStaus=FALSE;
//static uint8_t Switch_On_Off=SWTICH_OFF;
uint8_t ButtonType;
struct BT Button=
{
    SWTICH_OFF,
    SWTICH_OFF,
    SWTICH_OFF,
    SWTICH_OFF,
    SWTICH_OFF
};//初始化所有按键都是松开的状态
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
//    CYBLE_API_RESULT_T API_RESULT;
    CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T ReceviceNotifyValue;//这个是存放接收从机发送过来的Notify值
    CYBLE_GAPC_ADV_REPORT_T ScanResult;
    uint8_t i;
    switch(eventCode)
    {
        case CYBLE_EVT_STACK_ON://蓝牙初始化完成    
              #ifdef PRINT
//                printf("BLE is Ready!\r\n");
//                while((UART_SpiUartGetTxBufferSize() + UART_GET_TX_FIFO_SR_VALID) != 0);//等待串口缓冲区的数据发送完成                
              #endif
            LOWPOWER=TURE;//上电完成之后立马进入睡眠状态
        break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED://蓝牙连接成功                
              #ifdef PRINT
                printf("BLE is Connectted!\r\n");
//                while((UART_SpiUartGetTxBufferSize() + UART_GET_TX_FIFO_SR_VALID) != 0);//等待串口缓冲区的数据发送完成                
              #endif
            WriteCmd.attrHandle = CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
            WriteCmd.value.val = NotifyValue;
            WriteCmd.value.len = 2;
            CyBle_GattcWriteWithoutResponse(cyBle_connHandle, &WriteCmd);//打开透传通道            
//            CyBle_GapAuthReq(cyBle_connHandle.bdHandle,&cyBle_authInfo);//从机一旦建立连接马上发起配对请求
        break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED://蓝牙断开成功
              #ifdef PRINT
                printf("BLE is DisConnectted!\r\n");
                while((UART_SpiUartGetTxBufferSize() + UART_GET_TX_FIFO_SR_VALID) != 0);//等待串口缓冲区的数据发送完成                
              #endif
            DisonnectedStaus = TURE;
            if(FALSE == ALL_ON_OFF)
                LOWPOWER=TURE;//断开之后，主机马上进入低功耗
//            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);//断开连接之后也立马发起广播
//            THROUGH_STAUS=FALSE;
        break;
        case CYBLE_EVT_GATTS_WRITE_CMD_REQ://处理主机发起的写命令请求
//            WriteCmd = *((CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *)eventParam);
//            if(WriteCmd.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)//使能透传通道
//            {
//                if(WriteCmd.handleValPair.value.val[0] == 0x01)
//                {
//                    THROUGH_STAUS=TURE;//使能透传通道成功,从机可以给主机透传数据  
////                    ServiceToClient((uint8_t*)"Helon Test",sizeof("Helon Test"));
//                }
//                else
//                {
//                    THROUGH_STAUS=FALSE;//关闭透传通道
//                }
//            }
//            else if(WriteCmd.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_RX_CHAR_HANDLE)//接收主机给从机的透传数据
//            {
//                for(i=0;i<WriteCmd.handleValPair.value.len;i++)
//                {
//                    Buffer[i] = WriteCmd.handleValPair.value.val[i];//存放主机透传给从机的数据
//                }
//                RX_ISOVER=TURE;
//            }
        break;
        case CYBLE_EVT_GATTS_WRITE_REQ://处理主机发起的写请求
//            WriteValue = *((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam);
//            WriteValue = WriteValue;
////            if(WriteValue.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_TX_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)//使能透传通道
////            {
////                if(WriteValue.handleValPair.value.val[0] == 0x01)
////                {
////                    THROUGH_STAUS=TURE;//使能透传通道成功,从机可以给主机透传数据  
//////                    ServiceToClient((uint8_t*)"Helon Test",sizeof("Helon Test"));
////                }
////                else
////                {
////                    THROUGH_STAUS=FALSE;//关闭透传通道
////                }
////            }
//            CyBle_GattsWriteRsp(cyBle_connHandle);//写响应
        break; 
        case CYBLE_EVT_GATTC_HANDLE_VALUE_NTF:
            ReceviceNotifyValue = *((CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T *)eventParam);
            if(ReceviceNotifyValue.handleValPair.attrHandle == CYBLE_TROUGHPUT_SERVICE_TX_CHAR_HANDLE)
            {
                for(i=0;i<ReceviceNotifyValue.handleValPair.value.len;i++)
                {
                    Buffer[i] = ReceviceNotifyValue.handleValPair.value.val[i];
                } 
                RX_ISOVER=TURE;
                #ifdef PRINT
                    printf("Recevice From Server!\r\n");
    //                while((UART_SpiUartGetTxBufferSize() + UART_GET_TX_FIFO_SR_VALID) != 0);//等待串口缓冲区的数据发送完成                
                #endif    
                
            }
                            
        break;
        case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
            ScanResult = *((CYBLE_GAPC_ADV_REPORT_T *)eventParam);
            
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
    TxData = TxData;
    Len = Len;
//    CYBLE_API_RESULT_T API_RESULT;
//    if(THROUGH_STAUS)//如果主机打开从机的透传通道,从机则马上给主机发送透传数据
//    {
//        CYBLE_GATTS_HANDLE_VALUE_NTF_T ServiceToClientData;//临时存放要发送给主机的信息
//        ServiceToClientData.value.val=(uint8_t*)TxData;//发送数据的首地址
//        ServiceToClientData.value.len=Len-1;//所要发送的数据的长度
//        ServiceToClientData.attrHandle=CYBLE_TROUGHPUT_SERVICE_TX_CHAR_HANDLE;//所要发送数据的特征值句柄
//        while(CyBle_GattGetBusStatus()!=CYBLE_STACK_STATE_FREE);//等待蓝牙空闲时才发送数据        
//        do
//        {
//            API_RESULT=CyBle_GattsNotification(cyBle_connHandle,&ServiceToClientData);//发送数据给主机
//            CyBle_ProcessEvents();
//        }
//        while((API_RESULT != CYBLE_ERROR_OK)&&(CYBLE_STATE_CONNECTED == cyBle_state));        
////        API_RESULT=API_RESULT+0;
//    }
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
    RxData = RxData;
//    uint8_t Location,Index;       
//    //获取AT命令等号的位置，如“AT+RESET=1”中“=”号的位置是8,从0开始算起.
//    Location=strchr((char*)RxData,'=')-RxData;
//    //动态分配堆空间
//    char* String=malloc(sizeof(char)*Location);
//    //保存AT命令等号前的数据.
//    strncpy(String,RxData,Location); 
//    //匹配AT命令列表中AT命令的位置
//    for(Index=0;Index<CommandListLen;Index++)
//    {
//        if(strncmp((char*)String,CommandList[Index],Location)==0)
//        {
//            break;
//        }
//    }    
//    free(String);//释放之前分配的堆空间      
//    switch(Index)//执行相应的命令
//    {
//        case SWT:
//            if(RxData[Location+1]-0x30)//控制开关为开
//            {
//                BLUE_LED_Write(ON);
//                ServiceToClient("AT+ON",sizeof("AT+ON"));
//            }
//            else if(0 == RxData[Location+1]-0x30)//控制开关为关
//            {
//                BLUE_LED_Write(OFF);
//                ServiceToClient("AT+OFF",sizeof("AT+OFF"));
//            }
//        break;
//        default:
//        break;
//    }
}

/******************************************
  * @函数名：ClientData_Handler
  * @输入：  RxData---指向接收到的从机透传数据的首地址              
  * @返回：  NULL            
  * @描述：  处理主机透传给从机的数据
  *****************************************
*/
void  ServerData_Handler(const char* RxData)
{
//    RxData = RxData;
    uint8_t Location,Index;       
    CYBLE_API_RESULT_T API_RESULT;    
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
                switch(ButtonType)
                {
                    case BUTTON1:
                        Button.Button1_On_Off=SWTICH_ON;
                    break;
                    case BUTTON2:
                        Button.Button2_On_Off=SWTICH_ON;
                    break;
                    case BUTTON3:
                        Button.Button3_On_Off=SWTICH_ON;
                    break;
                    case BUTTON4:
                        Button.Button4_On_Off=SWTICH_ON;
                    break;
                    case BUTTON5:
                        if((Button.Button5_On_Off^1) == SWTICH_ON)//如果设备返回的是打开开关状态，而我主机也要打开开关，则不去再次打开该设备，直接断开连接返回即可
                        {
                            CyBle_GapDisconnect(cyBle_connHandle.bdHandle);//接收到从机的状态返回马上断开连接 
                            ALL_ON_OFF = TURE;
                            return ;
                        }
                        else
                        {
                            WriteCmd.attrHandle = CYBLE_TROUGHPUT_SERVICE_RX_CHAR_HANDLE;//从机接收属性的句柄值
                            WriteCmd.value.len=8;//发送的数据长度为8个字节
                            WriteCmd.value.val = (uint8_t *)CommandList[Button.Button5_On_Off^1];//需要发送给从机的数据
                            API_RESULT=CyBle_GattcWriteWithoutResponse(cyBle_connHandle, &WriteCmd);//开始发送
                            if(API_RESULT == CYBLE_ERROR_OK)//控制命令发送成功，则断开连接
                            {
                                CyBle_GapDisconnect(cyBle_connHandle.bdHandle);//接收到从机的状态返回马上断开连接     
                                ALL_ON_OFF = TURE;
                            }                            
                        }
                    break;
                    default:
                    break;
                }
            }
            else if(0 == RxData[Location+1]-0x30)//控制开关为关
            {
                switch(ButtonType)
                {
                    case BUTTON1:
                        Button.Button1_On_Off=SWTICH_OFF;
                    break;
                    case BUTTON2:
                        Button.Button2_On_Off=SWTICH_OFF;
                    break;
                    case BUTTON3:
                        Button.Button3_On_Off=SWTICH_OFF;
                    break;
                    case BUTTON4:
                        Button.Button4_On_Off=SWTICH_OFF;
                    break;
                    case BUTTON5:
                        if((Button.Button5_On_Off^1) == SWTICH_OFF)//如果设备返回的是关闭开关状态，而我主机也要关闭开关，则不去再次关闭该设备，直接断开连接返回即可
                        {
                            CyBle_GapDisconnect(cyBle_connHandle.bdHandle);//接收到从机的状态返回马上断开连接 
                            ALL_ON_OFF = TURE;
                            return ;
                        }
                        else
                        {
                            WriteCmd.attrHandle = CYBLE_TROUGHPUT_SERVICE_RX_CHAR_HANDLE;//从机接收属性的句柄值
                            WriteCmd.value.len=8;//发送的数据长度为8个字节
                            WriteCmd.value.val = (uint8_t *)CommandList[Button.Button5_On_Off^1];//需要发送给从机的数据
                            API_RESULT=CyBle_GattcWriteWithoutResponse(cyBle_connHandle, &WriteCmd);//开始发送
                            if(API_RESULT == CYBLE_ERROR_OK)//控制命令发送成功，则断开连接
                            {
                                CyBle_GapDisconnect(cyBle_connHandle.bdHandle);//接收到从机的状态返回马上断开连接     
                                ALL_ON_OFF = TURE;
                                return ;
                            }                            
                        }
                    break;
                    default:
                    break;
                }
            }
            WriteCmd.attrHandle = CYBLE_TROUGHPUT_SERVICE_RX_CHAR_HANDLE;//从机接收属性的句柄值
            WriteCmd.value.len=8;//发送的数据长度为8个字节
            switch(ButtonType)
            {
                case BUTTON1:
                    WriteCmd.value.val = (uint8_t *)CommandList[Button.Button1_On_Off^1];//需要发送给从机的数据
                break;
                case BUTTON2:
                    WriteCmd.value.val = (uint8_t *)CommandList[Button.Button2_On_Off^1];//需要发送给从机的数据
                break;
                case BUTTON3:
                    WriteCmd.value.val = (uint8_t *)CommandList[Button.Button3_On_Off^1];//需要发送给从机的数据
                break;
                case BUTTON4:
                    WriteCmd.value.val = (uint8_t *)CommandList[Button.Button4_On_Off^1];//需要发送给从机的数据
                break;
                default:
                break;
            }
            API_RESULT=CyBle_GattcWriteWithoutResponse(cyBle_connHandle, &WriteCmd);//开始发送
            if(API_RESULT == CYBLE_ERROR_OK)//控制命令发送成功，则断开连接
            {
                CyBle_GapDisconnect(cyBle_connHandle.bdHandle);//接收到从机的状态返回马上断开连接
            }                         
        break;
        default:
        break;
    }
}

/******************************************
  * @函数名：SystemInit
  * @输入：  NULL 
  * @返回：  NULL            
  * @描述：  系统初始化
  *****************************************
*/
void SystemInit(void)
{   
    UART_Start();
    isr_Button_StartEx(BT1_IntHandler);
//    isr_Button2_StartEx(BT2_IntHandler);
//    isr_Button3_StartEx(BT3_IntHandler);
//    isr_Button4_StartEx(BT4_IntHandler);
    CyBle_Start(StackEventHandler);//BLE协议栈初始化    
}

/******************************************
  * @函数名：BT1_IntHandler
  * @输入：  NULL              
  * @返回：  NULL            
  * @描述：  处理按键1事件
  *****************************************
*/
CY_ISR(BT1_IntHandler)
{   
    uint8_t IntPin;    
    CyDelay(200);
    IntPin=Button_ClearInterrupt();
//    isr_Button_ClearPending();
//    IntPin = Button_INTSTAT;
    #ifdef PRINT
//        printf("IntPin is %d!\r\n",IntPin);
    #endif
    switch(IntPin)
    {
        case 0x01:
            ButtonStatus=BUTTON1_DOWN;
            ButtonType=BUTTON1;
            #ifdef PRINT
                printf("Button1 is pressed!\r\n");
            #endif
        break;
        case 0x02:
            ButtonStatus=BUTTON2_DOWN;
            ButtonType=BUTTON2;   
            #ifdef PRINT
                printf("Button2 is pressed!\r\n");
            #endif
        break;
        case 0x04:
            ButtonStatus=BUTTON3_DOWN;
            ButtonType=BUTTON3;
            #ifdef PRINT
                printf("Button3 is pressed!\r\n");
            #endif
        break;
        case 0x08:
            ButtonStatus=BUTTON4_DOWN;
            ButtonType=BUTTON4;
            #ifdef PRINT
                printf("Button4 is pressed!\r\n");
            #endif
        break;
        case 0x10:
        ButtonStatus=BUTTON5_DOWN;
        ButtonType=BUTTON5;
        #ifdef PRINT
            printf("Button5 is pressed!\r\n");
        #endif
        break;
        default:
        break;
    }    
//    Button_ClearInterrupt();
//    isr_Button1_ClearPending();
//    Button1_ClearInterrupt();
//    Switch_On_Off=Switch_On_Off^SWTICH_ON;
}

/******************************************
  * @函数名：BT2_IntHandler
  * @输入：  NULL              
  * @返回：  NULL            
  * @描述：  处理按键2事件
  *****************************************
*/
CY_ISR(BT2_IntHandler)
{    
    ButtonStatus=BUTTON2_DOWN;
    ButtonType=BUTTON2;   
    #ifdef PRINT
        printf("Button2 is pressed!\r\n");
    #endif
//    isr_Button2_ClearPending();
//    Button2_ClearInterrupt();
}
/******************************************
  * @函数名：BT3_IntHandler
  * @输入：  NULL              
  * @返回：  NULL            
  * @描述：  处理按键3事件
  *****************************************
*/
CY_ISR(BT3_IntHandler)
{    
    ButtonStatus=BUTTON3_DOWN;
    ButtonType=BUTTON3;
    #ifdef PRINT
        printf("Button3 is pressed!\r\n");
    #endif
//    isr_Button3_ClearPending();
//    Button3_ClearInterrupt();
}
/******************************************
  * @函数名：BT4_IntHandler
  * @输入：  NULL
  * @返回：  NULL            
  * @描述：  处理按键4事件
  *****************************************
*/
CY_ISR(BT4_IntHandler)
{    
    ButtonStatus=BUTTON4_DOWN;
    ButtonType=BUTTON4;
    #ifdef PRINT
        printf("Button4 is pressed!\r\n");
    #endif
//    isr_Button4_ClearPending();
//    Button4_ClearInterrupt();
}

/******************************************
  * @函数名：ButtonHandler
  * @输入：  NULL
  * @返回：  NULL            
  * @描述：  处理所有的按键事件
  *****************************************
*/
void ButtonHandler(void)
{
//    uint8_t Button;
    CYBLE_API_RESULT_T API_RESULT;
    if(0 == BondNum)//如果绑定的设备为0，则所有开关按键功能都无效
    {
        ButtonStatus = BUTTON_UP;
        return ;
    }
    switch(ButtonStatus)
    {
        case BUTTON1_DOWN:
            ButtonStatus = BUTTON_UP;
            API_RESULT=CyBle_GapcConnectDevice(&Device1);//建立连接&BondList.bdAddrList[0]
            if(API_RESULT == CYBLE_ERROR_OK)
            {
                
            }
        break;
        case BUTTON2_DOWN:
            ButtonStatus = BUTTON_UP;
            API_RESULT=CyBle_GapcConnectDevice(&Device2);//建立连接&BondList.bdAddrList[0]
            if(API_RESULT == CYBLE_ERROR_OK)
            {
                
            }
        break;
        case BUTTON3_DOWN:
            ButtonStatus = BUTTON_UP;
            API_RESULT=CyBle_GapcConnectDevice(&Device3);//建立连接&BondList.bdAddrList[0]
            if(API_RESULT == CYBLE_ERROR_OK)
            {
                
            }
        break;
        case BUTTON4_DOWN:
            ButtonStatus = BUTTON_UP;            
            API_RESULT=CyBle_GapcConnectDevice(&Device4);//建立连接&BondList.bdAddrList[0]
            if(API_RESULT == CYBLE_ERROR_OK)
            {
                
            }
        break;
        case BUTTON5_DOWN://关闭或者打所有灯
            ButtonStatus = BUTTON_UP;
//            Button.Button5_On_Off = Button.Button5_On_Off^1;
            ALL_ON_OFF = TURE;
//            RX_ISOVER = TURE;
//            strncpy((char*)Buffer,"AT+SWT=1",8);
        break;
        default:
        break;
    }
}

/******************************************
  * @函数名：ALL_On_Off_Handler
  * @输入：  NULL
  * @返回：  NULL            
  * @描述：  ALL ON/OFF处理
  *****************************************
*/
void ALL_On_Off_Handler(void)
{
    CYBLE_API_RESULT_T API_RESULT;    
    if((DeviceCounts >= BondNum)&&(DisonnectedStaus == TURE))//如果大于绑定的设备则只控制完绑定的设备就可以了
    {
        DisonnectedStaus = FALSE;
        ALL_ON_OFF = FALSE;
        Button.Button5_On_Off = Button.Button5_On_Off^1;
        DeviceCounts = 0;
    }
    else
    {
        switch(DeviceCounts)
        {
            case 0:            
                API_RESULT=CyBle_GapcConnectDevice(&Device1);
                if(API_RESULT == CYBLE_ERROR_OK)
                {
                    DeviceCounts++;
    //                DeviceNum = DEVICE1;
                }
            break;
            case 1:
                if(DisonnectedStaus == TURE)
                {
                    API_RESULT=CyBle_GapcConnectDevice(&Device2);
                    if(API_RESULT == CYBLE_ERROR_OK)
                    {
                        DisonnectedStaus = FALSE;
                        DeviceCounts++;
    //                    DeviceNum = DEVICE2;
                    }
                }
            break;
            case 2:
                if(DisonnectedStaus == TURE)
                {
                    API_RESULT=CyBle_GapcConnectDevice(&Device3);
                    if(API_RESULT == CYBLE_ERROR_OK)
                    {
                        DisonnectedStaus = FALSE;
                        DeviceCounts++;
    //                    DeviceNum = DEVICE3;
                    }
                }                
            break;
            case 3:
            if(DisonnectedStaus == TURE)
            {
                API_RESULT=CyBle_GapcConnectDevice(&Device4);
                if(API_RESULT == CYBLE_ERROR_OK)
                {
                    DisonnectedStaus = FALSE;
//                    ALL_ON_OFF = FALSE;
                    DeviceCounts++;
    //                DeviceNum = DEVICE4;
                }
            }            
            break;
//            case 4:
//                if(DisonnectedStaus == TURE)
//                {
//                    DisonnectedStaus = FALSE;
//                    ALL_ON_OFF = FALSE;
//                    Button.Button5_On_Off = Button.Button5_On_Off^1;
//                    DeviceCounts = 0;
//    //              RX_ISOVER = FALSE;
//                    DeviceNum = INVALID;
//                }                        
//            break;
            default:                            
            break;
        }  
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
                /* 处理唤醒后的事件 */
                LOWPOWER=FALSE;
                WakeUp=TURE;
		 	}
		}
		else if((sleepMode == CYBLE_BLESS_SLEEP)||(sleepMode == CYBLE_BLESS_ACTIVE))
		{
		    if(blessState != CYBLE_BLESS_STATE_EVENT_CLOSE)
		    {
				/* If the BLESS hardware block cannot go to Deep Sleep and BLE Event has not 
				* closed yet, then place CPU to Sleep */ 
		        CySysPmSleep();  
                /* 处理唤醒后的事件 */
                LOWPOWER=FALSE;
                WakeUp=TURE;
		    }
		}
		/* Re-enable global interrupt mask after wakeup */
		CyGlobalIntEnable;           
        
}
/* [] END OF FILE */
