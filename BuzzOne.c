#include <xc.h>
#include "delays.h"

void buzzer_but() {
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 0; //-- Set RC0 as output
    //-- Set all pins on PortD as output
    while(PORTCbits.RC1 == 0){
        PORTCbits.RC2 = 1; //sound ON then OF
        delay_ms(1000);
    }
}





