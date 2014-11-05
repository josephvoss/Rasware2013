#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/servo.h>
#include <math.h>
// Blink the LED to show we're on

tBoolean blink_on = true;
tBoolean fuck;

tMotor* rightM;
tMotor* leftM;
tServo* irServo;
tADC* frontIR;
tADC* servoIR;
tLineSensor* lineSensor; 
int frontIRVolt;
double leftVolt;
double rightVolt;
int i;
int turnLeft;
int minVoltageDis=17;
float irServoTurnCount = 0.0;
float irDeltaTurn=0.25f;
int endOfTurn;
int lineSensorArrayVolt;
float lineArray[8];

void blink(void) {
    SetPin(PIN_F1, blink_on);
    blink_on = !blink_on;
}

void turn(turnL) {
	if (turnL) {
		SetMotor(leftM, -1.0f);
		SetMotor(rightM, 1.0f);
	}
	else {
		SetMotor(leftM, 1.0f);
		SetMotor(rightM, -1.0f);
	}
}

int irServoTurn(void) {
/*	SetMotor(irServo, irDeltaTurn);
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

	////////////////////////////////// -Peter- ////////////////////////////////////
	// Shouldn't irServo be a tServo instead of a tMotor?						 //
	// SetMotor specifies a speed, not position									 //
	// We should probably use SetServo(irServo, 0.0f) and SetServo(irServo, 1.0f)//
	// https://github.com/ut-ras/Rasware2013/wiki/servo.h 						 //
	///////////////////////////////////////////////////////////////////////////////

	SetServo(irServo, 1.0f);
	leftVolt=(int) (ADCRead(servoIR)*100);
	SetServo(irServo, 0.0f);
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


//First attempt at line following code. Feel free to change/alter/take apart/mutilate -J
//NOT TESTED WITH BATTERY, or at all really
void runLineFollower(void) {
	LineSensorReadArray(lineSensor, lineArray); 
	if (lineArray[0]>1.0) {
		//turn hard left
		SetMotor(leftM, .1f);
		SetMotor(rightM, .15f);
	}
	else if(lineArray[1]>1.0) {
		//medium left
		SetMotor(leftM, .1f);
		SetMotor(rightM, .134f);
	}
	else if(lineArray[2]>1.0) {
		//soft left
		SetMotor(leftM, .1f);
		SetMotor(rightM, .117f);
	}
	else if(lineArray[3]>1.0) {
		//straight
		SetMotor(leftM, .1f);
		SetMotor(rightM, .1f);
	}
	else if(lineArray[4]>1.0) {
		//straight
		SetMotor(leftM, .1f);
		SetMotor(rightM, .1f);
	}
	else if(lineArray[5]>1.0) {
		//soft right
		SetMotor(leftM, .117f);
		SetMotor(rightM, .1f);
	}
	else if(lineArray[6]>1.0) {
		//medium right
		SetMotor(leftM, .134f);
		SetMotor(rightM, .1f);
	}
	else if(lineArray[7]>1.0) {
		//hard right
		SetMotor(leftM, .15f);
		SetMotor(rightM, .1f);
	}
}

int main(void) {
    // Initialization code can go here
	Printf("Not dead");
    CallEvery(blink, 0, .25);
	rightM = InitializeServoMotor(PIN_C7, true);
	leftM = InitializeServoMotor(PIN_B5, false);
	irServo = InitializeServo(PIN_B3);
	frontIR = InitializeADC(PIN_D0);
	servoIR = InitializeADC(PIN_D1);
	ADCReadContinuously(frontIR, 0.25f);
	lineSensor = InitializeGPIOLineSensor(PIN_A2, PIN_A3, PIN_A4, PIN_B6, PIN_B7, PIN_F0, PIN_E0, PIN_B2);
  LineSensorReadContinuously(lineSensor, 0.f);
	while (1) {
        // Runtime code can go here
    //Printf("Hello World!\n");
	  Printf("%.2f\t",lineArray[0]);
		Printf("%.2f\t",lineArray[1]);
		Printf("%.2f\t",lineArray[2]);
		Printf("%.2f\t",lineArray[3]);
		Printf("%.2f\t",lineArray[4]);
		Printf("%.2f\t",lineArray[5]);
		Printf("%.2f\t",lineArray[6]);
		Printf("%.2f\n",lineArray[7]);

		//go line follower function
		//runLineFollower();
		//THERE CAN ONLY BE ONE...comment
		//(if un-comment runLineFollower, comment out this to avoid reading twice
		LineSensorReadArray(lineSensor, lineArray); 
		
		//comment out motors to avoid un-desirable enhanced robotic mobility capabilities
		//SetMotor(leftM, .1f);
		//SetMotor(rightM, .1185f);
		
		
		//Gross icky line follower stuff
		
		
		//frontIRVolt=(int) (ADCRead(frontIR)*100);
		//Printf("%02d\n", frontIRVolt);
		
		////////////////////////////////////////////////////////////	-Peter-	   ////////////////////////////////////////////////////
		// If I am thinking about this correctly, won't this take forever?															 //
		// The IR sensor will look left and right, then the robot will move a little, then the IR sensor will look around again, etc.//
		// Until there's no longer a wall in front 																					 //
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		/* comment out front IR test while working on line sensor
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
		}*/

		SetPin(PIN_F2, false);
	}
}

