/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

#include <stdio.h>
#include <math.h>
#include <drawstuff/drawstuff.h>
#include <ode/ode.h>


#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


void start()
{
  
  
}


void simLoop (int pause)
{
	//일단 비워두고요..
	//  
	float xyz[3],hpr[3];
  	dsGetViewpoint (xyz,hpr);
	printf("x = %f, y = %f, z = %f\n",xyz[0],xyz[1],xyz[2]);
}


void command (int cmd)
{
  dsPrint ("received command %d (`%c')\n",cmd,cmd);
  

     // 게임 인터페이스 w,a,s,d 로 이동하기!
  float xyz[3],hpr[3];
  dsGetViewpoint (xyz,hpr);
  switch (cmd) {
	case 'w' : xyz[0] += cos(hpr[0] * M_PI / 180)*0.3;
		   xyz[1] += sin(hpr[0] * M_PI / 180)*0.3; break;

	case 's' : xyz[0] -= cos(hpr[0] * M_PI / 180)*0.3;
		   xyz[1] -= sin(hpr[0] * M_PI / 180)*0.3; break;

	case 'a' : xyz[0] += -sin(hpr[0] * M_PI / 180)*0.3;
		   xyz[1] += cos(hpr[0] * M_PI / 180)*0.3; break;

	case 'd' : xyz[0] -= -sin(hpr[0] * M_PI / 180)*0.3;
		   xyz[1] -= cos(hpr[0] * M_PI / 180)*0.3; break;
        
	//우리집 컴퓨터는 마우스로 높이조절이 안되가지고..
	case 'u' : xyz[2] += 1; break;
	case 'j' : xyz[2] -= 1; break;
 }
 dsSetViewpoint (xyz,hpr);


}


int main (int argc, char **argv)
{
  // setup pointers to callback functions
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = command;
  fn.stop = 0;
  fn.path_to_textures = "./textures/";	// uses default

  //create world
  dWorldID world;
  world = dWorldCreate();
  dWorldSetGravity (world,0,0,-9.8);

  // run simulation
  dsSimulationLoop (argc,argv,400,400,&fn);


  //dWorldDestroy
  return 0;
}
