#ifndef __CHECKER
#define __CHECKER

#include "glew/glew.h"
#include <math.h>
#include <vector>
using namespace std; //makes using vectors easy
#define PI 3.14159265

class Checker
{
public:

	Checker()
	{
				positions.push_back(0);
				positions.push_back(0);

				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(0.0f);

		for (double i = 0; i < 2*PI;i=i+PI/180){
				positions.push_back(cos(i)/8);
				positions.push_back(sin(i)/8);

				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(0.0f);

				
		}

	}

	GLfloat const * getPosition() const
	{ return &positions[0]; }

	GLfloat const * getColor() const
	{ return &colors[0]; }

	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }

	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }
	
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	size_t objectCount;
	
private:
};

#endif