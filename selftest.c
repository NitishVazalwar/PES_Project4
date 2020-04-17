/*
 * selftest.c
 *
 *  Created on: Mar 24, 2020
 *      Author: nitis
 */

#include <stdint.h>
#include "mma8541.h"
#include "i2c.h"
#include <stdio.h>

int self_test(void)
{
	uint8_t cntrl_reg = 0x2B;  //control register2
	uint8_t data = 0x80;	//normal mode, 400Hz, xyz-enabled, self test value 0b1000 0000
	int16_t pre_x, pre_y, pre_z;
	int16_t post_x, post_y, post_z;
	int16_t diff_x, diff_y, diff_z;
	int8_t flag_x, flag_y, flag_z;
	//accel_read();
	read_full_xyz();
	pre_x = acc_X;
	pre_y = acc_Y;
	pre_z = acc_Z;
	//i2c_write_byte(dev_id, cntrl_reg, data); //Full scale, Self-test enabled, self-test plus mode
	i2c_write_byte(MMA_ADDR, cntrl_reg, data);
	//accel_read();
	read_full_xyz();
	post_x = acc_X;
	post_y = acc_Y;
	post_z = acc_Z;
	diff_x = post_x - pre_x;
	if(diff_x<0){diff_x=-diff_x;}
	diff_y = -(post_y - pre_y);
	if(diff_y<0){diff_y=-diff_y;}
	diff_z = post_z - pre_z;
	if(diff_z<0){diff_z=-diff_z;}
	printf("\nDiff_x is %d ",diff_x);
	printf("\nDiff_y is %d ",diff_y);
	printf("\nDiff_z is %d ",diff_z);
	if((diff_x <= 500))// && (diff_x <= 180))
	{
		flag_x = 1;
		printf("Flag x is set");
	}
	if((diff_y <= 300))// && (diff_y <= 180))
	{
		flag_y = 1;
		printf("Flag y is set");
	}
	if((diff_z >=0))// && (diff_y <= 370))
	{
		flag_z = 1;
		printf("Flag z is set");
	}
	if((flag_x & flag_y & flag_z) == 1)
	{
		data=0x00;
		i2c_write_byte(MMA_ADDR, cntrl_reg, data);
		printf("\nSelf Test pass!");
		return 0;
	}
	else
	{
		data=0x00;
		i2c_write_byte(MMA_ADDR, cntrl_reg, data);
		printf("\nSelf Test failed!");
		return 1;
	}
}


