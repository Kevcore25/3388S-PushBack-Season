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
int displayTime = 0;

// rd::Selector selector({
//     {"Solo AWP", soloAWP},
//     {"Left Auton", leftAuton},
//     {"Right Auton", rightAuton},
//     {"Skills", skills5},
//     {"Skills4", skills4}
// });



void nullfunc() {
	move_forward(2, 1000);
}

void (*autonFunc[])() = {nullfunc, soloAWP, leftAuton};
std::string autonStr[] = {"1in fwd", "Solo AWP", "Left Auton"};
int autonIndex = 0;


void update() {
	pros::lcd::print(3, "Selected: %s", autonStr[autonIndex]);
}

void left() {
	autonIndex -= 1;
	if (autonIndex < 0) autonIndex = std::size(autonStr) - 1;
	update();
}

void right() {
	autonIndex += 1;
	if (autonIndex >= std::size(autonStr)) autonIndex = 0;
	update();
}

void initialize() {

//   try {selector.focus();} 
//   cach (std::exception e) {}
	pros::lcd::initialize();
    chassis.calibrate();
	chassis.setPose(0, 0, 0);
	ballDetector.set_led_pwm(50);
	ballDetector.set_integration_time(3);

	pros::lcd::register_btn0_cb(left);

	pros::lcd::register_btn2_cb(right);
	pros::lcd::register_btn1_cb(autonomous);


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
				if (debug && displayTime == 0) {		
					pros::lcd::print(1, 0, "X: %.2f | Y: %.2f  |THETA: %.2f    ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
					controller.print(0, 0, "%.1f %.1f %.0f        ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
					// std::cout << chassis.getPose().x << " " << chassis.getPose().y << " " << chassis.getPose().theta << "\n";
					// controller.print(0, 0, "%din   ", ballDetector.get_distance());
				}
			}
			if (!imu.is_installed()) 
				imudc = true;
			if (imudc)
    			controller.print(0, 0, "IMU Disconnected");
		

			if (displayTime > 0) displayTime--;
			// Delay to save resources
			pros::delay(200);
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
	stopper.set_value(1);
	// ran = true;
	// adjustableRuiguan.set_value(1);
	// leftAuton();
	// soloAWP();
	// autonFunc[autonIndex]();
	// move_forward(2, 1000);
	// chassis.moveToPoint(0, 24, 5000);
	// chassis.turnToHeading(180, 10000);
	// leftAuton();
	// chassis.moveToPoint(0, 48, 10000);
	// autonFunc[autonIndex]();
	// skills5();
	// soloAWP();
	// skills6();
	// rightAuton2();
	// leftAuton();
	soloAWP();
	// chassis.turnToHeading(90, 10000);
	
}

bool onHoldMode = false;
int editCounter = 0;
bool nomove = false;

void display() {
	controller.print(0, 0, "kP: %.1f kD: %.1f kI: %.1f    ", lateral_controller.kP, lateral_controller.kD, lateral_controller.kI);
	displayTime = 10;
}

void tuner() {
	displayTime = 100;

	int editing = 0;
	while (true) {
		const bool left = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);
		const bool right = controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT);

		const bool A = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
		const bool X = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		const bool B = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);

		if (A || X || B) {
			lemlib::Chassis chassis(
				drivetrain, // drivetrain settings
				lateral_controller, // lateral PID settings
				angular_controller, // angular PID settings
				sensors,
				&throttle_curve,
				&steer_curve
			);
			
			chassis.setPose(0, 0, 0);
			displayTime = 0;
			if (A) chassis.moveToPoint(0, 24, 2000);
			if (X) chassis.moveToPoint(0, 48, 2000);
			if (B) chassis.moveToPoint(0, 72, 3000);
			chassis.waitUntilDone();
		}
		
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
			editing = 1;
			controller.print(0, 0, "Editing kP       ");
			displayTime = 5;
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			editing = 2;
			controller.print(0, 0, "Editing kD       ");
			displayTime = 5;
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			editing = 3;
			controller.print(0, 0, "Editing kI       ");
			displayTime = 5;
		}
		if (left) {
			// KP
			if (editing == 1)  {
				lateral_controller.kP -= 0.1;
			}
			if (editing == 2) {
				lateral_controller.kD -= 0.1;
			}
			if (editing == 3) {
				lateral_controller.kI -= 0.1;
			}
			display();
		}

		if (right) {
			// KP
			if (editing == 1)  {
				lateral_controller.kP += 0.1;
			}
			if (editing == 2) {
				lateral_controller.kD += 0.1;
			}
			if (editing == 3) {
				lateral_controller.kI += 0.1;
			}
			display();
		}

		if (std::abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) > 50 || std::abs(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)) > 50) {
			displayTime = 1;
			controller.print(0, 0, "Press L1 to exit       ");
		}

		if (controller.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1)) break;
		if (controller.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L2)) move_forward(-72, 1000, false, {.maxSpeed=70});

		pros::delay(100);
	}


}

void opcontrol() {	
	// chassis.moveToPoint(0, 24, 10000);
	// pros::delay(10000);
	// tuner();
	tuner();
	if (willWing) wing.set_value(1);
	
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
		const int down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

		const int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

		// if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		// 	editCounter++;
		// 	if (editCounter == 3) {
		// 		controller.print(0, 0, "Entering EDIT 	");
		// 	} else if (editCounter == 6) {
		// 		controller.print(0, 0, "Exiting EDIT	");
		// 		editCounter = 0;
		// 	}
		// }
		// if (editCounter >= 3) {
		// 	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		// 		useBoth = !useBoth;
		// 		controller.print(0, 0, "ManIntake %s	", useBoth ? "ON" : "OFF");
		// 	}
		// 	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		// 		debug = !debug;
		// 		controller.print(0, 0, "Debug %s		", useBoth ? "ON" : "OFF");
		// 	}
		// } 


		// //X: Intake limiter
		// if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
		// 	eject_amount_stop(3, 3000);

		// Drive function (Lemlib)
		chassis.arcade(leftY, rightX, false, 0.4);

		// Activate intake
		// if (!usingIntake) {
		intake = r2 * 100 + l2 * -100 + r1 * 100 + l1 * -100 + down * 40;
		// }

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			stopper.set_value(false);
		}

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			stopper.set_value(true);
		}

		// BUTTONS

		// A: Gutter activation
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
			gutterOn = !gutterOn;
			gutter.set_value(gutterOn);
			if (gutterOn){
				ruiguan = false;
				ruiguanChange(ruiguan);
			}
		}
		// B: ruiguan activation
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			ruiguan = !ruiguan;
			ruiguanChange(ruiguan);
			if (ruiguan){
				gutterOn = false;
				gutter.set_value(gutterOn);
			}
			wing.set_value(0);
		}
		
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
			// testdrive();		// //C: Intake piston activation (park)
			eject_amount(3, 3000);

		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {

			if (std::abs(chassis.getPose().x + chassis.getPose().y) < 0.1) 
				chassis.setPose(0, 0, 0);
			else
				chassis.setPose(0, 0, chassis.getPose().theta);

		}
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
 