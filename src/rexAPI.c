/*
 * rexAPI.c
 *
 *  Created on: Dec 31, 2015
 *      Author: Anton
 */

#include "rexAPI.h"

joy main;
joy partner;

void positionPIDControl(void *ignore)
{
	float error = 0;
	float integral = 0;
	float derivative = 0;
	float previousError = 0;
	unsigned long startTime = millis();
	unsigned long loopTime;
	float output = 0;
	pidParams params = *((pidParams*)ignore);

	if(params.timeOut>0)
	{
		while(millis() < startTime + params.timeOut)
		{
			loopTime = millis();
			error = params.target() - params.input();
			integral += error;
			derivative = error - previousError;

			if(integral > 50/params.kI)
				integral = 50/params.kI;

			if(error == 0)
				integral = 0;

			previousError = error;

			output = (error*params.kP) + (integral*params.kI) + (derivative*params.kD);
			if(output>127)
			{
				output = 127;
			}
			foreach(int *motor, params.outputs)
			{
				motorSet(abs(*motor), output*(*motor/abs(*motor)));
			}
			taskDelayUntil(&loopTime,MOTOR_REFRESH_TIME);
		}
	}
	else
	{
		while(true)
		{
			loopTime = millis();
			error = params.target() - params.input();
			integral += error;
			derivative = error - previousError;

			if(integral > 50/params.kI)
				integral = 50/params.kI;

			if(error == 0)
				integral = 0;

			previousError = error;

			output = (error*params.kP) + (integral*params.kI) + (derivative*params.kD);

			if(output>127)
			{
				output = 127;
			}

			foreach(int *motor, params.outputs)
			{
				motorSet(abs(*motor), output*(*motor/abs(*motor)));
			}
			taskDelayUntil(&loopTime,MOTOR_REFRESH_TIME);
		}
	}
}


void velocityPIDControl( void *ignore)
{
	float error = 0;
	float integral = 0;
	float derivative = 0;
	float previousError = 0;
	unsigned long startTime = millis();
	unsigned long loopTime;
	float output = 0;
	pidParams params = *((pidParams*)ignore);

	if(params.timeOut>0)
	{
		while(millis() < startTime + params.timeOut)
		{
			loopTime = millis();
			error = params.target() - params.input();
			integral += error;
			derivative = error - previousError;

			if(integral > 50/params.kI)
				integral = 50/params.kI;

			if(error == 0)
				integral = 0;

			previousError = error;

			output += (error*params.kP) + (integral*params.kI) + (derivative*params.kD);
			if(output>127)
			{
				output = 127;
			}
			foreach(int *motor, params.outputs)
			{
				motorSet(abs(*motor), output*(*motor/abs(*motor)));
			}
			printf("%f\n\r",velocity);
			taskDelayUntil(&loopTime,MOTOR_REFRESH_TIME);
		}
	}
	else
	{
		while(true)
		{
			loopTime = millis();
			error = params.target() - params.input();
			integral += error;
			derivative = error - previousError;

			if(integral > 50/params.kI)
				integral = 50/params.kI;

			if(error == 0)
				integral = 0;

			previousError = error;

			output += (error*params.kP) + (integral*params.kI) + (derivative*params.kD);

			if(output>127)
			{
				output = 127;
			}

			foreach(int *motor, params.outputs)
			{
				motorSet(abs(*motor), output*(*motor/abs(*motor)));
			}
			printf("%f\n\r",velocity);
			taskDelayUntil(&loopTime,MOTOR_REFRESH_TIME);
		}
	}
}

int* motorGroupInit(int group[])
{
	int position = 1;
	int number = 0;
	bool flag = true;

	lcdPrint(uart1,1,"Left Side Select");
	delay(2000);
	while(flag)
	{
		switch(position)
		{
		MOTOR_CASE(1)
						MOTOR_CASE(2)
						MOTOR_CASE(3)
						MOTOR_CASE(4)
						MOTOR_CASE(5)
						MOTOR_CASE(6)
						MOTOR_CASE(7)
						MOTOR_CASE(8)
						MOTOR_CASE(9)
						MOTOR_CASE(10)
		case 11:
			lcdPrint(uart1,1,"Exit?");
			break;
		}

		if(lcdReadButtons(uart1) == 1){
			position -= 1;
			delay(100);
		}
		else if(lcdReadButtons(uart1) == 2){
			if(position != 11)
				group[number] = position;
			else
				flag = false;
			number++;
			delay(100);
		}

		else if(lcdReadButtons(uart1) == 4){
			position +=1;
			delay(100);
		}

		if(position < 1)
			position = 1;

		else if(position > 11 )
			position = 11;

		if(number == 3)
			flag = false;
		delay(20);
	}
	position = 1;
	number = 0;
	flag = true;
	lcdClear(uart1);
	foreach(int *motor, group)
	{
		while(flag)
		{
			lcdPrint(uart1, 1, "reverse %d", *motor);
			lcdPrint(uart1, 2, "Yes           No");
			if(lcdReadButtons(uart1) == 1)
			{
				group[number] = 0-*motor;
				flag = false;
			}
			else if(lcdReadButtons(uart1) == 1)
			{
				flag = false;
			}
		}
		flag = true;
		number++;
	}
	return group;
}

