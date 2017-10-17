#include <msp430.h>
int num_cycles(int desired_blink_freq);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode


    int desired_blink_freq = 5;

    P1DIR |= BIT0;
    P2DIR |= BIT0; // Set LEDs to outputs
    P1OUT &= ~BIT0;
    P2OUT &= ~BIT0;


    TB0CCTL0 |= CCIE;
    TB0CTL |= TBSSEL_1 + MC_1;
    TB0CCTL0 &= 0x10;
    TB0CCR0 |= num_cycles(desired_blink_freq);
    TB1CCTL0 |= CCIE;
    TB1CTL |= TBSSEL_1 + MC_1;
    TB1CCTL0 &= 0x10;
    TB1CCR0 |= num_cycles(desired_blink_freq/2);

    __enable_interrupt();
    _BIS_SR(LPM1_bits + GIE);


}

#pragma vector=TIMER0_B0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= BIT0;   // Toggle P1.0 (LED)
}
#pragma vector=TIMER1_B0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Timer1_A0(void) // Define interrupt function that runs whenever interrupt is detected
{
    P2OUT ^= BIT0;
}

int num_cycles(int desired_blink_freq)
{
   int cycles;
   cycles = 32768/desired_blink_freq;
   return cycles;
}

