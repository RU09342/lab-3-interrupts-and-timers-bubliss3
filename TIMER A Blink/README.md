# TIMER A Blink

Control the speed of two LEDs using a TIMER module on 5 different MSP430 microcontrollers:

* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
 
## Code Configuration

The following code can be used on all of the boards with a slight change in code for the FR2311. The only other change that is needed to be made is the pin assignments labeled as x's for LED1 and y's for LED2

```c
	#include <msp430.h>

	int num_cycles(int desired_blink_freq);  //Function for blink frequency

	int main(void)
	{
    		WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    		PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode


    		int desired_blink_freq = 5; //Change to desired blink frequency

   		PxDIR |= BITx; 
    		PyDIR |= BITy; // Set LEDs to outputs
    		PxOUT &= ~BITx;
    		PyOUT &= ~BITy;

								// USE THIS CODE FOR FR2311
    		TA0CCTL0 |= CCIE;				// TB0CCTL0 |= CCIE;
    		TA0CTL |= TBSSEL_1 + MC_1;			// TB0CTL |= TBSSEL_1 + MC_1;
    		TA0CCTL0 &= 0x10;				// TB0CCTL0 &= 0x10;
    		TA0CCR0 |= num_cycles(desired_blink_freq);	// TB0CCR0 |= num_cycles(desired_blink_freq);
    		TA1CCTL0 |= CCIE;				// TB1CCTL0 |= CCIE;
    		TA1CTL |= TBSSEL_1 + MC_1;			// TB1CTL |= TBSSEL_1 + MC_1;
    		TA1CCTL0 &= 0x10;				// TB1CCTL0 &= 0x10;
    		TA1CCR0 |= num_cycles(desired_blink_freq/2);	// TB1CCR0 |= num_cycles(desired_blink_freq/2);

    		__enable_interrupt();
    		_BIS_SR(LPM1_bits + GIE);


		}

		#pragma vector=TIMER0_B0_VECTOR  // Define interrupt vector, in this case Port 1
		__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
		{
    			PxOUT ^= BITx;   // Toggle P1.0 (LED)
		}
		#pragma vector=TIMER1_B0_VECTOR  // Define interrupt vector, in this case Port 1
		__interrupt void Timer1_A0(void) // Define interrupt function that runs whenever interrupt is detected
		{
    			PyOUT ^= BITy;
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

Use Timer A on the MSP430F5529 to blink 2 LEDs at different frequencies

```c
	#include <msp430.h>

	int num_cycles(int desired_blink_freq);

	int main(void)
	{
    		WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    		PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode


    		int desired_blink_freq = 5;

    		P1DIR |= BIT0;
    		P4DIR |= BIT7; // Set LEDs to outputs
    		P1OUT &= ~BIT0;
    		P4OUT &= ~BIT7;

		TA0CCTL0 |= CCIE;
    		TA0CTL |= TASSEL_1 + MC_1;
    		TA0CCTL0 &= 0x10;
    		TA0CCR0 |= num_cycles(desired_blink_freq);
    		TA1CCTL0 |= CCIE;
    		TA1CTL |= TASSEL_1 + MC_1;
    		TA1CCTL0 &= 0x10;
    		TA1CCR0 |= num_cycles(desired_blink_freq/2);

    		__enable_interrupt();
    		_BIS_SR(LPM1_bits + GIE);
		

		}

		#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
		__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
		{
    			P1OUT ^= BIT0;   // Toggle P1.0 (LED)
		}
		#pragma vector=TIMER1_A0_VECTOR  // Define interrupt vector, in this case Port 1
		__interrupt void Timer1_A0(void) // Define interrupt function that runs whenever interrupt is detected
		{
    			P4OUT ^= BIT7;
		}

		int num_cycles(int desired_blink_freq)
		{
   			int cycles;
   			cycles = 32768/desired_blink_freq;
   			return cycles;
		}
```


