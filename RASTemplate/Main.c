#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>

// Blink the LED to show we're on
tBoolean blink_on = true;

tMotor* rightM;
tMotor* leftM;
tMotor* irServo;
tADC* frontIR;
tADC* servoIR;
int frontIRVolt;
int leftVolt;
int rightVolt;
int turnLeft;
const int minVoltageDis=2.00;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

void turn(turnL) {
	if (turnL) {
		SetMotor(leftM, 1.0f);
		SetMotor(rightM, -1.0f);
	}
	else {
		SetMotor(leftM, -1.0f);
		SetMotor(rightM, -1.0f);
}


// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
		rightM = InitializeServoMotor(PIN_B5, false);
		leftM = InitializeServoMotor(PIN_B0, true);
		irServo = InitializeServoMotor(PIN_B3, true);
		//SetPin(PIN_B0,true);
		frontIR = InitializeADC(PIN_F2);
		ADCReadContinuously(frontIR, 0.25f);	
    while (1) {
        // Runtime code can go here
        Printf("Hello World!\n");
				SetMotor(leftM, .25f);
				SetMotor(rightM, .25f);
				frontIRVolt=ADCRead(frontIR);
				if (frontIRVolt >= minVoltageDis) {
					//begin IRsensor turn, not sure about distance
					//WAIT
					SetMotor(irServo, -1.0f);
					leftVolt=ADCRead(servoIR);
					SetMotor(irServo, 1.0f);
					rightVolt=ADCRead(servoIR);
					SetPin(PIN_F3, true);
					if (leftVolt > rightVolt) {
						turnLeft=false;
					}
					else {
						turnLeft=true;
					}
					turn(turnLeft);
				}
				else {
					SetPin(PIN_F3, false);
				}
        
    }

}
