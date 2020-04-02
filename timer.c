/*
 * timer.c
 *
 *  Created on: Mar 25, 2020
 *      Author: nitis
 */

#include <MKL25Z4.h>
#include "statemachine1.h"
#include <stdio.h>
#include <stdint.h>
#include "touchSen.h"
#include "timer.h"
void Init_SysTick(void)
{
	SysTick->LOAD = (48000000L/48);
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}

void SysTick_Handler()
{
	SysTick->CTRL &=~ (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

	n++;
	//change state
	if(n<6 && state==sPollSlider)
	{
		state=sReadXYZ;
		currentevent=eTimeout;
	}
	else if(n==6 && state==sPollSlider)
	{
		state=nextStatemachine;
		currentevent=eTimeout;
		n=0;
	}
}