tankDriveVars tankDriveInit()
{
	tankDriveVars tank;


	lcdPrint(uart1,1,"Left Side Select");
	delay(2000);
	motorGroupInit(tank.leftSide);
	lcdClear(uart1);
	lcdPrint(uart1,1,"Right Side");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(tank.rightSide);
	return tank;
}

mecanumDriveVars mecanumDriveInit()
{
	mecanumDriveVars mecanum;

	lcdPrint(uart1,1,"Left Front");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(mecanum.leftFront);

	lcdPrint(uart1,1,"Right Front");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(mecanum.rightFront);

	lcdPrint(uart1,1,"Left Back");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(mecanum.leftBack);

	lcdPrint(uart1,1,"Right Back");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(mecanum.rightBack);
	return mecanum;
}

xDriveVars xDriveInit()
{
	xDriveVars xDrive;

	lcdPrint(uart1,1,"Left Front");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(xDrive.leftFront);

	lcdPrint(uart1,1,"Right Front");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(xDrive.rightFront);

	lcdPrint(uart1,1,"Left Back");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(xDrive.leftBack);

	lcdPrint(uart1,1,"Right Back");
	lcdPrint(uart1,2,"Select");
	delay(2000);
	motorGroupInit(xDrive.rightBack);
	return xDrive;
}

drive driveInit()
{
	drive output;
	int type;

	int position=0;
	bool flag = true;
	while(flag)
	{
		switch(position)
		{
		case 0:
			lcdPrint(uart1, 1, "tank");
			lcdPrint(uart1, 2,"<-   select   ->");
			break;
		case 1:
			lcdPrint(uart1, 1, "mecanum");
			lcdPrint(uart1, 2,"<-   select   ->");
			break;
		case 2:
			lcdPrint(uart1, 1, "x-drive");
			lcdPrint(uart1, 2,"<-   select   ->");
			break;
		}

		if(lcdReadButtons(uart1) == 1){
			position -= 1;
			delay(100);
		}
		else if(lcdReadButtons(uart1) == 2){
			type = position;
			flag = false;
			delay(100);
		}

		else if(lcdReadButtons(uart1) == 4){
			position +=1;
			delay(100);
		}

		if(position < 0)
			position = 0;

		else if(position >= MAX_DRIVE_TYPES )
			position = MAX_DRIVE_TYPES - 1;
		delay(20);
	}
	output.type = position;
	tankDriveVars newT;
	mecanumDriveVars newM;
	xDriveVars newX;
	switch(output.type)
	{
	case TANK:
		newT = tankDriveInit();
		output.driveVars = &newT;
		break;
	case MECANUM:
		newM = mecanumDriveInit();
		output.driveVars = &newM ;
		break;
	case X_DRIVE:
		newX = xDriveInit();
		output.driveVars = &newX;
		break;
	}
	return output;
}

void getJoysticks(void *ignore)
{
	long unsigned int timeStart;
	while(1)
	{
		timeStart = millis();
		main.rightVertical = joystickGetAnalog(1,2);
		main.rightHorizontal = joystickGetAnalog(1,1);
		main.leftVertical = joystickGetAnalog(1,3);
		main.leftHorizontal = joystickGetAnalog(1,4);
		partner.rightVertical = joystickGetAnalog(2,2);
		partner.rightHorizontal = joystickGetAnalog(2,1);
		partner.leftVertical = joystickGetAnalog(2,3);
		partner.leftHorizontal = joystickGetAnalog(2,4);
		main.rightBumper.up = joystickGetDigital(1,6,JOY_UP);
		main.rightBumper.down = joystickGetDigital(1,6,JOY_DOWN);
		main.leftBumper.up = joystickGetDigital(1,5,JOY_UP);
		main.leftBumper.down = joystickGetDigital(1,5,JOY_DOWN);
		main.rightDpad.up = joystickGetDigital(1,8,JOY_UP);
		main.rightDpad.down = joystickGetDigital(1,8,JOY_DOWN);
		main.rightDpad.right = joystickGetDigital(1,8,JOY_RIGHT);
		main.rightDpad.left = joystickGetDigital(1,8,JOY_LEFT);
		main.leftDpad.up = joystickGetDigital(1,7,JOY_UP);
		main.leftDpad.down = joystickGetDigital(1,7,JOY_DOWN);
		main.leftDpad.right = joystickGetDigital(1,7,JOY_RIGHT);
		main.leftDpad.left = joystickGetDigital(1,7,JOY_LEFT);
		taskDelayUntil(&timeStart,20);
	}
}
