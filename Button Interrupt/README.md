# Button Interrupt

Make a LED change states when the button is pressed using interrupts on 5 different MSP430 microcontrollers:

* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989

Using interrupts help the processor run alot more efficiently by processing information only when something happens to act upon it. The following code will tell the compiler that when a particular interrupt occurs, run this code  

```c
	#pragma vector=PORT1_VECTOR
	__interrupt void Port_1(void)
	{
	}
```
## Code Configuration

The following code can be used on all of the boards. The only change that is needed to be made is the pin assignments labeled as x's for LED and y's for Button

```c
	void main(void)
	{
    		WDTCTL = WDTPW + WDTHOLD;

    		PM5CTL0 &= ~LOCKLPM5; // Disable high impedance mode
				      // Use only for FR6989, FR5994, and FR2311 
    		PxDIR = BITx;
    		PyREN = BITy;
    		PyOUT = BITy;
    		PyIE = BITy;
    		__enable_interrupt();

   		_BIS_SR(LPM4_bits + GIE); // Enter LPM4 mode and enable global interrupt
					  // Use only for FR6989, FR5994, and FR2311 
	}

	#pragma vector=PORTy_VECTOR
	__interrupt void Port_y(void)
	{
   		__delay_cycles(500000);
    		PxOUT ^= BITx;
    		PyIFG = ~BITy;
	}
```
## Pin Assignments

Depending on which LED is chosen to blink, the following pin assignments will declare the corresponding LED to blink:

```c		
		   LED1		LED2	BUTTON
* MSP430G2553	=> P1.0 	P1.6	P1.3
* MSP430F5529	=> P1.0 	P4.7	P2.1
* MSP430FR2311	=> P1.0 	P2.0	P1.1
* MSP430FR5994	=> P1.0 	P1.1	P5.6
* MSP430FR6989	=> P1.0 	P9.7	P1.1
```

## Code Example

Use interrupts on the MSP430F5529 to blink an LED on/off using a button

```c
	void main(void)
	{
    		WDTCTL = WDTPW + WDTHOLD;
    		P1DIR = BIT0;
    		P2REN = BIT1;
    		P2OUT = BIT1;
    		P2IE = BIT1;	// Port 2 Interrupt Enable (Changes to this pin will trigger an interrupt)
    		__enable_interrupt(); // Enable the interupt
	}

	#pragma vector=PORT2_VECTOR
	__interrupt void Port_2(void)
	{
    		__delay_cycles(500000);
   		P1OUT ^= BIT0;
    		P2IFG = ~BIT1;	//Disable the interrupt flag
	}
```

In this code example Port 2 has an Interrupt Enabled, thus everytime there is a change to this pin, like a button push, code inside the "#pragma vector" will run causing the LED to turn on/off 
