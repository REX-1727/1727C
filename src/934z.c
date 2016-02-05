/*
 * 934z.c
 *
 *  Created on: Dec 31, 2015
 *      Author: Anton
 */


#include "main.h"
#include "rexAPI.h"



float velocity = 0;
float velocityTPS = 0;
float power = 0;
float powerTPS = 0;

void velocityReader(void *ignore)
{
	encoderReset(flywheelEncoder);
	unsigned long startTime = 0;
	while(true)
	{
		startTime = millis();
		velocityTPS = encoderGet(flywheelEncoder)*1000/20;
		velocity = velocityTPS*WHEEL_CIRCUMFERENCE/360.0;
		encoderReset(flywheelEncoder);
		taskDelayUntil(&startTime, MOTOR_REFRESH_TIME);
	}
}

int getVelocity()
{
	return (int)velocityTPS;
}


void powerListener(void *ignore)
{
	while(true)
	{
		if(main.rightDpad.up)
		{
			power++;
			taskDelay(100);
		}
		else if(main.rightDpad.down)
		{
			power--;
			taskDelay(100);
		}
		else if(main.rightDpad.right)
		{
			power += .5;
			taskDelay(100);
		}
		else if(main.rightDpad.left)
		{
			power -= .5;
			taskDelay(100);
		}
		else if(main.rightBumper.up)
		{
			power =45;
			taskDelay(100);
		}
		else if(main.rightBumper.down)
		{
			power =0;
			taskDelay(100);
		}
		if(power<0)
		{
			power =0;
			taskDelay(100);
		}

		powerTPS = power*360/(WHEEL_CIRCUMFERENCE);

		taskDelay(20);
	}
}

int getPower()
{
	return (int) powerTPS;
}


void driveControl(void *ignore)
{
	int rightBack;
	int leftBack;
	int rightFront;
	int leftFront;
	while(true)
	{
		rightBack = main.leftVertical - main.leftHorizontal + main.rightHorizontal;
		leftBack = -main.leftVertical - main.leftHorizontal + main.rightHorizontal;
		rightFront = -main.leftVertical - main.leftHorizontal - main.rightHorizontal;
		leftFront = -main.leftVertical + main.leftHorizontal + main.rightHorizontal;
		motorSet(RB, -rightBack);
		motorSet(LB, leftBack);
		motorSet(RF, -rightFront);
		motorSet(LF, leftFront);

		if(main.leftBumper.up)
		{
			motorSet(INTAKE, 127);
		}
		else if(main.leftBumper.down)
		{
			motorSet(INTAKE, -127);
		}
		else
		{
			motorSet(INTAKE, 0);
		}


	}
}



void taskHandler(void *ignore)
{
	while(true)
	{
		switch(CHECK_STATE)
		{
		case(OFFLINE):
										break;
		case(DISABLED):
										break;
		case(DRIVER):
										break;
		case(AUTONOMOUS):
										break;
		}

	}
}


