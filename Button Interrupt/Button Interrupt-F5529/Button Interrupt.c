#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = BIT0;
    P2REN = BIT1;
    P2OUT = BIT1;
    P2IE = BIT1;
    __enable_interrupt();
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    __delay_cycles(500000);
    P1OUT ^= BIT0;
    P2IFG = ~BIT1;
}
