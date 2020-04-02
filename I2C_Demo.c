/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "gpiodefs.h"
#include "led.h"
#include "i2c.h"
#include "mma8541.h"
#include "delay.h"
#include "statemachine1.h"
#include "selftest.h"
#include "typedef.h"
#include "logger.h"
#define ENABLE_LOGGER 1




uint8_t  state=0;
u8 byte_counter = 0;
u8 rx_buffer[BUFFER_SIZE];
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

//int main (void) {
//
//	Init_RGB_LEDs();
//	i2c_init();																/* init i2c	*/
//	if (!init_mma()) {												/* init mma peripheral */
//		Control_RGB_LEDs(1, 0, 0);/* Light red error LED */
//		while (1)																/* not able to initialize mma */
//			;
//	}
//
//	Control_RGB_LEDs(1, 1, 0);
//
//	Delay(100);
//
//	while (1) {
//		read_full_xyz();
//		DisplayXYZ();
//		convert_xyz_to_roll_pitch();
//		// Light green LED if pitch > 10 degrees
//		// Light blue LED if roll > 10 degrees
//		Control_RGB_LEDs(0, (fabs(roll) > 10)? 1:0, (fabs(pitch) > 10)? 1:0);
//	}
//}






int main(void)
{
	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


#ifdef ENABLE_LOGGER
    Log_enable();
#else
    Log_disable();
#endif
   int  status = Log_status();
    Log_string("Logger Status is", NO_NEWLINE);
    Log_integer(status);

	Init_RGB_LEDs();
	i2c_init();
	if (!init_mma()){
		Control_RGB_LEDs(1, 0, 0);
		return 0;
	}
	Delay(500);
	int ST=self_test();
	printf("\nValue of ST is %d",ST);
	if(1==ST)
	{
		Control_RGB_LEDs(1, 0, 0);
		return 1;
	}
	Control_RGB_LEDs(0, 1, 0);
	Delay(100);
	Control_RGB_LEDs(0, 0, 0);
	while(1)
	{
			//if(0==state)
			{
				Statemachine1();
			}
		if(5==state)
		{
			printf("\nEND");
			return 0;
		}

	}
	//printf("\nNot in loop ");
	return 0;
}




//void Enable_irq (int irq)
//{
//    /* Make sure that the IRQ is an allowable number. Up to 32 is
//     * used.
//     *
//     * NOTE: If you are using the interrupt definitions from the header
//     * file, you MUST SUBTRACT 16!!!
//     */
//
//      /* Set the ICPR and ISER registers accordingly */
//      NVIC->ICPR[0] |= 1 << (irq%32);
//      NVIC->ISER[0] |= 1 << (irq%32);
//}
//
//
//
//void I2C_Master_Init(void)
//{
//	/* Enable clock for I2C0 module */
//	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
//
//	/* Enable clock for Port E */
//	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
//
//
//	/* Port E MUX configuration */
//	PORTE->PCR[24] |=  PORT_PCR_MUX(5);
//	PORTE->PCR[25] |=  PORT_PCR_MUX(5);
//
//
//	/* Configure Divider Register */
//	I2C0->F |= (I2C_F_MULT(2) | I2C_F_ICR(22));
//
//	/* Enable I2C module and interrupt */
//	I2C0->C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;
//
//
//	/* Enable TX mode */
//	I2C0->C1 |= I2C_C1_TX_MASK;
//
//	/* Enable I2C0 NVIC interrupt */
//	Enable_irq(8);
//	printf("\nMaster init is done");
//}
//
//
//
//
//void I2C_Master_Transmit(void)
//{
//	I2C0->C1 |= I2C_C1_MST_MASK;		//  Generate START SIGNAL
//	I2C0->D = (0x3B);// Write 7-bit Slave Address + READ bit
//	printf("\nMaster transmit is done");
//}
//
//
//
//
//void I2C0_IRQHandler(void)
//{
//	u8 status = 0x00;
//	u8 dummy_var;
//
//	status = I2C0->S;	// Read status
//
//	I2C0->S |= I2C_S_IICIF_MASK;	// Clear interrupt flag
//
//	if(I2C0->C1 & I2C_C1_TX_MASK) // Transmitter mode?
//	{
//		if((status & I2C_S_RXAK_MASK) == 0) // ACK Received?
//		{
//			I2C0->C1 &= ~I2C_C1_TX_MASK;	// Change to receiver mode
//			dummy_var = I2C0->D;	// Dummy read to start reception
//		}
//		else
//		{
//			I2C0->C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
//		}
//	}
//	else
//	{
//		byte_counter++;		// Increment the count of bytes received
//
//		if(byte_counter == BUFFER_SIZE)	// Last byte to be received?
//		{
//			I2C0->C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
//		}
//		else
//		{
//			if(byte_counter == (BUFFER_SIZE - 1))	// Only 1 more byte pending to read?
//			{
//				I2C0->C1 |= I2C_C1_TXAK_MASK;	// Generate NACK in the next reception
//			}
//		}
//
//		rx_buffer[byte_counter-1] = I2C0->D;// Copy data register to buffer
//		//rx_buffer[0]='c';
//	}
//}
//
//

//int main(void)
//{
//	I2C_Master_Init();	// Initialize I2C module in master mode
//
//	I2C_Master_Transmit();	// Start transmission
//	printf("\nDone");
//	I2C0_IRQHandler();
//	for(int i=0;i<=BUFFER_SIZE;i++){printf("\n\rvalue is %d",rx_buffer[i]);}
//	//for(;;){}
//
//	return 0;
//}




