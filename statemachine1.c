/*
 * Statemachine1
 */
#include <stdio.h>
#include <stdint.h>
#include "i2c.h"
#include "mma8541.h"
#include "touchSen.h"
#include "statemachine2.h"
#include "statemachine1.h"
#include "timer.h"
#include "delay.h"

uint8_t n=0;


int currentevent=ePass;

void Statemachine1(void)
{
	switch (state)
	{
	case sReadXYZ:
		{
			if(currentevent==ePass || currentevent==eTimeout)
			{
				read_full_xyz();
				if(acc_X!=0)
				{
					currentevent=eComplete;
				}
				else
				{
					currentevent=eDisconnnected;
				}
			}

			else
			{
				printf("\nTerminated");
			}
			//if(readDone!=0){state=sDisconnect;break;}
			if(currentevent==eComplete)
			{
				state=sDisplay;
			}
			else if(currentevent==eDisconnnected)
			{
				state=sDisconnect;
			}
			break;

		}
	case sDisplay :
		{
			if(currentevent==eComplete)
			{
				DisplayXYZ();
				printf("\n-------");
				avg_xyz();
				printf("\n-------");
				state=sPollSlider;
				currentevent=eComplete;
			}

			break;
		}
	case sPollSlider :
		{


			if(currentevent==eComplete)
			{
				Touch_Init();
				Init_SysTick();

				while(state==sPollSlider)
				{




					uint16_t touch_val=0;

					touch_val=Touch_Scan_LH1();
					printf("\nTouch_val has value is %d",touch_val);
					if (touch_val>=800 && touch_val<1000)
					{
						currentevent=eLeftSlider;state=nextStatemachine;break;

					}
					else if(touch_val>1000)
					{
						currentevent=eRightSlider;state=sEnd;break;
					}



				}
			}


			break;
		}

	case sDisconnect :
	{
		if(currentevent==eDisconnnected)
		{
			printf("\nDisconnected !");
			state=sEnd;
			currentevent=eFinish;
			break;
		}
	}
	case sEnd :
		{
			if(currentevent==eRightSlider||currentevent==eFinish)
			{
				state=5;
				break;
			}
		}
	case nextStatemachine :
		{
			if(currentevent==eLeftSlider || currentevent==eTimeout)
			{
				//statemachine2();
				//printf("\nIn Statemachine2");
				statemachine2();
				break;
			}
		}
	default:{printf("\nIn default state press reset");state=5;break;}
	}
}
