#include "Chassis.h"

Chassis chassis;

/**
 * Chassis class
 */

static TaskHandle_t complexHandlerTask;

void onMotorTimer(void* param)
{
	ESP_LOGI(TAG, "Starting the PID loop thread");
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
    const TickType_t xInterval = 1; 	// sets up 1ms timer
	while(true)
	{
		vTaskDelayUntil(&xLastWakeTime, xInterval);
		chassis.motorHandler();
	}
	ESP_LOGE(TAG, "ERROR Pid thread died!");
}
/**
 * @param PWMgenerationTimer the timer to be used to generate the 20khx PWM
 * @param controllerTimer a timer for PID, velocity measurment, and trajectory planning
 */
void Chassis::allocateTimer(int PWMgenerationTimer)
{
	if (!timerAllocated)
	{
		xTaskCreatePinnedToCore(onMotorTimer, "PID loop Thread", 8192, NULL, 1,
								&complexHandlerTask, 0);
	}

	timerAllocated = true;
}

void Chassis::motorHandler(void)
{
	leftMotor.process();
    rightMotor.process();

    if(!timerAllocated) allocateTimer(0);

    //here's where you'll update the pose in Lab 2
	//updatePose(leftMotor.getSpeed(), ...);
}

Chassis::Chassis(void) :
    leftMotor(MOTOR_LEFT_PWM, MOTOR_LEFT_DIR, MOTOR_LEFT_ENCA, MOTOR_LEFT_ENCB),
    rightMotor(MOTOR_RIGHT_PWM, MOTOR_RIGHT_DIR, MOTOR_RIGHT_ENCA, MOTOR_RIGHT_ENCB)
{
    //TODO: Set gains for speed control
	//Either click into the MotorEncoded class and change the initializer in the 
	//constructor, or manually set the gains with the setter methods
}

void Chassis::init(void)
{
	allocateTimer(0); // used by the DC Motors
	leftMotor.attach();
    rightMotor.attach();
	motorHandler();
}

void Chassis::loop(void)
{
}

void Chassis::setWheelSpeeds(float left, float right) 
{
    leftMotor.setTargetDegreesPerSecond(left);
    rightMotor.setTargetDegreesPerSecond(right);
}

void Chassis::setMotorEfforts(float left, float right) 
{
    leftMotor.setEffortPercent(left);
    rightMotor.setEffortPercent(right);
}