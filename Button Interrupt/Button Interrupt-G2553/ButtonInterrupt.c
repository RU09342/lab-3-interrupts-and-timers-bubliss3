#include <msp430g2553.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = BIT6;
    P1REN = BIT3;
    P1OUT = BIT3;
    P1IE = BIT3;
    __enable_interrupt();
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(500000);
    P1OUT ^= BIT6;
    P1IFG = ~BIT3;
}
