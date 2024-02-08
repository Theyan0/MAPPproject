#include <xc.h>
#include "delays.h"
#include "lcd.h"

void main() {
    TRISAbits.TRISA0 = 1;
    ADCON1 = 0b00001111; 
    while(1){
        ultra();
        LockMotor();
        return;
    }
}