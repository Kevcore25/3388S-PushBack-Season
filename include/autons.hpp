// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"



inline void turn(float degrees, int timeout, bool async = false) {
   float angle = chassis.getPose().theta + degrees;

   chassis.turnToHeading(angle, timeout, {}, async);
}

inline void turn_to(float degrees, bool async = false) {
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
   chassis.setPose(15, 24, 90);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

   // Go to the match load
   gutter.set_value(1);
   chassis.moveToPoint(48, 24, 1000, {});
   chassis.turnToHeading(180, 500, {}, false);
   bottomIntake = 100;
   chassis.moveToPoint(48, -4, 800, {.maxSpeed=60}, false);
   pros::delay(500);
   chassis.moveToPoint(48, -4, 500, {.maxSpeed=80}, false);

   // Score to the tube
   chassis.moveToPoint(49, 72, 1000, {.forwards=false, .maxSpeed=80});
   chassis.waitUntilDone();
   bottomIntake = 0;
   fullIntake = 100;
   // eject_amount(4, 3000);
   gutter.set_value(0);
   // pros::delay(3000);
   eject_amount_stop(4, 2000);

   move_forward(12, 500, false, {.maxSpeed=50});
   pros::delay(500);
   move_forward(-48, 800, false, {.minSpeed=127});

   // Go to the 3 blocks 
   move_forward(12, 500);
   fullIntake = 100;
   chassis.turnToPoint(24, 48, 700);
   chassis.moveToPose(22, 50, -45, 1000, {.earlyExitRange=30});
   chassis.waitUntilDone();
   gutter.set_value(1);
   fullIntake = 0; 
   bottomIntake = 100;
   pros::delay(500);
   gutter.set_value(0);
   move_forward(2, 500);
   chassis.moveToPose(22 - 12, 48 + 10, -45, 1500, {.lead=0.2, .maxSpeed=70});
   chassis.waitUntilDone();
   // move_forward(16, 1000);

   bottomIntake = -100;

}

inline void soloAWP() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(15, 24, 90);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

   // Go to the match load
   gutter.set_value(1);
   chassis.moveToPoint(48, 24, 1000, {});
   chassis.turnToHeading(180, 500, {}, false);
   bottomIntake = 100;
   chassis.moveToPoint(48, -4, 800, {.maxSpeed=60}, false);
   pros::delay(500);
   chassis.moveToPoint(48, -4, 500, {.maxSpeed=80}, false);

   // Score to the tube
   chassis.moveToPoint(49, 72, 1000, {.forwards=false, .maxSpeed=80});
   chassis.waitUntilDone();
   bottomIntake = 0;
   fullIntake = 100;
   // eject_amount(4, 3000);
   gutter.set_value(0);
   // pros::delay(3000);
   eject_amount_stop(4, 2000);

   // Go to the 3 blocks 
   move_forward(12, 500);
   fullIntake = 100;
   chassis.turnToPoint(24, 48, 700);
   chassis.moveToPose(22, 50, -45, 1000, {.lead=0.3, .earlyExitRange=30});
   chassis.waitUntilDone();
   fullIntake = 0; 
   bottomIntake = -80;
   chassis.moveToPoint(22 - 10, 48 + 10, 1000, {.maxSpeed=70});
   chassis.waitUntilDone();
   bottomIntake = -100;
   pros::delay(500);

   // Get next 3
   move_forward(-12, 1000);
   fullIntake = 100; // Eject balls in the robo
   chassis.turnToPoint(-24, 48, 500);
   chassis.moveToPoint(-24 - 2, 48, 1800);
   chassis.waitUntilDone();
   move_forward(-4, 500);
   fullIntake = 0;
   bottomIntake = 100;
   chassis.waitUntil(40);
   gutter.set_value(1);
   chassis.waitUntilDone();

   // Score
   chassis.turnToHeading(-135, 700);
   chassis.waitUntilDone();
   move_forward(-20, 700);
   chassis.waitUntilDone();
   gutter.set_value(0);
   eject_amount_stop(3, 2000);



}



inline void leftAuton() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(-22, 24, -90);
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

   // Go to the match load
   gutter.set_value(1);
   chassis.moveToPoint(-49, 24, 1000, {});
   chassis.turnToHeading(-180, 500, {}, false);
   bottomIntake = 100;
   chassis.moveToPoint(-48, -4, 800, {.maxSpeed=60}, false);
   pros::delay(500);
   chassis.moveToPoint(-48, -4, 500, {.maxSpeed=80}, false);

   // Score to the tube
   chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
   chassis.waitUntilDone();
   bottomIntake = 0;
   fullIntake = 100;
   // eject_amount(4, 3000);
   gutter.set_value(0);
   // pros::delay(3000);
   eject_amount_stop(4, 2000);
   
   move_forward(12, 500, false, {.maxSpeed=50});
   pros::delay(500);
   move_forward(-48, 800, false, {.minSpeed=127});

   // Go to the 3 blocks 
   move_forward(12, 500);
   fullIntake=100;
   //bottomIntake = 100;
   chassis.turnToPoint(-24, 48, 700);
   chassis.moveToPose(-18, 48 + 2, 45, 1500, {.lead=0.3, .maxSpeed=60}, false);
   bottomIntake=100; fullIntake=0;
   gutter.set_value(1);
   pros::delay(500);
   chassis.waitUntilDone();
   chassis.turnToHeading(180 + 47, 1000);
   // chassis.moveToPose(-(12), 48 + 8, 180+45, 1000, {.forwards= false, .lead=0.2, .maxSpeed=90, .minSpeed=50});
   chassis.waitUntilDone();
   move_forward(-18, 1000);
   chassis.turnToHeading(180 + 45, 500);
   chassis.waitUntilDone();
   fullIntake = 100;
}


