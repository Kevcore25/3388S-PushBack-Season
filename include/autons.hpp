// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"



inline void turn_to(float degrees, bool async = false) {
   float timeout = std::abs(chassis.getPose().theta - degrees) * 3 + 400;

   chassis.turnToHeading(degrees, timeout, {}, async);
}

inline void turn(float degrees, bool async = false) {
   float timeout = std::abs(chassis.getPose().theta - degrees) * 8 + 300;
   float angle = chassis.getPose().theta + degrees;

   chassis.turnToHeading(angle, timeout, {}, async);
}


inline void move24() {
   chassis.setPose(0, 0, 0);
   chassis.moveToPoint(0, 24, 3000);

}
inline void rightAuton() {
// / Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(-15, 24, 90);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

   // Go to the match load
   gutter.set_value(1);
   chassis.moveToPoint(-48, 24, 1000, {});
   chassis.turnToHeading(180, 500, {}, false);
   // bottomIntake = 100;
   chassis.moveToPoint(-48, -4, 800, {.maxSpeed=60}, false);
   pros::delay(500);
   chassis.moveToPoint(-48, -4, 500, {.maxSpeed=80}, false);

   // Score to the tube
   chassis.moveToPoint(-49, 72, 1000, {.forwards=false, .maxSpeed=80});
   chassis.waitUntilDone();
   // bottomIntake = 0;
   // fullIntake = 100;
   // eject_amount(4, 3000);
   gutter.set_value(0);
   // pros::delay(3000);
   // eject_amount_stop(4, 2000);

   move_forward(12, 500, false, {.maxSpeed=50});
   pros::delay(500);
   move_forward(-48, 800, false, {.minSpeed=127});

   // Go to the 3 blocks 
   move_forward(12, 500);
   chassis.turnToHeading(45, 800);
   chassis.moveToPoint(-24-4, 48+4, 2000);
   chassis.waitUntilDone();
   gutter.set_value(1);
   
   pros::delay(1000);
   chassis.turnToHeading(-135, 1000);

   chassis.moveToPose(-22, 50, -45, 1000, {.forwards=false});
   chassis.waitUntilDone();
   gutter.set_value(1);
   // fullIntake = 0; 
   // bottomIntake = 100;
   pros::delay(500);
   gutter.set_value(0);
   move_forward(2, 500);
   chassis.moveToPose(22 - 12, 48 + 10, -45, 1500, {.lead=0.2, .maxSpeed=70});
   chassis.waitUntilDone();
   // move_forward(16, 1000);

   // bottomIntake = -100;

}

inline void soloAWP() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   // chassis.setPose(12, 24, 90);
   chassis.setPose(13, 24, 90);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
   adjustableRuiguan.set_value(0);

   // Go to the match load
   gutter.set_value(1);
   intake = 100;

   // OPTIMIZED PATH
   // chassis.moveToPose(48, -5, 179, 2000, {.lead=0.5, .minSpeed=60, .earlyExitRange=46});
   // chassis.moveToPose(48, 0, 180, 1000, {.minSpeed=80});

   chassis.moveToPoint(47, 24, 1000);
   chassis.turnToHeading(180, 600);
   chassis.moveToPoint(47, 4, 800, {.maxSpeed=70});
   chassis.waitUntilDone();
   pros::delay(500);

   // Score to the tube
   chassis.moveToPoint(47, 46, 1000, {.forwards=false, .maxSpeed=70});
   chassis.waitUntilDone();
   gutter.set_value(0);
   eject_amount(4, 1500);

   chassis.turnToHeading(-75, 800, {.minSpeed=70});
   intake = 100;
   chassis.waitUntilDone();
   stopper.set_value(0);
   chassis.moveToPoint(-24, 44, 2000);
   chassis.waitUntil(15);
   stopper.set_value(1);
   chassis.turnToHeading(-130, 800);
   adjustableRuiguan.set_value(1);
   chassis.moveToPose(-11, 56, -135, 1000, {.forwards=false, .minSpeed=80});
   // move_forward(-12, 1000, true, {.minSpeed=60});
   chassis.waitUntilDone();
   eject_amount(3, 2000);
   adjustableRuiguan.set_value(0);

   chassis.moveToPose(-44, -10, 180, 1800, {.lead=0.45, .minSpeed=80, .earlyExitRange=50});
   gutter.set_value(1);
   intake = 100;
   chassis.moveToPoint(-44, 0, 500, {.maxSpeed=50});
   chassis.waitUntilDone();
   pros::delay(100);
   chassis.moveToPoint(-45, 46, 1000, {.forwards=false, .maxSpeed=80});
   chassis.waitUntilDone();
   eject_amount(4, 2000);
}



