#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>

// Blink the LED to show we're on
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}


// The 'main' function is the entry point of the program
tMotor* test;
tMotor* fuckThisShitIAmSoDone;
tADC* irSensor;
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
		test = InitializeServoMotor(PIN_B5, false);
		fuckThisShitIAmSoDone = InitializeServoMotor(PIN_B0, true);
    while (1) {
        // Runtime code can go here
        Printf("Hello World!\n");
				SetMotor(test, .25f);
				SetMotor(fuckThisShitIAmSoDone, .25f);
				
        
    }
		irSensor = InitializeADC(PIN_F2);
		ADCReadContinuously(irSensor, 0.1f);
}
