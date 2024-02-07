#include <xc.h>
#include "delays.h"
#include "lcd.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>
#include "USART_Header_File.h"  // Assuming this controls UART
#include <stdbool.h>

#define PIR PORTEbits.RE0
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF


#define _XTAL_FREQ 48000000  // Ensure this matches your actual clock
unsigned char key, outchar;
unsigned char p1, p2, p3, p4;
char Message1[] = "Enter PIN number :  ";
int wrong = 0;


void UART_Init(void) {
    TRISCbits.TRISC6 = 0; // TX pin as output
    TRISCbits.TRISC7 = 1; // RX pin as input

    TXSTA = 0x24; // Transmit enabled, high speed
    RCSTA = 0x90; // Serial port enabled, continuous receive
    BAUDCON = 0x08; // BRG16 = 1, use 16-bit baud rate generator
    SPBRG = 77; // Baud rate setting for 9600 with 48MHz clock and 16-bit BRG
    SPBRGH = 0; // Not needed for this baud rate but included for completeness
    PIR1bits.TXIF = 0;
}

void UART_Write(char data) {
    while (!TRMT); // Wait for the transmit shift register to be empty
    TXREG = data; // Write data to transmit register
}



void LockMotor() {
    TRISAbits.TRISA0 = 1;
    ADCON1 = 0b00001111; 
    unsigned char key, p1, p2, p3, p4;

    // Initialize the LCD and UART
    lcd_init();
    UART_Init();

    // Enable backlight if needed
    lcd_backlight_on();
    while(1){
    // Clear the LCD screen
    lcd_write_cmd(0x01); // Clear display command

    // Display the prompt message
    lcd_write_cmd(0x80); // Set cursor to first line
    char Message1[] = "Enter PIN number :  ";
    for (int i = 0; Message1[i] != '\0'; i++) {
        lcd_write_data(Message1[i]);
    }

    // Read 4-digit PIN from keypad
    lcd_write_cmd(0xC0); // Move cursor to second line
    p1 = getkey(); lcd_write_data('*');
    p2 = getkey(); lcd_write_data('*');
    p3 = getkey(); lcd_write_data('*');
    p4 = getkey(); lcd_write_data('*');

    // Check if PIN is correct
    if (p1 == '4' && p2 == '5' && p3 == '5' && p4 == '0') {
        // Correct PIN
        lcd_write_cmd(0x01); // Clear display
        lcd_write_cmd(0x80); // Set cursor position
        char openMsg[] = "OPENING LOCK";
        for (int i = 0; openMsg[i] != '\0'; i++) {
            lcd_write_data(openMsg[i]);
        }
            delay_ms(1000);
            motorlock_unlock();
            motordoor_open();
            lcd_write_cmd(0x01);
            while(1){
                if (PORTAbits.RA0 == 0) {
                    // Motion detected

                } else {
                    motordoor_close();
                    motorlock_lock();
                    delay_ms(1000);
                    break;
                }
            }break;
            wrong=0;
            
            
    } else {
        // Wrong PIN
        wrong++;
        lcd_write_cmd(0x01); // Clear display
        lcd_write_cmd(0x80); // Set cursor position
        char errorMsg[] = "WRONG PIN";
        for (int i = 0; errorMsg[i] != '\0'; i++) {
            lcd_write_data(errorMsg[i]);
        }
        lcd_write_data(wrong);

        delay_ms(2000); // Display message for 2 seconds

       
    }
    
     if (wrong >= 3) {
            // Send '1' to Arduino after 3 wrong attempts
            UART_Write('1');
            // Consider adding a lockout mechanism or delay here
        }
    }}