inline void leftAuton() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(-16, 24, 0);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
   adjustableRuiguan.set_value(0);

   // Get first 3 blocks
   intake = 100;
   chassis.moveToPoint(-24, 48, 2000);
   chassis.waitUntil(24);
   gutter.set_value(1);
   chassis.waitUntilDone();
   pros::delay(1000);

   // Score to mid
   chassis.turnToHeading(-135, 1000);
   // chassis.moveToPoint(-10, 54, 2000, {.forwards=false});
   chassis.waitUntilDone();
   gutter.set_value(0);
   adjustableRuiguan.set_value(1);
   move_forward(-13, 1000);
   eject_amount(4, 3000);

   // Go to the matchloader
   adjustableRuiguan.set_value(0);
   gutter.set_value(1);
   chassis.moveToPose(-47, 0, 180, 2000, {.lead=0.5,.maxSpeed=100, .minSpeed=50,  .earlyExitRange=50});
   chassis.moveToPoint(-46, 0, 500, {.maxSpeed=50});
   intake = 100;
   chassis.waitUntilDone();
   pros::delay(500);

   // Score
   chassis.turnToHeading(180, 700);
   // move_forward(-44, 2000);
   chassis.moveToPoint(-46, 44, 1000, {.forwards=false, .maxSpeed=70});
   chassis.waitUntilDone();
   eject_amount(3, 2000);
}

inline void rightAuton2() {
   chassis.setPose(19, 24, 13);

   intake = 100;  
   // chassis.moveToPoint(26, 50, 1000);
   // move_forward(26, 1000, false, {.maxSpeed=80});
   chassis.moveToPoint(25, 50, 2000);
   chassis.waitUntil(20);
   gutter.set_value(1);
   chassis.waitUntilDone();
   pros::delay(1000);
   chassis.turnToHeading(150, 1000);
   chassis.moveToPose(47.5, 4, 180, 2500, {.lead=0.6, .minSpeed=50});
   chassis.waitUntilDone();
   pros::delay(500);
   // move_forward(-28, 1000);
   chassis.moveToPoint(48, 46, 1500, {.forwards=false});
   chassis.waitUntilDone();
   eject_amount(7, 4000);
   chassis.moveToPoint(60, 28, 1000);
   chassis.turnToHeading(180, 600);
   chassis.moveToPoint(62, 62, 2000, {.forwards=false, .maxSpeed=80});
   willWing=true;
   // chassis.moveToPoint(47, 46, 2000, {.forwards=false});
}


inline void skills4() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(15, 24, 90);
   wing.set_value(1);

   // BOTTOM RIGHT SIDE
      // Go to the match load
//       gutter.set_value(1);
//       chassis.moveToPoint(47, 24, 1000, {});
//       chassis.turnToHeading(180, 500, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(47, -4, 800, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(4, 2000); // Eject red, should be 3 BLUE in robo

//       // Prepare bottom left
//       chassis.moveToPoint(47, 24, 1000);
//       chassis.turnToHeading(270, 700);

