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

		//left
		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(0);

		positions.push_back(0);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);

		positions.push_back(thick);
		positions.push_back(wallWidth);
		positions.push_back(wallHeight);


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
		positions.push_back(thick);
		positions.push_back(0);
		positions.push_back(wallHeight);

		positions.push_back(0);
		positions.push_back(0);
		positions.push_back(wallHeight);

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


			normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);

		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);

		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);

		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);
		//left
		normals.push_back(-1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(-1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(-1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(-1);
		normals.push_back(0);
		normals.push_back(0);
		
		//top
		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);
		
		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);

		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);

		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);

		//wall right
		normals.push_back(1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(1);
		normals.push_back(0);
		normals.push_back(0);
		
		normals.push_back(1);
		normals.push_back(0);
		normals.push_back(0);

		//front
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(-1);
		
		//back
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(1);
		
		normals.push_back(0);
		normals.push_back(0);
		normals.push_back(1);



	}

	GLfloat const * getPosition() const
	{ return &positions[0]; }

	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }

	GLfloat const * getNormals() const
	{ return &normals[0]; }

	size_t getNormalsBytes() const
	{ return normals.size()*sizeof(GLfloat); }
	
	vector<GLfloat> positions;
	vector<GLfloat> normals;
	size_t objectCount;
	
private:
};

#endif