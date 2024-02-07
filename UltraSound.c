#include <xc.h>
#include "delays.h"  // You should have a delay library with Delay_ms function
#include "lcd.h"     // Include your LCD functions

void ultra() {
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA3 = 0;
    ADCON1 = 0b00001111;  // RD0 as trigger output, RD1 as echo input, RD2 optional for LED
    
    PORTAbits.RA4 = 0;

    lcd_init();  // Initialize LCD, you should have this function

    while(1) {
        unsigned int Count = 0;

        // Trigger the ultrasonic sensor
        PORTAbits.RA2 = 1;
        delay_us(10);  // At least 10 us
        PORTAbits.RA2 = 0;
        delay_ms(1);  // Delay a while (assuming Delay_ms is in milliseconds)

        // Measure echo pulse
        while (PORTAbits.RA1 == 0); // Wait here if echo not back
        Count = 0; // Echo back, start timing

        while ((PORTAbits.RA1 == 1) && (Count < 200)) {
            delay_us(58);  // Delay 58 us
            Count++;
        } // Count increments if echo signal still high
        
        PORTAbits.RA3 = 0;
        while (PORTAbits.RA4 == 1 ) {
            PORTAbits.RA3 = 1; 
            delay_ms(100);
        }
        
        if (Count <= 5) {
                // If someone is near (5cm or less), light up the LCD
                lcd_backlight_on();
                break;
            } else{
                lcd_backlight_off();
            }
            delay_ms(100);
        }
    }