//    // BOTTOM LEFT SIDE
//       // Go to the match load
//       chassis.moveToPoint(-46, 24, 5000, {});
//       chassis.turnToHeading(180, 700, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(-46, -4, 1000, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
//       // Prepare for TOP LEFT
//       gutter.set_value(0);
//       move_forward(12, 500);
//       chassis.turnToHeading(270, 700);
//       chassis.moveToPoint(-48 - 18, 48 - 12, 1000);
//       chassis.turnToHeading(0, 1000);
//       chassis.waitUntilDone(); 
//       fullIntake=100; // Eject blocks

//    // TOP LEFT SIDE
//       // Move to matchloader
//       gutter.set_value(1);
//       chassis.moveToPoint(-48 - 20, 120 - 24, 4000);
//       chassis.moveToPose(-48 - 12, 120 + 12, 0, 2000, {.minSpeed=70});
//       chassis.waitUntilDone();
//       bottomIntake = 100;
//       fullIntake = 0;
//       chassis.moveToPoint(-46 - 10, 120 + 24 + 4, 1000);
//       pros::delay(2000);
      
//       // Score to the tube
//       chassis.moveToPoint(-48 - 10, 120 - 28, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(3, 2000); // Eject red, should be 3 BLUE in robo

//    // TOP RIGHT SIDE
//       // Knock balls
//       gutter.set_value(0);
//       chassis.moveToPose(12, 120 + 8, 95, 5000, {.lead=0.7, .minSpeed=70, .earlyExitRange =8});
//       chassis.moveToPoint(24, 120 + 8, 1000);
//       chassis.turnToHeading(120, 1000);
//       move_forward(60, 3000);


//       // reset
//       chassis.setPose(0, 0, 90);
//       move_forward(-18, 1000);
//       chassis.turnToHeading(0, 1000);
//       gutter.set_value(1);
//       chassis.waitUntilDone();

//       bottomIntake = 100;
//       move_forward(24, 1000);
//       pros::delay(2000);
//       move_forward(-48, 2000);
//       fullIntake=100;
      gutter.set_value(1);
//       chassis.moveToPoint(47, 24, 1000, {});
//       chassis.turnToHeading(180, 500, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(47, -4, 800, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(4, 2000); // Eject red, should be 3 BLUE in robo

//       // Prepare bottom left
//       chassis.moveToPoint(47, 24, 1000);
//       chassis.turnToHeading(270, 700);

//    // BOTTOM LEFT SIDE
//       // Go to the match load
//       chassis.moveToPoint(-46, 24, 5000, {});
//       chassis.turnToHeading(180, 700, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(-46, -4, 1000, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
//       // Prepare for TOP LEFT
//       gutter.set_value(0);
//       move_forward(12, 500);
//       chassis.turnToHeading(270, 700);
//       chassis.moveToPoint(-48 - 18, 48 - 12, 1000);
//       chassis.turnToHeading(0, 1000);
//       chassis.waitUntilDone(); 
//       fullIntake=100; // Eject blocks

//    // TOP LEFT SIDE
//       // Move to matchloader
//       gutter.set_value(1);
//       chassis.moveToPoint(-48 - 20, 120 - 24, 4000);
//       chassis.moveToPose(-48 - 12, 120 + 12, 0, 2000, {.minSpeed=70});
//       chassis.waitUntilDone();
//       bottomIntake = 100;
//       fullIntake = 0;
//       chassis.moveToPoint(-46 - 10, 120 + 24 + 4, 1000);
//       pros::delay(2000);
      
//       // Score to the tube
//       chassis.moveToPoint(-48 - 10, 120 - 28, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(3, 2000); // Eject red, should be 3 BLUE in robo

//    // TOP RIGHT SIDE
//       // Knock balls
//       gutter.set_value(0);
//       chassis.moveToPose(12, 120 + 8, 95, 5000, {.lead=0.7, .minSpeed=70, .earlyExitRange =8});
//       chassis.moveToPoint(24, 120 + 8, 1000);
//       chassis.turnToHeading(120, 1000);
//       move_forward(60, 3000);


