#include <xc.h>
#include "delays.h"
#include "lcd.h"
unsigned char c;



void PIR() {
    ADCON1 = 0x0F;
    TRISAbits.TRISA0 = 1;  // PIR sensor input

    while(1) {
        lcd_write_cmd(0xC0);  // Move cursor to line 2 position 1
        if (PORTAbits.RA0 == 0) {
            delay_ms(1000);
        } else {
            motordoor_close();
            delay_ms(5000);
            motorlock_lock();
            delay_ms(500);
        }

    }
}