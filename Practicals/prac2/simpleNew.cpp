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

using namespace PlayerCc;

double move(double distance, Position2dProxy & p);
double turn(double angle, Position2dProxy & p);

int main(int argc, char *argv[])
{
	
	PlayerClient robot("localhost");
	//PlayerClient robot("lisa.islnet"); //For use with Pioneer.
	RangerProxy rp(&robot,0); // '0' refers to an array of specific type of sensor, '1' could be an array of infra-red sensors.
	//SonarProxy rp(&robot,0); //For use with Pioneer.
	Position2dProxy pp(&robot,0);

	pp.SetMotorEnable(true);
	double turnrate;
	double speed;
	
	/* for(;;) {
		double speed;

		//read from the proxies
		robot.Read();

		//print out sonars for fun
		std::cout << rp << std::endl;

		//do simple collision avoidance
			if((rp[0] + rp[1]) < (rp[6] + rp[7]))
			turnrate = dtor(-20); //turn 20 degrees per second
		
		else
			turnrate = dtor(20);

			if(rp[3] < 0.500)
				speed = 0;
				else
					speed = 0.30;

			//command the motors
			
		speed = 0;
		
		turnrate = turn(90, pp);
		pp.SetSpeed(speed, turnrate);

	} */
	
	bool task_finished = false;
	double targetX = p.GetXPos() + 10;
	double targetY = p.GetYPos();
	
	while (!task_finished) {
	  robot.Read(); //Get proxy results
	  speed = move()
	  pp.SetSpeed(, turnrate);
	} 

double approachRate (Position2dProxy &p, double targetX, double targetY) {  
  double xChange = targetX - p.GetXPos();
  double yChange = targetY - p.GetYPos();
  targetPos - currentPos * PGAIN;
}

double move(double distance, Position2dProxy & p) {

  double angle = p.GetYaw();

  return distance - PGAIN;
}

double turn(double angle, Position2dProxy & p) {
  double current_angle = p.GetYaw();

  return dtor((angle - current_angle) * PGAIN);
} 





