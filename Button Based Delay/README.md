# Button Based Delay

User Timers set a blinking rate of an LED by holding down a button on 5 different MSP430 microcontrollers:

* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989

The duration in which the button is depressed should then become the new rate at which the LED blinks. The longer you hold the button down the longer the delay of the LED will be
 
## Code Configuration

The following code can be used on all of the boards with a slight change in code for the FR2311, change all the timer setups from TA to TB. The only other change that is needed to be made is the pin assignments labeled as x's for LED and y's for Button

```c
	#include <msp430.h>

	int num_cycles(int desired_blink_freq);
	int PushedButton;

	int main(void)
	
    	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    	//PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode
    	int desired_blink_freq = 10;

    	PxDIR |= BITx; // Set LEDs to outputs
    	PxOUT &= ~BITx;

    	PyREN |= BITy;       //setup button 1.1 using pull up resistor and enable
    	PyOUT |= BITy;
    	PyIE |= BITy;
    	PyIES |= BITy;
   	PyIFG &= ~BITy;

    	TA0CCTL0 |= CCIE;
    	TA0CTL |= TASSEL_1 + MC_1;
    	TA0CCTL0 &= 0x10;
    	TA0CCR0 |= num_cycles(desired_blink_freq);

    	__enable_interrupt();
    	_BIS_SR(LPM1_bits + GIE);


	}

	#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
	__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
	{
    		PxOUT ^= BITx;   // Toggle P1.0 (LED)

	}

	#pragma vector=PORT1_VECTOR
	__interrupt void PORT_1(void)
	{
    		if (PushedButton == 0)          //Falling-edge of button
	{
    		TA1CTL = TASSEL_1 + MC_2;       //Selecting Timer A and Count Up indefinitely
    		TA1CCR0 = 0xFFFF;               //Initialize value of TA1CCR0
    		TA1CCTL0 = CAP;                 //Capture mode
    		PushedButton = 1;
    		TA0CCR0 = 1;                    //Reset CCR0

	}
	else if (PushedButton == 1)         //Rising-edge of a button
	{
    		TA1CTL = MC_0;                  //Stop Counting
    		TA0CCR0 = TA1R;                 //Assign new value for CCR0
    		if (TA0CCR0 > 100000)           //Set Fastest Rate
        		TA0CCR0 = 0xFFFF;
    		if (TA0CCR0 < 500)              //Set Slowest Rate
        		TA0CCR0 = 500;
    		TA1CTL = TACLR;                 //Clear Timer A1
    		PushedButton = 0;
	}

		PyIES ^= BITy;                      //toggle to set as rising edge
		PyIE |= BITy;                       // Enable interrupt
		PyIFG &= ~(BITy);

}


int num_cycles(int desired_blink_freq)
{
   int cycles;
   cycles = 32768/desired_blink_freq;
   return cycles;
}
```

## Pin Assignments

```c		
		   LED1		LED2	BUTTON
* MSP430G2553	=> P1.0 	P1.6	P1.3
* MSP430F5529	=> P1.0 	P4.7	P2.1
* MSP430FR2311	=> P1.0 	P2.0	P1.1
* MSP430FR5994	=> P1.0 	P1.1	P5.6
* MSP430FR6989	=> P1.0 	P9.7	P1.1
```

## Code Example

Control the rate of blinking using a button on the MSP430F5529 depending on the time the button is held down

```c
	#include <msp430.h>

	int num_cycles(int desired_blink_freq);
	int PushedButton;

	int main(void)
	{
    		WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    		//PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode
    		int desired_blink_freq = 10;

    		P1DIR |= BIT0; // Set LEDs to outputs
    		P1OUT &= ~BIT0;

    		P2REN |= BIT1; //setup button 1.1 using pull up resistor and enable
    		P2OUT |= BIT1;
    		P2IE |= BIT1;
    		P2IES |= BIT1;
    		P2IFG &= ~BIT1;

    		TA0CCTL0 |= CCIE;
    		TA0CTL |= TASSEL_1 + MC_1;
    		TA0CCTL0 &= 0x10;
    		TA0CCR0 |= num_cycles(desired_blink_freq);

    		__enable_interrupt();
    		_BIS_SR(LPM1_bits + GIE);


	}

	#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
	__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
	{
    		P1OUT ^= BIT0;   // Toggle P1.0 (LED)

	}

	#pragma vector=PORT2_VECTOR
	__interrupt void PORT_2(void)
	{
    		if (PushedButton == 0)           //Falling-edge of button
	{
    		TA1CTL = TASSEL_1 + MC_2;        //Selecting Timer A and Count Up indefinitely
    		TA1CCR0 = 0xFFFF;                //Initialize value of TA1CCR0
    		TA1CCTL0 = CAP;                  //Capture mode
    		PushedButton = 1;
    		TA0CCR0 = 1;                     //Reset CCR0

	}
		else if (PushedButton == 1)         //Rising-edge of a button
	{
    		TA1CTL = MC_0;                  //Stop Counting
    		TA0CCR0 = TA1R;                 //Assign new value for CCR0
    		if (TA0CCR0 > 100000)           //Set Fastest Rate
        		TA0CCR0 = 0xFFFF;
    		if (TA0CCR0 < 500)              //Set Slowest Rate
        		TA0CCR0 = 500;
    		TA1CTL = TACLR;                 //Clear Timer A1
    		PushedButton = 0;
	}

	P2IES ^= BIT1;                      //toggle to set as rising edge
	P2IE |= BIT1;                       // Enable interrupt
	P2IFG &= ~(BIT1);

	}


	int num_cycles(int desired_blink_freq)
	{
   		int cycles;
   		cycles = 32768/desired_blink_freq;
   		return cycles;
	}
```


