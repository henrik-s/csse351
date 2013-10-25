#ifndef __WALLH
#define __WALLH

#include "glew/glew.h"
#include <math.h>
#include <vector>
#include "Maze.h"
using namespace std; //makes using vectors easy
#define PI 3.14159265 // scott says 'sscccchhhh' about this one

class WallH
{
public:

	WallH()
	{		
	}
	WallH(Maze const & maze){
		size_t h = maze.getHeight();
		size_t w = maze.getWidth();
		float thick = .3;
		float wallWidth = 1+thick;
		float wallHeight = .5;
		//bottom
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(wallWidth);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(wallWidth);
		positions.push_back(thick);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(thick);
		positions.push_back(0);
		//top
		positions.push_back(0);
		positions.push_back(thick);
		positions.push_back(wallHeight);

		positions.push_back(wallWidth);
		positions.push_back(thick);
		positions.push_back(wallHeight);
		
		
		positions.push_back(wallWidth);
		positions.push_back(0);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(wallHeight);

		//wall right
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(wallWidth);
		positions.push_back(0);
		positions.push_back(0);

		//front
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(thick);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(thick);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(wallHeight);

		//back
		positions.push_back(wallWidth);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(wallWidth);
		positions.push_back(thick);
		positions.push_back(0);

		positions.push_back(wallWidth);
		positions.push_back(thick);
		positions.push_back(wallHeight);

		positions.push_back(wallWidth);
		positions.push_back(0);
		positions.push_back(wallHeight);

		



		
	}

	GLfloat const * getPosition() const
	{ return &positions[0]; }

	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }

	
	vector<GLfloat> positions;
	size_t objectCount;
	
private:
};

#endif