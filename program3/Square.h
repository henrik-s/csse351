#ifndef __SQUARE
#define __SQUARE

#include "glew/glew.h"
#include <vector>
using namespace std; //makes using vectors easy
/**
Makes the board in the position arrays
*/
class Square
{
public:

	Square()
	{
		int rowsize = 4;
		int colsize = 8;
		//origin should be in bottom left
		int ox = -1;
		int oy = -1;
		float sqwidth = .25;
		int id = 1;

		//creates positions for each square
		for(int x = 0; x < rowsize; x++){
			for(int y = 0; y < colsize; y++){
				float offset = .25;
				if(y%2 != 0){
					offset = 0;
				}
				
				//bottom left vertex
				positions.push_back(ox+offset+x*2*sqwidth);
				positions.push_back(oy+y*sqwidth);

				//bottom right vertex
				positions.push_back(ox+offset+x*2*sqwidth+sqwidth);
				positions.push_back(oy+y*sqwidth);
				
				//top right
				positions.push_back(ox+offset+x*2*sqwidth+sqwidth);
				positions.push_back(oy+y*sqwidth+sqwidth);

				//top left
				positions.push_back(ox+offset+x*2*sqwidth);
				positions.push_back(oy+y*sqwidth+sqwidth);

				//pushes an identity for each vertex
				identity.push_back(id);
				identity.push_back(id);
				identity.push_back(id);
				identity.push_back(id);

				id++;
			}
		}
	}
	//points to position array
	GLfloat const * getPosition() const
	{ return &positions[0]; }
	//gets number of vertexes
	size_t getVertexCount() const
	{ return positions.size()/2; }
	//number of bytes in position array
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	//number of bytes in identity array
	size_t getIdentityBytes() const
	{ return identity.size()*sizeof(int);}
	vector<GLfloat> positions;
	vector<int> identity;
private:
	
};

#endif