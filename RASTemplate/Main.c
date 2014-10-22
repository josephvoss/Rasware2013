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
tMotor* rightM;
tMotor* leftM;
tMotor* irServo;
tADC* irSensor;
int frontSensor;
const int minVoltageDis;
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
		rightM = InitializeServoMotor(PIN_B5, false);
		leftM = InitializeServoMotor(PIN_B0, true);
		irServo = InitializeServoMotor(PIN_B3, true);
		SetPin(PIN_B0,true);
		irSensor = InitializeADC(PIN_F2);
		ADCReadContinuously(irSensor, 0.25f);	
    while (1) {
        // Runtime code can go here
        Printf("Hello World!\n");
				SetMotor(leftM, .25f);
				SetMotor(rightM, .25f);
				frontSensor=ADCRead(irSensor);
				if (frontSensor >= minVoltageDis) {
					//begin IRsensor turn, not sure about distance
					SetMotor(irServo, .25f);
				}
				
        
    }

}
