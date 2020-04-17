/*
 * Statemachine2 is table driven.
 */
#include "i2c.h"
#include "mma8541.h"
#include "touchSen.h"
#include <stdio.h>
#include "statemachine1.h"
#include "led.h"
static int Current_state=0;
static int Current_event=0;

typedef enum states2{
	sReadXYZ2,
	sDisplay2,
	sPollSlider2,
	sDisconnect2,
	nextStatemachine2,
	sEnd2

}systemstate;

typedef enum events2{
	ePass2,
	eFail2,
	eDisconnnected2,
	eComplete2,
	eLeftSlider2,
	eRightSlider2,
	eTimeout2,
	eFinish2
};

void PollSliderHandler(void)
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
							currentevent=eLeftSlider;state=nextStatemachine;Statemachine1();

						}
						else if(touch_val>1000)
						{
							currentevent=eRightSlider;state=sEnd;Statemachine1();
						}



					}
}
typedef void(*functionPointerType)(void);

struct commandStruct{
	int *name;
	functionPointerType execute;
};

void DisconectHandler(void)
{
	state=3;
	Statemachine1();
}

void NextSM(void)
{
	state=4;
	Statemachine1();
}

void EndHandler(void)
{
	state=5;
	Statemachine1();
}

const struct commandStruct commands[6]= {
  		{sReadXYZ2, &read_full_xyz},
  		{sDisplay2, &DisplayXYZ},
  		{sPollSlider2, &PollSliderHandler},
  		{sDisconnect2, &DisconectHandler},
  		{nextStatemachine2, &NextSM},
  		{sEnd2, &EndHandler},
//			{"",0,""} /* End of table indicator */
  };

const struct commandStruct *commandPtr = commands;
/* Declare function pointer */
   void (*command)(void);

 void statemachine2()
 {
	 uint8_t cmdReturn;

 	for (uint8_t i = 0; i < 6; i++)
 	{
   		commandPtr = commands; /* Set ptr back to beginning */
   		for (uint8_t j = 0; j < 6; j++)
   		{	/* Iterate through every command */
   			//if (commandStruct[i].name == commands[j].name)

   			{
   				command = commandPtr->execute;
   				command();
   				if (cmdReturn)
   				{ /* error */
   					Control_RGB_LEDs(1, 0, 0);
   				} else
   				{ /* success */
   					Control_RGB_LEDs(0, 1, 0);
   				}
   			}
   			/* Next pointer element */
   			commandPtr++;
   		}
   	}
 }






//systemstate ReadXYZHandler(void)
//{
//    printf("inReadXYZ");
//   eNewevent=eComplete;
//
//	return sDisplay;
//}
//
//systemstate DisplayHandler(void)
//{
//	printf("inDisplay");
//	eNewevent=eComplete;
//	return sPollSlider;
//}
//
//systemstate PollSliderHandler(void)
//{
//	printf("inPollSlider");
//	return sReadXYZ;
//}
//
//systemstate DisconnectHandler(void)
//{
//	printf("inDisconnect");
//	return sDisconnect;
//}
//
//systemstate SM1Handler(void)
//{
//	printf("inSM!");
//	return nextStatemachine;
//}
//
//systemstate EndHandler(void)
//{
//	printf("inEnd");
//	return sEnd;
//}
//
//void statemachine2(void)
//{
//	//printf("\nInside statemachine2");
//
//	if(statetable[Current_state][Current_event]==sReadXYZ2)
//		{	Current_state=sReadXYZ2;
//			read_full_xyz();
//			printf("\nRead is done");
//			Current_state=sDisplay2;
//			Current_event=eComplete2;
//			statemachine2();
//		}
//		else if(statetable[Current_state][Current_event]==sDisplay2)
//		{	Current_state=sDisplay2;
//			DisplayXYZ();
//			avg_xyz();
//			statemachine2();
//		}
//		else if(statetable[Current_state][Current_event]==sPollSlider2)
//		{	Current_state=sPollSlider;
//			printf("\nPoll Slider");
//			statemachine2();
//		}
//		else if (statetable[Current_state][Current_event]==sEnd2)
//		{	Current_state=sEnd2;
//			state=sEnd; //for statemachine 1
//			Statemachine1();
//		}
//		else if(statetable[Current_state][Current_event]==sDisconnect2)
//		{
//			Current_state=sDisconnect2;
//			state=sDisconnect;
//			Statemachine1();
//		}
//		else
//		{
//			printf("\nError");
//			printf("\nCurrent_State is %d",Current_state);
//			printf("\nCurrent_Event is %d",Current_event);
//		}
//
//
//}



