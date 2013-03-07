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

#define MOVEPGAIN 0.4
#define TURNPGAIN 0.5
#define CELLWIDTH 0.6
#define ERRORBOUND 0.05

int main(int argc, char *argv[]) {
    using namespace PlayerCc;
    using namespace std;

    PlayerClient robot("localhost");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    time_t currentTime = time(NULL);
    time_t lastTime = time(NULL);
    double timeDifference = 0.000;
    double speed = 0.000;
    double lastSpeed = 0.000;
    double distance = 0.000;
    bool travelledDistance = false;

    while (travelledDistance == false) {
        lastTime = currentTime;
        currentTime = time(NULL);
        timeDifference = difftime(currentTime, lastTime);
        //cout << "Time Difference: " << timeDifference << endl;
        printf("Time Difference: %.2f\n", timeDifference);

        if ((distance <= (CELLWIDTH + ERRORBOUND)) && (distance >= (CELLWIDTH - ERRORBOUND))) {
            speed = 0.000;
            lastSpeed = 0.000;
            distance = 0.000;
            travelledDistance = true;
            cout << "Arrived at next cell." << endl;
        } else {
            lastSpeed = speed;
            distance += lastSpeed * timeDifference; //Speed in m/sec, time in sec.
            speed = CELLWIDTH - distance * MOVEPGAIN;
        }

        pp.SetSpeed(speed, 0.000);
    }
}