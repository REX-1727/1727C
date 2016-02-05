/*
 * rexAPI.h
 *
 *  Created on: Dec 13, 2015
 *      Author: Anton
 */

#include"main.h"

#ifndef REXAPI_H_

#define REXAPI_H_



#define MOTOR_REFRESH_TIME 20


/*
 * foreach implementation for item in array.
 */
#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)\

/*
 * Shorthand for dereferencing void pointers
 */
#define dereference(type, pointer) *((type*)pointer)

/*
 *
 */
typedef struct pidParams
{
	void *ignore;
	int	(*input)();
	int	(*target)();
	int timeOut;
	float kP;
	float kI;
	float kD;
	int outputs[4];

}pidParams;

/*
 * Positional version of a PID loop, make sure that input and target values are sensor positions
 * not changes in sensor position.
 *
 * @param input Function which returns the current sensor feedback value.
 * @param target Function which returns the current target value.
 * @param timeOut Time, in milliseconds, for which the pid will be active.
 * @param outputs Array of output motors. If reversed, port number should be negative.
 * @param kP Proportional constant.
 * @param kI Integral constant.
 * @param kD Derivative constant.
 * @param ignore Should be Null.
 */
void positionPIDControl(void *ignore);

/*
 * Velocity version of a PID loop, make sure that input and target values are sensor velocities
 * not sensor positions.
 *
 * @param input Function which returns the current sensor feedback value .
 * @param target Function which returns the current target value.
 * @param timeOut Time, in milliseconds, for which the pid will be active.
 * @param outputs Array of output motors. If reversed, port number should be negative.
 * @param kP Proportional constant.
 * @param kI Integral constant.
 * @param kD Derivative constant.
 * @param ignore Should be Null.
 */
void velocityPIDControl( void *ignore);

/*
 * General drive structure;
 */
typedef struct drive
{
	int type;
	void *driveVars;
}drive;

/*
 * Type value for tank drive.
 */
#define TANK 	0

/*
 * Type value for mecanum drive.
 */
#define MECANUM	1

/*
 * Type value for x-drive.
 */
#define X_DRIVE	2

/*
 * Maximum drive types.
 */
#define MAX_DRIVE_TYPES 3

/*
 * Setup helper macro
 */
#define MOTOR_CASE(port)\
		case port:\
		lcdPrint(uart1, 1, #port);\
		lcdPrint(uart1, 2,"<-   select   ->");\
		break;

/*
 * Function that fills an integer array with motor ports
 * @param group Array to be filled with motors.
 */
int* motorGroupInit(int group[]);

/*
 * Stanadrd tank drive with up to 3 motors per side.
 */
typedef struct tankDrive
{

	int	leftSide[3],rightSide[3];


} tankDriveVars;

/*
 * Function that initializes a tank drive variable set.
 */
tankDriveVars tankDriveInit();

/*
 * Stanadrd mecanum drive with up to 3 motors per corner.
 */
typedef struct mecanumDrive
{
	int	leftFront[3],rightFront[3],leftBack[3],rightBack[3];
}mecanumDriveVars;

/*
 * Function that initializes a mecanum drive variable set.
 */
mecanumDriveVars mecanumDriveInit();

/*
 * Stanadrd x-drive with up to 3 motors per corner.
 */
typedef struct xDrive
{
	int	leftFront[3],rightFront[3],leftBack[3],rightBack[3];
}xDriveVars;

/*
 * Function that initializes a x-drive variable set.
 */
xDriveVars xDriveInit();

/*
 * Initializes the drive structure for use in current run or to be saved and used in future runs.
 */
drive driveInit();


/*
 * Global current drive type
 */
extern int driveType;

/*
 * Structure that encompases all button groups on the joystick.
 */
typedef struct buttonGroup
{
	bool up;
	bool down;
	bool right;
	bool left;
}btnGrp;

/*
 * Structure that contains all the values returned from a joystick.
 */
typedef struct joystick
{
	int rightVertical;
	int rightHorizontal;
	int leftVertical;
	int leftHorizontal;
	btnGrp rightBumper;
	btnGrp leftBumper;
	btnGrp rightDpad;
	btnGrp leftDpad;
} joy;

/*
 * Main joystick.
 */
extern joy main;

/*
 * Partner joystick.
 */
extern joy partner;

/*
 * Function that should be run in a task in order to get joystick values constantly.
 * @param ignore Should be null.
 */
void getJoysticks(void *ignore);



#endif /* REXAPI_H_ */
