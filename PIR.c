#include <xc.h>
#include "delays.h"
#include "lcd.h"
unsigned char c;

// Function to initialize PWM
void initializePWM() {
    TRISC = 0x00; // PortC RC2 connects to the motor
}


void PIR() {
    ADCON1 = 0x0F;
    TRISA0 = 1;  // PIR sensor input
    initializePWM();
    lcd_init();

    while(1) {
        // Check PIR sensor
        lcd_write_cmd(0xC0);  // Move cursor to line 2 position 1
        if (PORTAbits.RA0 == 0) {
            // Motion detected
            delay_ms(500);
      
        } else {
            // No motion
            motorlock_right();
            delay_ms(500);
        }

    }
}