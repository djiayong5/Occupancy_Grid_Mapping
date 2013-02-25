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

#define PGAIN 0.5

//void spinOnTheSpot();
//double calculateSpeed(Position2dProxy pp, double target_x_coord, double target_y_coord);

int main(int argc, char *argv[])
{
	using namespace PlayerCc;

	PlayerClient    robot("lisa.islnet");
	SonarProxy      sp(&robot,0);
	Position2dProxy pp(&robot,0);

	pp.SetMotorEnable(true);

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
	
	robot.Read();
	
	for(;;){
	  robot.Read();
	  double yaw = rtod(pp.GetYaw());
	  std::cout << yaw<< std::endl;
	}
}

/*void spinOnTheSpot() {
	  for(;;){
	pp.SetSpeed(0, 20);
	}
} */

/*double calculateSpeed(Position2dProxy pp, double target_x_coord, double target_y_coord) {
	double current_x_coord = pp.GetXPos();
	double current_y_coord = pp.GetYPos();
	double delta_x_change = target_x_coord - current_x_coord;
	double delta_y_change = target_y_coord - current_y_coord;
	double speed;

	delta_x_change = delta_x_change * delta_x_change;
	delta_y_change = delta_y_change * delta_y_change;
	return sqrt(delta_x_change + delta_y_change) * PGAIN;		
} */
