#include <xc.h>
#include "delays.h"
#include "lcd.h"
#include "keypad.h"

unsigned char key, outchar;
unsigned char p1, p2, p3, p4;
char Message1[] = "Enter PIN number :  ";

void clearScreen() {
    int i;
    for (i = 0; i < 20; i++) {
        lcd_write_cmd(0x80 + i);
        lcd_write_data(' ');
        lcd_write_cmd(0xC0 + i);
        lcd_write_data(' ');
    }
}



void LockMotor() {
    int i;
    lcd_init();
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0; // Optional: Set other pins as needed for motor control


        lcd_write_cmd(0x80);
        for (i = 0; i < 20; i++) {
            outchar = Message1[i];
            lcd_write_data(outchar);
        }

        lcd_write_cmd(0xC0);

        key = getkey();
        p1 = key;
        lcd_write_data('*');

        key = getkey();
        p2 = key;
        lcd_write_data('*');

        key = getkey();
        p3 = key;
        lcd_write_data('*');

        key = getkey();
        p4 = key;
        lcd_write_data('*');
        
        lcd_write_cmd(0x01);

        if (p1 == '4' && p2 == '5' && p3 == '5' && p4 == '0') {
            lcd_write_data(0x80);
            lcd_write_data('O');
            lcd_write_data('P');
            lcd_write_data('E');
            lcd_write_data('N');
            lcd_write_data(0x20);
            
            delay_ms(1000);
            motorlock_unlock();
            delay_ms(5000);
            motordoor_open();
            delay_ms(1000);
            lcd_write_cmd(0x01);
        } else {
            lcd_write_data(0x80);
            lcd_write_data('W');
            lcd_write_data('R');
            lcd_write_data('O');
            lcd_write_data('N');
            lcd_write_data('G');
            
            delay_ms(2000);
            lcd_write_cmd(0x01);
        }
}
