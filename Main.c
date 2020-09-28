#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void setup(void);

unsigned int t_read = 1878;
float temperatura, temp;
unsigned char flag = 1;
unsigned int f=0, i=0;
char c[50]; //size of the number

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    setup();
    while(1){
        if(flag) {
            ADC12CTL0 &= ~ADC12SC;                  //produci fronte di salita bit SC (Start of Conversion)
            ADC12CTL0 |= ADC12SC;                   //per avviare la conversione
            flag = 0;                               //azzera flag per aspettare risultato prima di avviare nuova conversione
        }

        //calcola temperatura sulla base del valore rilevato dall'ADC (t_read)
        temperatura = (float)(((long)t_read - CALADC12_15V_30C) * (85 - 30)) / (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0;
        f=1;
        sprintf(c ,"%d.%d\n", (int)temperatura, (int)(((float)temperatura-(int)temperatura)*100));
    }
    return 0;
}

//interrupt service routine ADC
//viene invocata al termine della conversione, quando l'ADC scrive
//il risutato nella prima posizione del buffer di memoria (ADC12MEM0)
#pragma vector = ADC12_VECTOR;
__interrupt void ADC12_ISR(void) {
    t_read = ADC12MEM0;     //leggi risultato conversione
    flag = 1;               //attiva flag per autorizzare avvio nuova conversione
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

    P2DIR=0x00;
    P2REN=0xFF;
    P2OUT=0xFF;
    P2IES &= ~BIT1;
    P2IE |= BIT1;
    P2IFG &= ~BIT1;                                 //attivazione dell'interrupt

    REFCTL0 &= ~REFMSTR;                            //disabilita riferimento di tensione da blocco REF
    ADC12CTL0 = ADC12SHT0_8 + ADC12REFON + ADC12ON; //imposta tempo di sampling, abilita riferimento di tensione interno
    ADC12CTL1 = ADC12SHP;                           //campionamento basato su timer ADC12
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;        //Vref-=0V, Vref+=1.5V, selezione canale analogico 10 (integrated Temp. sensor)
    ADC12IE = 0x0001;                               //abilita interrupt corrispondente a ADC12IFG0
    __delay_cycles(100);                            //ritardo per assestamento tensioni di riferimento
    ADC12CTL0 |= ADC12ENC;                          //abilitazione blocco ADC
    __enable_interrupt();                           //GIE = 1
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void){
    if(f==1){
        __delay_cycles(5000);
        while(!(UCA1IFG & UCTXIFG));
        if(c[i]!='\0'){
            UCA1TXBUF=c[i];
            i++;
        }
        else{
            f=0;
            i=0;
        }
    }
    UCA1TXBUF = UCA1RXBUF;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void) //S1
{

    P2IFG &= ~BIT1;
}