//       // reset
//       chassis.setPose(0, 0, 90);
//       move_forward(-18, 1000);
//       chassis.turnToHeading(0, 1000);
//       gutter.set_value(1);
//       chassis.waitUntilDone();

//       bottomIntake = 100;
//       move_forward(24, 1000);
//       pros::delay(2000);
//       move_forward(-48, 2000);
//       fullIntake=100;
      
}



// inline void skills5() {
//    // Tune the major. Make sure center (0, 0) is the center wall of the field
//    chassis.setPose(16, 24, 90);
//    wing.set_value(1);

//    // BOTTOM RIGHT SIDE
//       // Go to the match load
//       gutter.set_value(1);
//       chassis.moveToPoint(47, 24, 1000, {});
//       chassis.turnToHeading(180, 500, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(47, -4, 800, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(4, 2000); // Eject red, should be 3 BLUE in robo

//       // Prepare bottom left
//       chassis.moveToPoint(47, 24, 1000);
//       chassis.turnToHeading(270, 700);

//    // BOTTOM LEFT SIDE
//       // Go to the match load
//       chassis.moveToPoint(-46, 24, 5000, {});
//       chassis.turnToHeading(180, 700, {}, false);
//       bottomIntake = 100;
//       chassis.moveToPoint(-46, -4, 1000, {.maxSpeed=80}, false);
//       pros::delay(2000);

//       // Score to the tube
//       chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
//       chassis.waitUntilDone();
//       bottomIntake = 0;
//       eject_amount_stop(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
//       // Prepare for PARK
//       gutter.set_value(0);
//       move_forward(24, 2000);
//       chassis.turnToHeading(180, 2000);

//    // Park
//       chassis.moveToPose(24, 4, 90, 5000, {.lead=0.7, .minSpeed=80});
//       chassis.waitUntilDone();
//       move_forward(-24, 3000);
// }
// }



inline void skills5() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(13, 24, 90);
   wing.set_value(0);


   // BOTTOM RIGHT SIDE
      // Go to the match load
      gutter.set_value(1);
      chassis.moveToPoint(48, 24, 1000, {});
      chassis.turnToHeading(180, 500, {}, false);
      intake = 100;
      chassis.moveToPoint(48, -4, 800, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      intake = 0;
      eject_amount(4, 2000); // Eject red, should be 3 BLUE in robo

      // Prepare bottom left
      chassis.moveToPoint(48, 24, 1000);
      chassis.turnToHeading(270, 700);

   // BOTTOM LEFT SIDE
      // Go to the match load
      chassis.moveToPoint(-47.5, 24, 5000, {});
      chassis.turnToHeading(180, 700, {}, false);
      intake = 100;
      chassis.moveToPoint(-47.5, -4, 1000, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      intake = 0;
      eject_amount(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
      // Prepare for PARK
      gutter.set_value(0);
      move_forward(12, 2000);
      chassis.turnToHeading(180, 1000);

   // Park
      chassis.moveToPose(6, 1.5, 90, 10000, {.lead=0.6, .minSpeed=80});
      chassis.waitUntilDone();
}



inline void skills6() {
   chassis.setPose(14, 24, -90);

   move_forward(32, 1000);
   gutter.set_value(1);
   turn_to(180); 
   intake = 100;
   move_forward(24, 1000, false, {.maxSpeed=80});
   pros::delay(1000);
   
   move_forward(-8, 800);
   turn_to(-45);
   intake = 0;
   gutter.set_value(0);
   move_forward(26, 1000);
   chassis.turnToHeading(5, 1000, {}, false);
   wing.set_value(1);
   move_forward(70, 3000);
   turn_to(45);
   
   move_to_relative_pose(12, 26, 2000, -5, {.minSpeed=30}, false);
   move_forward(-30, 1000);
   eject_amount(6, 2000);
   intake = 100;
   gutter.set_value(1);
   move_forward(30, 1000);
   eject_amount(6, 2000);
   gutter.set_value(0);
   move_forward(-28, 1000);
}