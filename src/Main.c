/**
  ******************************************************************************
  * @file    :Main.c
  * @author  :MG Team
  * @version :V1.0
  * @date
  * @brief
  ******************************************************************************
***/

/* Includes ------------------------------------------------------------------*/
#include "Includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//workmode:iBeacon & Eddyston & Remote
volatile uint8_t Work_Mode = Work_Mode_Help;


//fcunction
#ifdef NO_HELP

#define LINE_DISP 3
char *FunctionDisplay[LINE_DISP]={
  "*********************************************************\r\n",
  "+ MacroGiga Electronics Demo 2016.08                    +\r\n"
  "*********************************************************\r\n",
};

#else

#define LINE_DISP 11
char *FunctionDisplay[LINE_DISP]={
	"---------------------------iBeacon-----------------------\r\n",
	"++  1--Enable iBeacon function ,  APP: Wechat          ++\r\n",
	"*********************************************************\r\n",
	"---------------------------Eddystone---------------------\r\n",
	"++  2--Enable Eddystone Uri,    APP: EddystoneValidator++\r\n",
	"++  3--Enable Eddystone Uid,    APP: EddystoneValidator++\r\n",
	"++  4--Enable Eddystone Tlm,    APP: EddystoneValidator++\r\n",
	"++  5--Enable Remote function, 1  key function         ++\r\n",
	"*********************************************************\r\n",
	"++  0--Display all command                             ++\r\n",
	"---------------------------------------------------------\r\n",
};
#endif

/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************
* Function	 :		main
* Parameter  :		void
* Returns	 :		void
* Description:
* Note: 	 :
*******************************************************************************/
void main(void)
{
    uint8_t temp0=0;

    //init mcu system
    Init_System();
    Rx_Buffer[Rx_Tx_Buffer_Cnt] = 0;
    Rx_Tx_Buffer_Cnt = 0;

    while(1)
    {
		//rx data . change work mode .
		if((Rx_Tx_Buffer_Cnt == 1) && (Work_Mode == Work_Mode_Null))
		{
			Work_Mode = Rx_Buffer[0];
			Rx_Buffer[0] = 0;
			Rx_Tx_Buffer_Cnt = 0;
		}


		switch(Work_Mode)
		{
			case Work_Mode_Null:

				break;
			case Work_Mode_Help:

				for(temp0=0;temp0<LINE_DISP;temp0++)
				{
					Uart_Send_String(FunctionDisplay[temp0]);
				}

				Uart_Send_String("\r\n");
				Uart_Send_String("\r\n");
				Uart_Send_String("\r\n");
				Uart_Send_String("\r\n");
				Uart_Send_String("\r\n");
				Uart_Send_String("\r\n");
#ifndef NO_HELP
				Work_Mode = Work_Mode_Null;
#else
                Work_Mode = Work_Mode_iBeacon;
#endif
			break;
			case Work_Mode_iBeacon:
			case Work_Mode_Eddystone_Uri:
			case Work_Mode_Eddystone_Uid:
			case Work_Mode_Eddystone_Tlm:
			case Work_Mode_Remote:
                /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
                /* IWDG timeout equal to 1.7 s (the timeout may varies due to LSI frequency dispersion) */
                /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI = (254 + 1) * 256 / 38 000 = 1717.6 ms */
                /* Enable the IWDG */
                IWDG_Enable();
                /* Enable write access to IWDG_PR and IWDG_RLR registers */
                IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
                /* Set IWDG timeout */
                IWDG_SetPrescaler(IWDG_Prescaler_256);
                IWDG_SetReload(254); //RELOAD_VALUE
                /* Refresh IWDG */
                IWDG_ReloadCounter();

				//BLE initnal
				BLE_Init();

                //rtx
                BLE_TRX(3,3);
			break;

			default:
			break;
		}
    }
}

