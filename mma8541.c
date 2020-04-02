#include <MKL25Z4.H>
#include "mma8541.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#include<stdio.h>
int16_t acc_X=0, acc_Y=0, acc_Z=0;
int16_t te_X,te_Y,te_Z=0;
int16_t avg_X,avg_Y,avg_Z;
static uint16_t ctr=1;
float roll=0.0, pitch=0.0;

//mma data ready
extern uint32_t DATA_READY;


//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma()
{
	//set active mode, 14 bit samples and 800 Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;



	//printf("\n\r%d\n",acc_X);
}

void avg_xyz(void)
{


	{
	te_X=te_X+acc_X;
	te_Y=te_Y+acc_Y;
	te_Z=te_Z+acc_Z;
	//ctr++;
	avg_X=(te_X)/ctr;
	avg_Y=(te_Y)/ctr;
	avg_Z=(te_Z)/ctr;
	}
	ctr++;
	printf("\nAverage is %d",avg_X);
	printf("\nAverage is %d",avg_Y);
	printf("\nAverage is %d",avg_Z);
}

void DisplayXYZ(void)
{
	printf("\nXcoordinate is %d",acc_X);
	printf("\nYcoordinate is %d",acc_Y);
	printf("\nZcoordinate is %d",acc_Z);
	printf("\n");
}

void read_xyz(void)
{
	// sign extend byte to 16 bits - need to cast to signed since function
	// returns uint8_t which is unsigned
	acc_X = (int8_t) i2c_read_byte(MMA_ADDR, REG_XHI);
	Delay(100);
	acc_Y = (int8_t) i2c_read_byte(MMA_ADDR, REG_YHI);
	Delay(100);
	acc_Z = (int8_t) i2c_read_byte(MMA_ADDR, REG_ZHI);

}

void convert_xyz_to_roll_pitch(void) {
	float ax = acc_X/COUNTS_PER_G,
				ay = acc_Y/COUNTS_PER_G,
				az = acc_Z/COUNTS_PER_G;

	roll = atan2(ay, az)*180/M_PI;
	pitch = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;

}
void read_full_xyz1()
{
	//I2C_Master_Init();	// Initialize I2C module in master mode

		//I2C_Master_Transmit();	// Start transmission
		//printf("\nDone");
		I2C0_IRQHandler();
		//for(int i=0;i<=BUFFER_SIZE;i++){printf("\n\rvalue is %d",rx_buffer[i]);}
	int i;
	//uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		rx_buffer[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	rx_buffer[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((rx_buffer[2*i]<<8) | rx_buffer[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;



	//printf("\n\r%d\n",acc_X);
}


//mma data ready irq
// void PORTA_IRQHandler()
// {
//	NVIC_ClearPendingIRQ(PORTA_IRQn);
// 	DATA_READY = 1;
// }


