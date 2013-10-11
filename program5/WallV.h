#ifndef __WALLV
#define __WALLV

#include "glew/glew.h"
#include <math.h>
#include <vector>
#include "Maze.h"
using namespace std; //makes using vectors easy
#define PI 3.14159265

class WallV
{
public:

	WallV()
	{		
	}
	WallV(Maze const & maze){
		size_t h = maze.getHeight();
		size_t w = maze.getWidth();
		float thick = .3;
		float wallWidth = 1+thick;
		float wallHeight = .5;
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(0);
		//top
		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);

		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);
		
		
		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(wallHeight);

		//wall right
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(0);

		//front
		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(wallHeight);

		//back
		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(0);

		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);

		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(w);
		positions.push_back(0);
		positions.push_back(0);

		positions.push_back(w);
		positions.push_back(h);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(h);
		positions.push_back(0);



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