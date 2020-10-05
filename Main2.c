#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void setup(void);

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    setup();
    while(1){

        }


    return 0;
}



void setup(void){
    P4SEL = BIT4 +BIT5; //select peripheral function for P4.4(Tx) and P4.5(Rx)
    UCA1CTL1 |= UCSWRST; //reset USCI for configuration
    UCA1CTL1 |= UCSSEL_2; //select clock source (SMCLK)
    UCA1BR0 = 109; //set baud-rate prescaler
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_2; //set baud-rate modulator
    UCA1CTL1 &= ~UCSWRST; //enable USCI
    UCA1IE |= UCTXIE;
    UCA1IE |= UCRXIE;

    P1DIR |= BIT0;                            // Set P1.0 to output
   P1OUT &= ~BIT0;
   P4DIR |= BIT7;                            // Set P4.7 to output  (green)
   P4OUT &= ~BIT7;
    __enable_interrupt();
}

#pragma vector=USCI_A1_VECTOR

__interrupt void USCI_A1_ISR(void)


{

if(UCA1RXBUF == 65) //Toggle red LED if "A" RXed
{
    P1OUT &= ~ BIT0;
    P4OUT |= BIT7;
}

if(UCA1RXBUF == 66) //Toggle green LED if "B" RXed
{
    P4OUT &= ~ BIT7;
    P1OUT |= BIT0;
}

}