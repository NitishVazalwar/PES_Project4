#include <MKL25Z4.H>
#include "i2c.h"
int lock_detect=0;
int i2c_lock=0;

//init i2c0
void i2c_init(void)
{
	//clock i2c peripheral and port E
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

	//set pins to I2C function
	PORTE->PCR[24] |= PORT_PCR_MUX(5);
	PORTE->PCR[25] |= PORT_PCR_MUX(5);

	//set to 100k baud
	//baud = bus freq/(scl_div+mul)
 	//~400k = 24M/(64); icr=0x12 sets scl_div to 64

 	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	// Select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}


void i2c_busy(void){
	// Start Signal
	lock_detect=0;
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C_TRAN;
	I2C_M_START;
	I2C0->C1 |=  I2C_C1_IICEN_MASK;
	// Write to clear line
	I2C0->C1 |= I2C_C1_MST_MASK; /* set MASTER mode */
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->D = 0xFF;
	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U) {
	} /* wait interrupt */
	I2C0->S |= I2C_S_IICIF_MASK; /* clear interrupt bit */


							/* Clear arbitration error flag*/
	I2C0->S |= I2C_S_ARBL_MASK;


							/* Send start */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK; /* Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK; /* START signal generated */

	I2C0->C1 |= I2C_C1_IICEN_MASK;
							/*Wait until start is send*/

							/* Send stop */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_MST_MASK; /* set SLAVE mode */
	I2C0->C1 &= ~I2C_C1_TX_MASK; /* Set Rx */
	I2C0->C1 |= I2C_C1_IICEN_MASK;


								/* wait */
							/* Clear arbitration error & interrupt flag*/
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->S |= I2C_S_ARBL_MASK;
	lock_detect=0;
	i2c_lock=1;
}

#pragma no_inline
void i2c_wait(void) {
	lock_detect = 0;
	while(((I2C0->S & I2C_S_IICIF_MASK)==0) & (lock_detect < 200)) {
		lock_detect++;
	}
	if (lock_detect >= 200)
		i2c_busy();
	I2C0->S |= I2C_S_IICIF_MASK;
}

//send start sequence
void i2c_start()
{
	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
}

//send device and register addresses
#pragma no_inline
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */

	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */

	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */

	I2C_REC;						   /*set to receive mode */

}

//read a byte and ack/nack as appropriate
// #pragma no_inline
uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;

	lock_detect = 0;

	if(isLastRead)	{
		NACK;								/*set NACK after read	*/
	} else	{
		ACK;								/*ACK after read	*/
	}

	data = I2C0->D;				/*dummy read	*/
	I2C_WAIT							/*wait for completion */

	if(isLastRead)	{
		I2C_M_STOP;					/*send stop	*/
	}
	data = I2C0->D;				/*read data	*/

	return  data;
}



//////////funcs for reading and writing a single byte
//using 7bit addressing reads a byte from dev:address
// #pragma no_inline
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT							/*wait for completion */

	I2C0->D =  address;		/*send read address	*/
	I2C_WAIT							/*wait for completion */

	I2C_M_RSTART;				   /*repeated start */
	I2C0->D = (dev|0x1);	 /*send dev address (read)	*/
	I2C_WAIT							 /*wait for completion */

	I2C_REC;						   /*set to recieve mode */
	NACK;									 /*set NACK after read	*/

	data = I2C0->D;					/*dummy read	*/
	I2C_WAIT								/*wait for completion */

	I2C_M_STOP;							/*send stop	*/
	data = I2C0->D;					/*read data	*/

	return data;
}



//using 7bit addressing writes a byte data to dev:address
#pragma no_inline
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{

	I2C_TRAN;							/*set to transmit mode */
	I2C_M_START;					/*send start	*/
	I2C0->D = dev;			  /*send dev address	*/
	I2C_WAIT						  /*wait for ack */

	I2C0->D =  address;		/*send write address	*/
	I2C_WAIT

	I2C0->D = data;				/*send data	*/
	I2C_WAIT
	I2C_M_STOP;

}
void Enable_irq (int irq)
{
    /* Make sure that the IRQ is an allowable number. Up to 32 is
     * used.
     *
     * NOTE: If you are using the interrupt definitions from the header
     * file, you MUST SUBTRACT 16!!!
     */

      /* Set the ICPR and ISER registers accordingly */
      NVIC->ICPR[0] |= 1 << (irq%32);
      NVIC->ISER[0] |= 1 << (irq%32);
}



void I2C_Master_Init(void)
{
	/* Enable clock for I2C0 module */
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;

	/* Enable clock for Port E */
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;


	/* Port E MUX configuration */
	PORTE->PCR[24] |=  PORT_PCR_MUX(5);
	PORTE->PCR[25] |=  PORT_PCR_MUX(5);


	/* Configure Divider Register */
	I2C0->F |= (I2C_F_MULT(2) | I2C_F_ICR(22));

	/* Enable I2C module and interrupt */
	I2C0->C1 |= I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK;


	/* Enable TX mode */
	I2C0->C1 |= I2C_C1_TX_MASK;

	/* Enable I2C0 NVIC interrupt */
	Enable_irq(8);
	printf("\nMaster init is done");
}




void I2C_Master_Transmit(void)
{
	I2C0->C1 |= I2C_C1_MST_MASK;		//  Generate START SIGNAL
	I2C0->D = ((0x3A)<<1|0x01);// Write 7-bit Slave Address + READ bit
	printf("\nMaster transmit is done");
}




void I2C0_IRQHandler(void)
{
	u8 status = 0x00;
	u8 dummy_var;

	status = I2C0->S;	// Read status

	I2C0->S |= I2C_S_IICIF_MASK;	// Clear interrupt flag

	if(I2C0->C1 & I2C_C1_TX_MASK) // Transmitter mode?
	{
		if((status & I2C_S_RXAK_MASK) == 0) // ACK Received?
		{
			I2C0->C1 &= ~I2C_C1_TX_MASK;	// Change to receiver mode
			dummy_var = I2C0->D;	// Dummy read to start reception
		}
		else
		{
			I2C0->C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
		}
	}
	else
	{
		byte_counter++;		// Increment the count of bytes received

		if(byte_counter == BUFFER_SIZE)	// Last byte to be received?
		{
			I2C0->C1 &= ~I2C_C1_MST_MASK;	// Generate STOP signal
		}
		else
		{
			if(byte_counter == (BUFFER_SIZE - 1))	// Only 1 more byte pending to read?
			{
				I2C0->C1 |= I2C_C1_TXAK_MASK;	// Generate NACK in the next reception
			}
		}

		rx_buffer[byte_counter-1] = I2C0->D;// Copy data register to buffer
		//rx_buffer[0]='c';
	}
}

