#include <xc.h>
#include "delays.h"
#include "lcd.h"
#include "keypad.h"

void buzzer() {
    
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA3 = 0;
    ADCON1 = 0b00001111;

    PORTAbits.RA4 = 0;
    
    while(1){
        PORTAbits.RA3 = 0;
        while (PORTAbits.RA4 == 1 ) {
            PORTAbits.RA3 = 1; 
            delay_ms(100);
        }
    }
    
}
