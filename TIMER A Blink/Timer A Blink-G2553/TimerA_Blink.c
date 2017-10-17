#include <msp430.h>
int num_cycles(int desired_blink_freq);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    int desired_blink_freq = 100;

    P1DIR |= BIT0;
    P1DIR |= BIT6; // Set LEDs to outputs
    P1OUT &= ~BIT0;
    P1OUT &= ~BIT6;


    TA0CCTL0 |= CCIE;
    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCTL0 &= 0x10;
    TA0CCR0 |= num_cycles(desired_blink_freq);
    TA1CCTL0 |= CCIE;
    TA1CTL |= TASSEL_1 + MC_1;
    TA1CCTL0 &= 0x10;
    TA1CCR0 |= num_cycles(desired_blink_freq/2);

    __enable_interrupt();

}

#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= BIT0;   // Toggle P1.0 (LED)
}
#pragma vector=TIMER1_A0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Timer1_A0(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= BIT6;
}

int num_cycles(int desired_blink_freq)
{
   int cycles;
   cycles = 32768/desired_blink_freq;
   return cycles;
}