inline void skills4() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(15, 24, 90);
   wing.set_value(1);

   // BOTTOM RIGHT SIDE
      // Go to the match load
      gutter.set_value(1);
      chassis.moveToPoint(47, 24, 1000, {});
      chassis.turnToHeading(180, 500, {}, false);
      bottomIntake = 100;
      chassis.moveToPoint(47, -4, 800, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      bottomIntake = 0;
      eject_amount_stop(4, 2000); // Eject red, should be 3 BLUE in robo

      // Prepare bottom left
      chassis.moveToPoint(47, 24, 1000);
      chassis.turnToHeading(270, 700);

   // BOTTOM LEFT SIDE
      // Go to the match load
      chassis.moveToPoint(-46, 24, 5000, {});
      chassis.turnToHeading(180, 700, {}, false);
      bottomIntake = 100;
      chassis.moveToPoint(-46, -4, 1000, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      bottomIntake = 0;
      eject_amount_stop(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
      // Prepare for TOP LEFT
      gutter.set_value(0);
      move_forward(12, 500);
      chassis.turnToHeading(270, 700);
      chassis.moveToPoint(-48 - 18, 48 - 12, 1000);
      chassis.turnToHeading(0, 1000);
      chassis.waitUntilDone(); 
      fullIntake=100; // Eject blocks

   // TOP LEFT SIDE
      // Move to matchloader
      gutter.set_value(1);
      chassis.moveToPoint(-48 - 20, 120 - 24, 4000);
      chassis.moveToPose(-48 - 12, 120 + 12, 0, 2000, {.minSpeed=70});
      chassis.waitUntilDone();
      bottomIntake = 100;
      fullIntake = 0;
      chassis.moveToPoint(-46 - 10, 120 + 24 + 4, 1000);
      pros::delay(2000);
      
      // Score to the tube
      chassis.moveToPoint(-48 - 10, 120 - 28, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      bottomIntake = 0;
      eject_amount_stop(3, 2000); // Eject red, should be 3 BLUE in robo

   // TOP RIGHT SIDE
      // Knock balls
      gutter.set_value(0);
      chassis.moveToPose(12, 120 + 8, 95, 5000, {.lead=0.7, .minSpeed=70, .earlyExitRange =8});
      chassis.moveToPoint(24, 120 + 8, 1000);
      chassis.turnToHeading(120, 1000);
      move_forward(60, 3000);


      // reset
      chassis.setPose(0, 0, 90);
      move_forward(-18, 1000);
      chassis.turnToHeading(0, 1000);
      gutter.set_value(1);
      chassis.waitUntilDone();

      bottomIntake = 100;
      move_forward(24, 1000);
      pros::delay(2000);
      move_forward(-48, 2000);
      fullIntake=100;
      
}



inline void skills5() {
   // Tune the major. Make sure center (0, 0) is the center wall of the field
   chassis.setPose(16, 24, 90);
   wing.set_value(1);

   // BOTTOM RIGHT SIDE
      // Go to the match load
      gutter.set_value(1);
      chassis.moveToPoint(47, 24, 1000, {});
      chassis.turnToHeading(180, 500, {}, false);
      bottomIntake = 100;
      chassis.moveToPoint(47, -4, 800, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      bottomIntake = 0;
      eject_amount_stop(4, 2000); // Eject red, should be 3 BLUE in robo

      // Prepare bottom left
      chassis.moveToPoint(47, 24, 1000);
      chassis.turnToHeading(270, 700);

   // BOTTOM LEFT SIDE
      // Go to the match load
      chassis.moveToPoint(-46, 24, 5000, {});
      chassis.turnToHeading(180, 700, {}, false);
      bottomIntake = 100;
      chassis.moveToPoint(-46, -4, 1000, {.maxSpeed=80}, false);
      pros::delay(2000);

      // Score to the tube
      chassis.moveToPoint(-48, 72, 1000, {.forwards=false, .maxSpeed=80});
      chassis.waitUntilDone();
      bottomIntake = 0;
      eject_amount_stop(6, 3000); // Eject BLUE, there are currently: BBBBBBRRR that means 6
      
      // Prepare for PARK
      gutter.set_value(0);
      move_forward(24, 2000);
      chassis.turnToHeading(180, 2000);

   // Park
      chassis.moveToPose(24, 4, 90, 5000, {.lead=0.7, .minSpeed=80});
      chassis.waitUntilDone();
      move_forward(-24, 3000);
}