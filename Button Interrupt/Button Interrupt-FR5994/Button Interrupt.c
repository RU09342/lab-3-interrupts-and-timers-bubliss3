#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode

    P1DIR = BIT0;
    P5REN = BIT6;
    P5OUT = BIT6;
    P5IE = BIT6;
    __enable_interrupt();
    _BIS_SR(LPM4_bits + GIE);
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    __delay_cycles(500000);
    P1OUT ^= BIT0;
    P5IFG = ~BIT6;
}

