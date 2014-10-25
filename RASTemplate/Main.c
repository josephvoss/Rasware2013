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
double leftVolt;
double rightVolt;
int turnLeft;
int minVoltageDis=17;
float irServoTurnCount = 0.0;
float irDeltaTurn=0.25f;
int endOfTurn;
void blink(void) {
    SetPin(PIN_F1, blink_on);
    blink_on = !blink_on;
}

void turn(turnL) {
	if (turnL) {
		SetMotor(leftM, -1.0f);
		SetMotor(rightM, 0.0f);
	}
	else {
		SetMotor(leftM, 1.0f);
		SetMotor(rightM, -1.0f);
	}
}

int irServoTurn(void) {
/*			SetMotor(irServo, irDeltaTurn);
			irServoTurnCount += irDeltaTurn;
			if (irServoTurnCount == 1.0) {
			leftVolt=ADCRead(servoIR);
			//switches delta to negative to have sensor turn other way
			irDeltaTurn = -1*irDeltaTurn;
			endOfTurn=true;
			}
			if (irServoTurnCount == 0.0) {
			rightVolt=ADCRead(servoIR);
			irDeltaTurn = -1*irDeltaTurn;
			endOfTurn=true;
			}
			if (leftVolt > rightVolt && endOfTurn) {
				turnLeft=false;
			}
			if (endOfTurn) {
				//edit for end of turn
				turnLeft=true;
			}
			return turnLeft; */
			SetMotor(irServo, 1.0f);
			leftVolt=(int) (ADCRead(servoIR)*100);
			SetMotor(irServo, 0.0f);
			rightVolt=(int) (ADCRead(servoIR)*100);
			if (leftVolt<rightVolt) {
				turnLeft=true;
			}
			else {
				turnLeft=false;
			}
			return turnLeft;
}

// The 'main' function is the entry point of the program
int count=0;

int main(void) {
    // Initialization code can go here
		Printf("Not dead");
    CallEvery(blink, 0, .25);
		rightM = InitializeServoMotor(PIN_B5, false);
		leftM = InitializeServoMotor(PIN_B6, true);
		irServo = InitializeServoMotor(PIN_B3, true);
		frontIR = InitializeADC(PIN_D0);
		servoIR = InitializeADC(PIN_D1);
		ADCReadContinuously(frontIR, 0.25f);	
    while (1) {
        // Runtime code can go here
        //Printf("Hello World!\n");
				SetMotor(leftM, .25f);
				SetMotor(rightM, .25f);
				frontIRVolt=(int) (ADCRead(frontIR)*100);
				Printf("%02d\n", frontIRVolt);
				while (frontIRVolt >= minVoltageDis) {
					//begin IRsensor turn, not sure about distance
					//WAIT
					turnLeft=irServoTurn();
					//how long does irServoTurn take to run? Might slow down
					//robo if called here. Can find a way to it call continuously
					//and not interfer with speed/other calls?
					turn(turnLeft);
					SetPin(PIN_F2, true);
					frontIRVolt=(int) (ADCRead(frontIR)*100);
				}
				SetPin(PIN_F2, false);
	}
}

