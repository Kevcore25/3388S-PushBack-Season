#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "robodash/api.h" // IWYU pragma: keep

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

bool imudc = false;

void initialize() {
    chassis.calibrate();     // calibrate sensors
  	chassis.setPose(0.0, 0.0, 0.0);

//   try {selector.focus();} 
//   catch (std::exception e) {}

	pros::Task intakeTask([]() {
		intakeMotor.move(intake * 1.27);
		pros::delay(20);
	});

	pros::Task screenTask([&]() {
		controller.clear();

		while (true) {
			// Detect motor burn out
			if (intakeMotor.get_temperature() >= 45) {
				controller.print(0, 0, "Intake: %.0fC ", intakeMotor.get_temperature());
			} else {
				// DEBUG
				controller.print(0, 0, "%.1f %.1f %.0f  ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
			}

			if (!imu.is_installed()) 
				imudc = true;
		
			if (imudc) 
				controller.print(2, 0, "IMU Disconnected");

			// delay to save resources
			pros::delay(500);
		}
	});
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {	
	while (true) {
		// DRIVE FUNCTION
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  
		int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
  
		// Drive function (Lemlib)
		if (abs(leftY) > 5) 
			chassis.arcade(leftY, rightX, false, 0.4);
  
		// Activate intake
		intake = r2 * 100 + l2 * -100;

		// Delay to save resources. 10 MS for low latency
		pros::delay(10);
	}
}