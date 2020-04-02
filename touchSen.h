/*
 * touchSen.h
 *
 *  Created on: Mar 19, 2020
 *      Author: nitis
 */

#ifndef TOUCHSEN_H_
#define TOUCHSEN_H_

#include "MKL25Z4.h"
#include <stdint.h>

// Touch Sensor function prototypes
void Touch_Init(void);
int Touch_Scan_LH(void);
uint16_t Touch_Scan_LH1(void);
int Touch_Scan_HL(void);

// Macros
#define SCAN_OFFSET 544  // Offset for scan range
#define SCAN_DATA TSI0->DATA & 0xFFFF // Accessing the bits held in TSI0_DATA_TSICNT


#endif /* TOUCHSEN_H_ */
