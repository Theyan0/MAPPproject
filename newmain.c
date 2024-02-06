#include <xc.h>
#include "delays.h"
#include "keypad.h"
#include "lcd.h"


void main(void) {
    TRISC = 0b00000010;
    
    while(1){
        UltraSound();
        LockMotor();
        
    }
}
