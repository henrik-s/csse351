#ifndef __CHECKER
#define __CHECKER

#include "glew/glew.h"
#include <math.h>
#include <vector>
using namespace std; //makes using vectors easy
#define PI 3.14159265
/*
This class gives us a position array for a single checker
centered at the origin.
**/

class Checker
{
public:

	Checker()
	{
		//Here we have the middle point
		positions.push_back(0);
		positions.push_back(0);
		//then we go in a circle around the middle point
		for (double i = 0; i < 2*PI;i=i+PI/180){
			positions.push_back(cos(i)/8);
			positions.push_back(sin(i)/8);
			
		}

	}
	//gets the pointer for the start of position array
	GLfloat const * getPosition() const
	{ return &positions[0]; }
	//gets the amount of bytes in the pos array
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	vector<GLfloat> positions;
	
private:
};

#endif