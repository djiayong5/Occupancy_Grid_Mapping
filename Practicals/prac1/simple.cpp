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
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

#define PGAIN 0.5

int main(int argc, char *argv[]) {	
	PlayerClient robot("localhost");
	RangerProxy  rp(&robot,0);
	Position2dProxy pp(&robot,0);

	pp.SetMotorEnable(true);

	for(;;)	{
		double turnrate, speed;

		// Read from the proxies
		robot.Read();

		// Print out sonars for fun
		std::cout << rp << std::endl;

		// Do simple collision avoidance
		if((rp[0] + rp[1]) < (rp[6] + rp[7]))
			turnrate = dtor(-20); // Turn 20 degrees per second
		else
			turnrate = dtor(20);

		if(rp[3] < 0.500)
			speed = 0;
		else
			speed = 0.100;

		// Command the motors
		pp.SetSpeed(speed, turnrate);

	}
}

double turn(Position2dProxy pp, double turn) {
	turn = (dtor(90) - pp.getYaw()) x PGAIN;
	return turn;
}

double move(Position2dProxy pp) {
	double current_x;
	double current_y;
	double new_x;
	double new_y;
	double speed;

	current_x = pp.getXCoord();
	current_y = pp.getYCoord();
	return speed;
}
