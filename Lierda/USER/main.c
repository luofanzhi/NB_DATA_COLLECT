#include "Systerm_Init.h"
#include "stm32f10x_it.h" 
#include "oled.h"
#include "stdlib.h"
#include "delay.h"
#include "beep.h"
#define Scan_min(n) 60000*n				//定时扫描事件
 int main(void)
{	
	SystemInit();						//系统时钟初始化
	SYSTERM_INIT();						//系统初始化
	OLED_Init();						//OLED初始化
	OLED_ColorTurn(0);					//0正常显示，1- 反色显示
	OLED_DisplayTurn(0);				//0正常显示，1-屏幕翻转显示
	OLED_Clear();						//清屏
	BLUE_POWER_OPEN();					//打开蓝牙电源
	Deveice_Display();					//设备信息显示
	BlueConnectStruct.Connect=0;	
	
  while(1)
	{	
		NB_INIT();						//上电初始化NB，储存EMI号和信号强度
		KEY_Set_Blue_S_Mode();			//短按按键蓝牙切换成从模式和APP通讯
		APP_To_BlueScan(Scan_min(5));	//APP启动扫描, 计时扫描单位：分钟
		Determine_Deveice();			//APP通讯错误信息返回
		JZQ_Write_NAME_MAC();			//APP写表号
		NB_UPDATE_DATA();				//NB上传表数据--最大上传40个蓝牙水表
		//Wake_Deal();					//定时唤醒处理		
		if(BlueConnectStruct.Connect==1)
		{
			if(BLUE_CONNECT_COND==0)
			{
				OLED_Display_GB2312_string(1,6,"蓝牙等待连接..."); 
			}
			if(BLUE_CONNECT_COND==1)
			{
				OLED_Display_GB2312_string(1,6,"蓝牙已连接！！！"); 
				BlueConnectStruct.Connect=2;
			}
		}
		else if(BlueConnectStruct.Connect==2)
		{
			if(BLUE_CONNECT_COND==0)
			{
				BlueConnectStruct.Connect=1;
				
			}
		}	
		
						
	}
 }

 
 
 
