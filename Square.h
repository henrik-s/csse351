#ifndef __SQUARE
#define __SQUARE

#include "glew/glew.h"
#include <vector>
using namespace std; //makes using vectors easy

class Square
{
public:

	Square()
	{
		//triangle 1
		int rowsize = 4;
		int colsize = 8;
		int ox = -1;
		int oy = -1;
		float sqwidth = .25;
		int id = 1;
		for(int x = 0; x < rowsize; x++){
			for(int y = 0; y < colsize; y++){
				float offset = .25;
				if(y%2 != 0){
					offset = 0;
				}

				positions.push_back(ox+offset+x*2*sqwidth);
				positions.push_back(oy+y*sqwidth);

				positions.push_back(ox+offset+x*2*sqwidth+sqwidth);
				positions.push_back(oy+y*sqwidth);

				positions.push_back(ox+offset+x*2*sqwidth+sqwidth);
				positions.push_back(oy+y*sqwidth+sqwidth);

				positions.push_back(ox+offset+x*2*sqwidth);
				positions.push_back(oy+y*sqwidth+sqwidth);

				//triangle 2


				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(0.3f);

				colors.push_back(1.0f);
				colors.push_back(0.0f);
				colors.push_back(0.8f);

				colors.push_back(0.0f);
				colors.push_back(1.0f);
				colors.push_back(0.8f);

				colors.push_back(0.2f);
				colors.push_back(0.1f);
				colors.push_back(1.0f);

				identity.push_back(id);
				identity.push_back(id);
				identity.push_back(id);
				identity.push_back(id);

				id++;
			}
		}
	}

	vector<GLfloat> betterWay() {
		return positions;
	}

	GLfloat const * getPosition() const
	{ return &positions[0]; }

	GLfloat const * getColor() const
	{ return &colors[0]; }

	size_t getVertexCount() const
	{ return positions.size()/2; }

	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }

	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }

	size_t getIdentityBytes() const
	{ return identity.size()*sizeof(int);}
	vector<GLfloat> positions;
	vector<GLfloat> colors;
	vector<int> identity;
	size_t objectCount;
private:
	
};

#endif