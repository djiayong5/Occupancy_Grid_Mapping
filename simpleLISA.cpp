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

#define MOVEPGAIN 0.1
#define TURNPGAIN 0.5
#define CELLWIDTH 0.6
#define ERRORBOUND 0.05

double calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * TURNPGAIN;
    return turnRate;
}

int main(int argc, char *argv[]) {
    using namespace PlayerCc;
    using namespace std;

    PlayerClient robot("localhost");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    time_t currentTime;
    time_t lastTime;
    double timeDifference = 0.000;
    double speed = 0.000;
    double lastSpeed = 0.000;
    double distance = 0.000;
    bool travelledDistance = false;

    bool yawAchieved = false;
    double currentYaw;
    double turnRate = 0.00;
    double targetYaw = 0.000;

    while (yawAchieved != true) {
        robot.Read();
        currentYaw = rtod(pp.GetYaw());
        
        if (currentYaw >= (targetYaw - ERRORBOUND) && currentYaw <= (targetYaw + ERRORBOUND)) {
            turnRate = 0.000;
            yawAchieved = true;
        }
        else turnRate = dtor(calculateTurnRate(currentYaw, targetYaw));

        pp.SetSpeed(0.000, turnRate);
    }
    
    currentTime = time(NULL);
    
    while (travelledDistance == false) {
        lastTime = currentTime;
        cout << "Last Time: " << lastTime << endl;
        currentTime = time(NULL);
        cout << "Current Time: " << currentTime << endl;
        timeDifference = difftime(currentTime, lastTime);
        cout << "Time Difference: " << timeDifference << endl;

        if ((distance <= (CELLWIDTH + ERRORBOUND)) && (distance >= (CELLWIDTH - ERRORBOUND))) {
            speed = 0.000;
            lastSpeed = 0.000;
            distance = 0.000;
            travelledDistance = true;
            cout << "Arrived at next cell." << endl;
        } else {
            lastSpeed = speed;
            distance += lastSpeed * timeDifference; //Speed in m/sec, time in sec.
            cout << "Distance Travelled So Far: " << distance << endl;
            speed = (CELLWIDTH - distance) * MOVEPGAIN;
            cout << "Speed: " << speed << endl << endl;
        }

        pp.SetSpeed(speed, 0.000);
    }
}