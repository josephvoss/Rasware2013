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

/*void blink(void) {
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
}

int irServoTurn(void) {
			SetMotor(irServo, -1.0f);
			delay?
			leftVolt=ADCRead(servoIR);
			SetMotor(irServo, 1.0f);
			delay?
			rightVolt=ADCRead(servoIR);
			if (leftVolt > rightVolt) {
				turnLeft=false;
			}
			else {
				turnLeft=true;
			}
			return turnLeft;
}
*/
// The 'main' function is the entry point of the program
int count=0;
int main(void) {
    // Initialization code can go here
    //CallEvery(blink, 0, 0.5);
		rightM = InitializeServoMotor(PIN_B5, false);
		leftM = InitializeServoMotor(PIN_B6, true);
		irServo = InitializeServoMotor(PIN_B7, true);
		frontIR = InitializeADC(PIN_A5);
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
					//turnLeft=irServoTurn();
					//how long does irServoTurn take to run? Might slow down
					//robo if called here. Can find a way to it call continuously
					//and not interfer with speed/other calls?
					//turn(turnLeft);
					//SetPin(PIN_F3, true);
				}
        else {
					//SetPin(PIN_F3, false);

				}
				count+=1;
	}
}

