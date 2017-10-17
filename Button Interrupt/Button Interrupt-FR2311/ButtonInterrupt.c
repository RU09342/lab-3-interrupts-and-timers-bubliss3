#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode
    P1DIR = BIT0;
    P1REN = BIT1;
    P1OUT = BIT1;
    P1IE = BIT1;
    __enable_interrupt();
    _BIS_SR(LPM4_bits + GIE); //enter LPM4 mode and enable global interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(500000);
    P1OUT ^= BIT0;
    P1IFG = ~BIT1;
}
