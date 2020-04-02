/*
 * staemachine1.h
 *
 *  Created on: Mar 19, 2020
 *      Author: nitis
 */

#ifndef STATEMACHINE1_H_
#define STATEMACHINE1_H_
extern  uint8_t state;
//extern uint8_t readDone;

enum states{
	sReadXYZ,
	sDisplay,
	sPollSlider,
	sDisconnect,
	nextStatemachine,
	sEnd

};

enum events{
	ePass,
	eFail,
	eDisconnnected,
	eComplete,
	eLeftSlider,
	eRightSlider,
	eTimeout,
	eFinish
};

extern int currentevent;

void Statemachine1(void);


#endif /* STATEMACHINE1_H_ */
