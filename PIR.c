#include <xc.h>
#include "delays.h"
#include "lcd.h"

void pir() { 
    TRISAbits.TRISA0 = 1;
    ADCON1 = 0b00001111;   // PIR sensor input

    lcd_init();

    while(1) {
        lcd_write_cmd(0x01);
        if (PORTAbits.RA0 == 0) {
            // Motion detected

        } else {
            motordoor_close();
            motorlock_lock();
            delay_ms(1000);
        }

    }
}