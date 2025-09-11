/**
* \file main.h
*
* Contains common definitions and header files used throughout your PROS
* project.
*
* \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
* All rights reserved.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
* If defined, some commonly used enums will have preprocessor macros which give
* a shorter, more convenient naming pattern. If this isn't desired, simply
* comment the following line out.
*
* For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
* E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
* not convenient for most student programmers.
*/
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#define PROS_USE_SIMPLE_NAMES

/**
* If defined, C++ literals will be available for use. All literals are in the
* pros::literals namespace.
*
* For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
*/
#define PROS_USE_LITERALS

#include "api.h"
#include "robodash/api.h"
#include "lemlib/api.hpp"

/**
* You should add more #includes here
*/
//#include "okapi/api.hpp"

/**
* If you find doing pros::Motor() to be tedious and you'd prefer just to do
* Motor, you can use the namespace with the following commented out line.
*
* IMPORTANT: Only the okapi or pros namespace may be used, not both
* concurrently! The okapi namespace will export all symbols inside the pros
* namespace.
*/
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
* Prototypes for the competition control tasks are redefined here to ensure
* that they can be called from user code (i.e. calling autonomous from a
* button press in opcontrol() for testing purposes).
*/
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

inline int intake = 0;
inline int rollerIntake = 0;

inline pros::MotorGroup left_motors({-20, -19, -18}, pros::MotorGearset::blue);
inline pros::MotorGroup right_motors({11, 12, 13}, pros::MotorGearset::blue);

inline pros::Motor intakeMotor(16);

inline pros::Imu imu(17);

inline pros::adi::DigitalOut gutter('A');
inline pros::adi::DigitalOut doorMid('B');

inline pros::Rotation verticalTrackingWheel(-15);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

inline lemlib::Drivetrain drivetrain(
    &left_motors,
    &right_motors,
    11.25,
    lemlib::Omniwheel::NEW_325,
    360,
    2
);


inline lemlib::ControllerSettings lateral_controller(
    8.2, // proportional gain (kP)
    0, // integral gain (kI)
    4.6, // derivative gain (kD)
    0, // anti windup
    0.25, // small error range, in inches
    50, // small error range timeout, in milliseconds
    0.5, // large error range, in inches
    200, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

inline lemlib::ControllerSettings angular_controller(
    2, // proportional gain (kP)
    0, // integral gain (kI)
    10, // derivative gain (kD)
    0, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    0 // maximum celeration (slew)
);

inline lemlib::TrackingWheel verticalTracking(&verticalTrackingWheel, lemlib::Omniwheel::NEW_2, -1.0);

inline lemlib::OdomSensors sensors(
    &verticalTracking, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &imu//&imu // inertial sensor
);


// input curve for throttle input during driver control
inline lemlib::ExpoDriveCurve throttle_curve(
    5, // joystick deadband out of 127
    10, // minimum output where drivetrain will move out of 127
    1.021 // expo curve gain
);

// input curve for steer input during driver control
inline lemlib::ExpoDriveCurve steer_curve(
    3, // joystick deadband out of 127
    0, // minimum output where drivetrain will move out of 127
    1.021 // expo curve gain
);

inline lemlib::Chassis chassis(
    drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors,
    &throttle_curve,
    &steer_curve
);

extern lemlib::Drivetrain drivetrain;

extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

extern lemlib::TrackingWheel horizontalTracking;

extern lemlib::OdomSensors sensors;

extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

extern lemlib::Chassis chassis;

inline int sign(int num) {
    if (num == 0)
        return 0;
    return (num >= 0) ? 1 : -1;
}

inline float lateralInputRemap(float input, float scale) {
    if (scale != 0) {
        auto eq1 = [](float x, float a) {
        return powf(a, std::abs(x) - 127) * (std::abs(x)) * sign(x);
        };

        return (eq1(input, scale) * 127.0 / (eq1(127, scale)));
    }
    return input;
}

inline float angularInputRemap(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) *
                                                (1 - powf(2.718, -(scale / 10)))) *
            input;
    }
    return input;
}

inline void arcade(int throttle, int angular) {
    int deadzone = 5;

    throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
    angular = (abs(angular) <= deadzone) ? 0 : angular;

    throttle = lateralInputRemap(throttle, 1.021);

    angular = angularInputRemap(angular, 7.5) * 0.6;

    if (throttle + angular < 100) {
        angular /= 0.6;
    }

    left_motors.move(throttle + angular);
right_motors.move(throttle - angular);
}


inline void move_forward(float inches, int timeout, bool async = false, lemlib::MoveToPointParams params = {}) {
    bool forwards = true;

    if (inches < 0) {
        forwards = false;
    } 

    params.forwards = forwards;


    double angle = -(chassis.getPose(true).theta) + M_PI_2;

    float x = cos(angle) * inches + chassis.getPose().x;
    float y = sin(angle) * inches + chassis.getPose().y;
    
    chassis.moveToPoint(x, y, timeout, params, async);
}


inline void move_to_relative_point(float x, float y, int timeout, bool async = false, lemlib::MoveToPointParams params = {}){
    chassis.moveToPoint(chassis.getPose().x + x, chassis.getPose().y + y, timeout, params, async);
}


/* 
Turns to a specified angle and stops when it is around that range.
The normal chassis.turnToHeading requires full timeout in order to move onto the next move, since our PID is bad.
*/
inline void quick_turn_to(float theta, int maxTimeout = 2000, float errorRange = 1.5, lemlib::TurnToHeadingParams params = {}) {
    chassis.turnToHeading(theta, maxTimeout, params, true);

    int maxWhileLoops = maxTimeout / 5;    

    while (maxWhileLoops > 0) {
        float currentAngle = chassis.getPose().theta;
        float error = std::abs(theta - currentAngle);
        
        // If the current angle is within the error range then stop
        if (error <= errorRange) {
            chassis.cancelMotion();
            break;
        }
        maxWhileLoops--;
        pros::delay(5);
    }
}

/* 
Just like the quick_turn_to function, this will attempt to move the robot to x inches and stop when it reaches the error range.
The purpose is to provide quicker movements at the cost of accuracy.
E.g. you can have 2 "chained" move forward functions like:

void autonomous() {
    quick_move_forward(5);
    quick_turn_to(90);
    quick_move_forward(5);
}
*/
inline void quick_move_forward(float inches, int maxTimeout = 5000, float errorRange = 0.5, lemlib::MoveToPointParams params = {}) {
    bool forwards = true;

    if (inches < 0) {
        forwards = false;
    } 

    params.forwards = forwards;

    double angle = -(chassis.getPose(true).theta) + M_PI_2;

    float x = cos(angle) * inches + chassis.getPose().x;
    float y = sin(angle) * inches + chassis.getPose().y;
    
    chassis.moveToPoint(x, y, maxTimeout, params, {});

    int maxWhileLoops = maxTimeout / 5;    

    while (maxWhileLoops > 0) {
        float currentX = chassis.getPose().x;
        float currentY = chassis.getPose().y;

        double xDiff = std::abs(x - currentX);
        double yDiff = std::abs(y - currentY);

        float error = std::sqrt(xDiff * xDiff + yDiff * yDiff);
        
        // If the current distance is within the error range then stop
        if (error <= errorRange) {
            chassis.cancelMotion();
            break; // Once the while loop is broken, the function completes
        }
        maxWhileLoops--;
        pros::delay(5);
    }

}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
* You can add C++-only headers here
*/
#endif
#endif