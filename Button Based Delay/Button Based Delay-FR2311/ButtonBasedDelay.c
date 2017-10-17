#include <msp430.h>

int num_cycles(int desired_blink_freq);
int Push_Button;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    int desired_blink_freq = 10;

    P1DIR |= BIT0; // Set LEDs to outputs
    P1OUT &= ~BIT0;

    P1REN |= BIT1;                              //setup button 1.1 using pull up resistor
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;
    P1IFG &= ~BIT1;

    TB0CCTL0 |= CCIE;
    TB0CTL |= TBSSEL_1 + MC_1;
    TB0CCTL0 &= 0x10;
    TB0CCR0 |= num_cycles(desired_blink_freq);

    __enable_interrupt();
    _BIS_SR(LPM1_bits + GIE);


}

#pragma vector=TIMER0_B0_VECTOR  // Define interrupt vector
__interrupt void Timer0_B0(void) // Define interrupt function
{
    P1OUT ^= BIT0;   // Toggle P1.0 (LED)

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    if (Push_Button == 0)              //Falling-edge of button
{
    TB1CTL = TBSSEL_1 + MC_2;        //Selecting Timer A and Count Up indefinitely
    TB1CCR0 = 0xFFFF;               //Initialize value of TB1CCR0
    TB1CCTL0 = CAP;                 //Capture mode
    Push_Button = 1;
    TB0CCR0 = 1;                    //Reset CCR0

}
else if (Push_Button == 1)         //Rising-edge of a button
{
    TB1CTL = MC_0;                  //Stop Counting
    TB0CCR0 = TB1R;                 //Assign new value for CCR0
    if (TB0CCR0 > 100000)           //Set Fastest Rate
        TB0CCR0 = 0xFFFF;
    if (TB0CCR0 < 500)              //Set Slowest Rate
        TB0CCR0 = 500;
    TB1CTL = TBCLR;                 //Clear Timer B1
    Push_Button = 0;
}

P1IES ^= BIT1;                      //toggle to set as rising edge
P1IE |= BIT1;                       // Enable interrupt
P1IFG &= ~(BIT1);

}


int num_cycles(int desired_blink_freq)
{
   int cycles;
   cycles = 32768/desired_blink_freq;
   return cycles;
}
