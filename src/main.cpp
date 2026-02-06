#include "main.h"
#include "liblvgl/llemu.hpp"
#include "liblvgl/lvgl.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rotation.hpp"
#include "pros/rtos.hpp"
// #include "robodash/api.h" // IWYU pragma: keep
#include "liblvgl/lvgl.h" // IWYU pragma: keep
#include "pros/misc.hpp"
#include "pros/motors.h"
// #include "robodash/views/selector.hpp"
#include "autons.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>

bool imudc = false;
bool useBoth = true;
bool ran = false;
// rd::Console console;

// rd::Selector selector({
//     {"Solo AWP", soloAWP},
//     {"Left Auton", leftAuton},
//     {"Right Auton", rightAuton},
//     {"Skills", skills5},
//     {"Skills4", skills4}
// });


void nullfunc() {}

void (*autonFunc[])() = {nullfunc, soloAWP, leftAuton, rightAuton};
std::string autonStr[] = {"Nothing", "Solo AWP", "Left Auton", "Right AUton"};
int autonIndex = 0;

void initialize() {

//   try {selector.focus();} 
//   catch (std::exception e) {}

    chassis.calibrate();
	ballDetector.set_led_pwm(50);
	ballDetector.set_integration_time(3);


	pros::Task intakeTask([]() {
		int jamAmt = 0;
		int jam2Amt = 0;
		while (true) {
			intakeMotor.move(intake * 1.27);

			if (
				(intake == 100 && intakeMotor.get_actual_velocity() < 5) ||
				(jamAmt < 0) && (jam2Amt < 200)
			) {
				jamAmt++;
				if (jamAmt > 4) {
					intakeMotor.move(intake * -1.27);
					pros::delay(100);
					jamAmt = -10;
					jam2Amt += 50;
				}
			}


			if (jam2Amt > 0) jam2Amt--;

			pros::delay(20);
		}
	});


	pros::Task screenTask([]() {
		controller.clear();

		while (true) {
			// Detect motor burn out
			if (intakeMotor.get_temperature() >= 45) {
				controller.print(0, 0, "Intake1: %.0fC ", intakeMotor.get_temperature());
				pros::delay(500);
			} else if (intakeMotor.get_temperature() >= 45) {
				controller.print(0, 0, "Intake2: %.0fC ", intakeMotor.get_temperature());
			} else {
				// DEBUG
				if (debug && ran) {
					
					controller.print(0, 0, "%.1f %.1f %.0f    ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
					// std::cout << chassis.getPose().x << " " << chassis.getPose().y << " " << chassis.getPose().theta << "\n";
					// controller.print(0, 0, "%din   ", ballDetector.get_distance());
				}
			}
			if (!imu.is_installed()) 
				imudc = true;
			if (imudc)
    			controller.print(0, 0, "IMU Disconnected");
		


			// Delay to save resources
			pros::delay(1000);
		}
	});

	pros::Task autonTask([]() {

		while (true) {
			if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
				autonIndex++;
				if (autonIndex >= sizeof(autonStr)) autonIndex = 0;
				controller.print(0, 0, "%s     ", autonStr[autonIndex]);
			}
			else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
				autonIndex--;
				if (autonIndex <= 0) autonIndex = sizeof(autonStr);
				controller.print(0, 0, "%s     ", autonStr[autonIndex]);
			}

			pros::delay(100);
		}
	});
}

void testdrive() {
	int motors[] = {-20, -19, -18, 7,9,10};
	for (int motor : motors) {
	 	pros::Motor m(motor, pros::MotorGearset::blue);
		m.move(127);
		pros::delay(500);
		m.move(0);
		pros::delay(500);
	}
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
void competition_initialize() {
	// selector.focus();
}

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
 

bool gutterOn = false;
bool ruiguan = false;
bool intakeOn = false;
bool intakeLimiter = false;
bool wingOn = false;


void autonomous() {
	ran = true;
	soloAWP();
	// leftAuton();
	// chassis.moveToPoint(0, 48, 10000);
	// autonFunc[autonIndex]();
	// skills5();
}

bool onHoldMode = false;
int editCounter = 0;
bool nomove = false;


void opcontrol() {	
	ran = true;
	// chassis.moveToPoint(0, 24, 10000);
	// pros::delay(10000);
	
	usingIntake = false;
	while (true) {
		// Variables
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  

		const int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		const int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		const int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
		const int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

		const int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
			editCounter++;
			if (editCounter == 3) {
				controller.print(0, 0, "Entering EDIT 	");
			} else if (editCounter == 6) {
				controller.print(0, 0, "Exiting EDIT	");
				editCounter = 0;
			}
		}
		if (editCounter >= 3) {
			if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
				useBoth = !useBoth;
				controller.print(0, 0, "ManIntake %s	", useBoth ? "ON" : "OFF");
			}
			if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
				debug = !debug;
				controller.print(0, 0, "Debug %s		", useBoth ? "ON" : "OFF");
			}
		} 


		// //X: Intake limiter
		// if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
		// 	eject_amount_stop(3, 3000);

		// Drive function (Lemlib)
		chassis.arcade(leftY, rightX, false, 0.4);

		// Activate intake
		// if (!usingIntake) {
		intake = r2 * 100 + l2 * -100 + r1 * 100 + l1 * -100;
		// }

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			wingOn = true;
			wing.set_value(wingOn);
		}

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			wingOn = false;
			wing.set_value(wingOn);
		}

		// BUTTONS

		// A: Gutter activation
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			gutterOn = !gutterOn;
			gutter.set_value(gutterOn);
		}
		// B: ruiguan activation
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			ruiguan = !ruiguan;
			ruiguanChange(ruiguan);
		}
		
		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
		// 	testdrive();		// //C: Intake piston activation (park)
		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
		// 	eject_amount(3, 5000);
		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
		// 	intakeOn = !intakeOn;
		// 	intakePiston.set_value(intakeOn);
		// }

		//D: Wing activation
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			wingOn = !wingOn;
			wing.set_value(wingOn);
		}
		// Y: Switch between hold mode
		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
		// 	onHoldMode = !onHoldMode;
		

		//X: Intake limiter
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
			intakeMotor.move(-45);

		// Adjustable motor
		// if (abs(rightX) < 10 && abs(rightY) > 10) 
		// 	adjustableMotor.move(rightY);
		// else
		// 	adjustableMotor.move(0);

		// If controller is disconnected then set the robot to hold mode
		// It won us provs LOL
		if (!controller.is_connected() || onHoldMode || (gutterOn)) {
			chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
		} else {
			chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
		}

		// Delay to save resources. 10 MS for low latency
		pros::delay(10);
	}
}
 