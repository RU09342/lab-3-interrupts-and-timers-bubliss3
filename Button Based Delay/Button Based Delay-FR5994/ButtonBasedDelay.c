#include <msp430.h>

    int PushedButton;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode

    int desired_blink_freq = 10;
    P1DIR |= BIT0; // Set LEDs to outputs
    P1OUT &= ~BIT0;

    P5REN |= BIT5;                              //setup button 5.5 using pull up resistor and enable
    P5OUT |= BIT5;
    P5IE |= BIT5;
    P5IES |= BIT5;
    P5IFG &= ~BIT5;

    TA0CCTL0 |= CCIE;
    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCTL0 &= 0x10;
    TA0CCR0 |= num_cycles(desired_blink_freq);

    __enable_interrupt();


}

#pragma vector=TIMER0_A0_VECTOR  // Define interrupt vector, in this case Port 1
__interrupt void Timer0_A0(void) // Define interrupt function that runs whenever interrupt is detected
{
    P1OUT ^= BIT0;   // Toggle P1.0 (LED)

}

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{

    if (PushedButton == 0)              //Falling-edge of button
{
    TA1CTL = TASSEL_1 + MC_2;        //Selecting Timer A and Count Up indefinitely
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

P5IES ^= BIT5;                      //toggle to set as rising edge
P5IE |= BIT5;                       // Enable interrupt
P5IFG &= ~(BIT5);

}


int num_cycles(int desired_blink_freq)
{
   int cycles;
   cycles = 32768/desired_blink_freq;
   return cycles;
}
