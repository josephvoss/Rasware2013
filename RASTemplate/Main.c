#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/servo.h>
#include <stdbool.h>
#include <math.h>
#include <RASLib/inc/time.h>

///////////////////////////////////////////////////////////////////
///////									PETER/MAURYA!
///////					The code below is garbage
///////					It works but there are definetly much better methods
///////					Just a warning
///////         -Joseph
///////////////////////////////////////////////////////////////////

tBoolean blink_on = true;

tMotor* rightM;
tMotor* leftM;
tServo* irServo;
tADC* frontIR;
tADC* servoIR;
tLineSensor* lineSensor; 
int frontIRVolt;
int pfrontIRVolt;
int tfrontIRVolt;
int cfrontIRVolt;
int servoIRVolts[2];
double leftVolt;
double rightVolt;
int i;
int turnLeft;
int minVoltDis=15;
int minVoltDisTurn=30;
float irServoTurnCount = 0.0;
float irDeltaTurn=0.25f;
int endOfTurn;
int lineSensorArrayVolt;
float lineArray[8];

void blink(void) {
    SetPin(PIN_F1, blink_on);
    blink_on = !blink_on;
}

// The 'main' function is the entry point of the program
int count=0;

void turnleft(bool left) {
	if (left) {
		SetMotor(leftM, 1.0f);
		SetMotor(rightM, -1.0f);
		Printf("Turning left");
		Wait(.9);
	}
	else {
		SetMotor(leftM,-1.0f);
		SetMotor(rightM, 1.0f);
		Printf("Turning right");
		Wait(.9);
	}
}
	
void move(float left, float right) {
	SetMotor(leftM, left);
	SetMotor(rightM, right);
}

void stopMotors(void) {
	SetMotor(leftM, .0f);
	SetMotor(rightM, .0f);
}

bool isBlack(float x) {
	if(isinf(x) || x > 1) {
		return true;
	} else {
		return false;
	}
}

int* irServoMeasure(int* grossArrayThing) {
	//Moves the servo returns 2 measurements of IR
	grossArrayThing[0]=0;
	grossArrayThing[1]=0;
	SetServo(irServo, 1.0f);
	Wait(1);
	while (i<1000) {
		grossArrayThing[0] +=(int) (ADCRead(servoIR)*100);
		i += 1;
	}
	i=0;
	SetServo(irServo, 0.0f);
	Wait(1);
		while (i<1000) {
		grossArrayThing[1] +=(int) (ADCRead(servoIR)*100);
		i += 1;
	}
	i=0; 
	grossArrayThing[0]=grossArrayThing[0]/1000;
	grossArrayThing[1]=grossArrayThing[1]/1000;
	return grossArrayThing;
	}

void irServoMove(int* grossArrayThing) {
	//takes in 2 measurements of ir and turns robrot
	if (grossArrayThing[0] >= minVoltDisTurn) {
		//turn one way
		turnleft(false);
		Printf("right\n");
	}
	else if (grossArrayThing[1] >= minVoltDisTurn) {
		//turn other way bruh
		turnleft(true);
		Printf("left\n");
	}
}

int main(void) {
    // Initialization code can go here
	Printf("Not dead");
    CallEvery(blink, 0, .25);
	rightM = InitializeServoMotor(PIN_C5, true);
	leftM = InitializeServoMotor(PIN_B4, false);
	irServo = InitializeServo(PIN_B3);
	frontIR = InitializeADC(PIN_D0);
	servoIR = InitializeADC(PIN_D1);
	//ADCReadContinuously(frontIR, 0.0f);
	ADCReadContinuously(servoIR, 0.0f);
	
	lineSensor = InitializeGPIOLineSensor(PIN_A2, PIN_A3, PIN_A4, PIN_B6, PIN_B7, PIN_F0, PIN_E0, PIN_B2);
  LineSensorReadContinuously(lineSensor, 0.f);

	while (1) {
/*
		LineSensorReadArray(lineSensor, lineArray);
		
		if(isBlack(lineArray[6]) || isBlack(lineArray[7])) {
			move(.1f, -.1f);
		} else if(isBlack(lineArray[0]) || isBlack(lineArray[1])) {
			move(-.1f, .1f);
		} else if(isBlack(lineArray[3]) || isBlack(lineArray[4])) {
			move(.15f, .15f);
		}
*/		
		/*
		
		PETER WAT DIS DO
		
		float side = lineArray[7] - lineArray[0];
		if(lineArray[7] > .5f) {
		
			stopMotors();
		} else {		
			move(.2f, .2f);
		}
		*/
		
		
		
		/*
		Printf("%.2f\t",lineArray[0]);
		Printf("%.2f\t",lineArray[1]);
		Printf("%.2f\t",lineArray[2]);
		Printf("%.2f\t",lineArray[3]);
		Printf("%.2f\t",lineArray[4]);
		Printf("%.2f\t",lineArray[5]);
		Printf("%.2f\t",lineArray[6]);
		Printf("%.2f\n",lineArray[7]);
		*/
		
		//comment out motors to avoid un-desirable enhanced robotic mobility capabilities
		//SetMotor(leftM, .1f);
		//SetMotor(rightM, .1185f);
		
				
		move(.15f,.15f);
		
		frontIRVolt=(int) (ADCRead(frontIR)*100);
		//Printf("%d\n", frontIRVolt);
		
		tfrontIRVolt += frontIRVolt;
		cfrontIRVolt += 1;
		
		if (cfrontIRVolt==10000) {
			cfrontIRVolt=0;
			tfrontIRVolt=0;
		}
		
		//Measures front IR, if within wall distance, do stuff
		if (tfrontIRVolt/cfrontIRVolt >= minVoltDis) {
				if (frontIRVolt==pfrontIRVolt) {
			SetPin(PIN_F2, true);
			cfrontIRVolt=0;
			tfrontIRVolt=0;
			irServoMeasure(servoIRVolts);
			irServoMove(servoIRVolts);
		}
			else {
				SetPin(PIN_F2, false);
			}
		}
		else {
		SetPin(PIN_F2, false);
	}
	
	pfrontIRVolt=frontIRVolt;
	
}
}
