/*
 * Copyright (c) 2005, Brad Kratochvil, Toby Collett, Brian Gerkey, Andrew Howard, ...
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *           this list of conditions and the following disclaimer.
 *               * Redistributions in binary form must reproduce the above copyright notice,
 *                     this list of conditions and the following disclaimer in the documentation
 *                           and/or other materials provided with the distribution.
 *                               * Neither the name of the Player Project nor the names of its contributors
 *                                     may be used to endorse or promote products derived from this software
 *                                           without specific prior written permission.
 *
 *                                           THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *                                           ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *                                           WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *                                           DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *                                           ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *                                           (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *                                           LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *                                           ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *                                           (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *                                           SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *                                           */

#include <iostream>
#include <time.h>
#include <libplayerc++/playerc++.h>

#define PGAIN 0.5
#define CELLWIDTH 0.6
#define ERRORBOUND 0.05

//void spinOnTheSpot();
//double calculateSpeed(Position2dProxy pp, double target_x_coord, double target_y_coord);

int main(int argc, char *argv[]) {
    using namespace PlayerCc;
    using namespace std;

    PlayerClient robot("localhost");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    clock_t clock;
    double currentSecs;
    double lastSecs;
    double timeDifference;
    double turnRate = 0.00;
    double speed;
    double lastSpeed;
    double currentY;
    double currentX;
    double distance;
    bool travelledDistance = false;

    pp.SetMotorEnable(true);
    pp.SetSpeed(CELLWIDTH, 0.000);

    while (travelledDistance == false) {
        clock = clock();
        lastSecs = currentSecs;
        currentSecs = (double) clock / CLOCKS_PER_SEC;
        timeDifference = lastSecs - currentSecs;

        //Print out second values.
        cout << "Current Secs: " << currentSecs << endl;
        cout << "Last Secs: " << lastSecs << endl;
        cout << "Time Difference: " << timeDifference << endl;


        /* Read from proxies. */
        robot.Read();
        /* Get robot's current x and y coordinates. */
        currentY = pp.GetYPos();
        cout << "Current Y : " << currentY << endl;
        currentX = pp.GetXPos();
        cout << "Current X : " << currentX << endl;

        if (distance <= (CELLWIDTH + ERRORBOUND) && distance >= (CELLWIDTH - ERRORBOUND)) {
            speed = 0.000;
            lastSpeed = 0.000;
            distance = 0.000;
            turnRate = 0.000;
            travelledDistance = true;
        } else {
            lastSpeed = speed;
            distance += lastSpeed * timeDifference; //Speed in m/sec, time in sec.
            speed = CELLWIDTH - distance * PGAIN;
            turnRate = 0.000;
        }
        
        pp.SetSpeed(speed, turnRate);
    }
}

/*for (;;) {
    robot.Read();
    double currentYaw = rtod(pp.GetYaw());
    std::cout << currentYaw << std::endl;
} */
}

/*void spinOnTheSpot() {
          for(;;){
        pp.SetSpeed(0, 20);
        }
} */

/*double calculateSpeed(Position2dProxy pp, double targetY, double targetX) {
        double currentX = pp.GetXPos();
        double currentY = pp.GetYPos();
        double yChange = targetY - currentY;
        double xChange = targetX - currentX;
        double speed;

        xChange = xChange * xChange;
        yChange = yChange * yChange;
        return sqrt(xChange + yChange) * PGAIN;		
} */

/*for(;;)
        {
                double turnrate, speed;

                //Read from the proxies
                robot.Read();

                //Print out sonar readings
                std::cout << sp << std::endl;

                //Do simple collision avoidance
                if((sp[0] + sp[1]) < (sp[6] + sp[7]))
                        turnrate = dtor(-20); //Turn 20 degrees per delta time
                else
                        turnrate = dtor(20);

                if(sp[3] < 0.500)
                        speed = 0;
                else
                        speed = 0.100;

                //Command the motors
                pp.SetSpeed(speed, turnrate);

        }*/