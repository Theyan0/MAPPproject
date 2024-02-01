#include <xc.h>
#include "delays.h"  // You should have a delay library with Delay_ms function
#include "lcd.h"     // Include your LCD functions

void UltraSound() {
    TRISC = 0b00000010;  // RD0 as trigger output, RD1 as echo input, RD2 optional for LED

    lcd_init();  // Initialize LCD, you should have this function

    while(1) {
        unsigned int Count = 0;

        // Trigger the ultrasonic sensor
        PORTCbits.RC0 = 1;
        delay_us(10);  // At least 10 us
        PORTCbits.RC0 = 0;
        delay_ms(1);  // Delay a while (assuming Delay_ms is in milliseconds)

        // Measure echo pulse
        while (PORTCbits.RC1 == 0); // Wait here if echo not back
        Count = 0; // Echo back, start timing

        while ((PORTCbits.RC1 == 1) && (Count < 200)) {
            delay_us(58);  // Delay 58 us
            Count++;
        } // Count increments if echo signal still high

        // Convert count to distance (in cm)
        unsigned int distance_cm = Count / 58;  // Divide by 58 to convert to cm

        // Check if distance is less than or equal to 5cm
        if (distance_cm <= 5) {
            // If someone is near (5cm or less), light up the LCD
            lcd_backlight_on();
        } else {

            lcd_backlight_off();
        }
    }
}