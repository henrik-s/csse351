#ifndef __RENDERENGINE
#define __RENDERENGINE
#define RESOLUTION 512

#define PI 3.14.f

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include <stdlib.h>
#include <cmath>
#include "MazeModel.h"
#include "WallH.h"
#include "WallV.h"
class Position {
public:
	int x;
	int y;
	Position(){}
	Position(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void printPos() {
		printf("Position: (%d, %d)\n", x, y);
	}
private:
};
class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		glm::vec3 e, c, u, axis;
		e = glm::vec3(-10,5,0.3);		// Specifies the position of the eye point.
		c = glm::vec3(0,5,0.3);			// Specifies the position of the reference point.
		u = glm::vec3(0,0,1);			// Specifies the direction of the up vector.
		this->C = glm::lookAt(e, c, u);
		this->M = glm::mat4(1);
		direction = 'N';
		mapToggled = false;
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteVertexArrays(1, &vertexArray);
		}
	}

	void init(unsigned int const & w, unsigned int const & h)
	{
		this->w = w;
		this->h = h;
		movement = false;
		clk.Reset();
		setupGlew();
		setupShader();
		generateMaze();
		initialized = true;
	}

	/**
	Save projection, camera and transition matrix and swap
	*/
	void toggleMap() {
		// Prevent interruptiong current movement
		if (xMove > 0 || yMove > 0 || zMove > 0 || turning > 0)
			return;
		if (!mapToggled) {
			this->Pswap = P;
			this->Cswap = C;
			this->Mswap = M;
			directionSwap = direction;
			glm::vec3 e, c, u, axis;
			e = glm::vec3(11/2,11/2,20);
			c = glm::vec3(11/2,11/2,0);
			u = glm::vec3(1,0,0);
			this->C = glm::lookAt(e, c, u);
			this->M = glm::mat4(1);			
			this->P = glm::perspective(60.0f, (float)RESOLUTION/(float)RESOLUTION, 0.1f, 200.0f);
			direction = 'N';
			mapToggled = true;
		}
		else {			
			this->P = Pswap;
			this->C = Cswap;
			this->M = Mswap;			
			direction = directionSwap;
			mapToggled = false;
		}
	}

	/**
	Print help text
	*/
	void printHelp() {
		printf("\n*** Keys ***\n");
		printf("W = Forward\n");
		printf("A = Strafe left\n");
		printf("S = Backward\n");
		printf("D = Strafe right\n");
		printf("Q = Turn smoothly left\n");
		printf("E = Turn smoothly right\n");
		printf("\n");
		printf("Spacebar = Generate new maze\n");
		printf("H = Display this help message\n");
		printf("M = Toggle on/off map\n");
		printf("\n");
		printf("--- Map mode ---\n");
		printf("Move around using W,A,S,D\n");
		printf("Q = Zoom out\n");
		printf("E = Zoom in\n");

		
	}

	void display(bool pickingEnabled=false)
	{
		glEnable(GL_DEPTH_TEST);

		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//use shader
		glUseProgram(shaderProg);

		glm::mat4 T = P*C*M;

		updateMovement();
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
		
		float t = clk.GetElapsedTime();
		glUniform1f(timeSlot, t);
		//draw
		glBindVertexArray(vertexArray);

		//upload uniforms for the color of the floor
		glUniform1f(rSlot, 1);
		glUniform1f(gSlot, 1);
		glUniform1f(bSlot, 1);
		
		//glDrawElements(GL_LINES, model.getElementCount(), GL_UNSIGNED_INT, 0);
		int v = model.getVertexCount();
		//draw floor
		glDrawArrays(GL_TRIANGLE_FAN,v+48,4);

		//this draws the horizontal walls
		for(int i = 0; i < model.getElementCount(); i+=2){
			if(model.elements[i]-model.elements[i+1]==1||model.elements[i]-model.elements[i+1]==-1){
				int e = model.elements[i];
				//colors
				glUniform1f(rSlot, model.positions[3*e]/sqrt((double) model.getElementCount())+.1);
				glUniform1f(gSlot, model.positions[3*e+1]/sqrt((double) model.getElementCount()));
				glUniform1f(bSlot, model.positions[3*e+2]/sqrt((double) model.getElementCount()));
				//location of wall
				glUniform1f(transSlotX, model.positions[3*e]-.05);
				glUniform1f(transSlotY, model.positions[3*e+1]);
				//bottom
				glDrawArrays(GL_TRIANGLE_FAN,v,4);
				//right
				glDrawArrays(GL_TRIANGLE_FAN,v+4,4);
				//top
				glDrawArrays(GL_TRIANGLE_FAN,v+8,4);
				//left
				glDrawArrays(GL_TRIANGLE_FAN,v+12,4);
				//front
				glDrawArrays(GL_TRIANGLE_FAN,v+16,4);
				//back
				glDrawArrays(GL_TRIANGLE_FAN,v+20,4);
			}
		}
		//this drawas the vertical walls
		v = model.getVertexCount()+24;
		for(int i = 0; i < model.getElementCount(); i+=2){
			if(model.elements[i]-model.elements[i+1]!=1&&model.elements[i]-model.elements[i+1]!=-1){
				int e = model.elements[i];
				 //colors
				glUniform1f(rSlot, model.positions[3*e+2]/sqrt((double) model.getElementCount()));
				glUniform1f(gSlot, model.positions[3*e+1]/sqrt((double) model.getElementCount()));
				glUniform1f(bSlot,model.positions[3*e]/sqrt((double) model.getElementCount())+.1);
				//location of wall
				glUniform1f(transSlotX, model.positions[3*e]-.05);
				glUniform1f(transSlotY, model.positions[3*e+1]);
				//bottom
				glDrawArrays(GL_TRIANGLE_FAN,v,4);
				//right
				glDrawArrays(GL_TRIANGLE_FAN,v+4,4);
				//top
				glDrawArrays(GL_TRIANGLE_FAN,v+8,4);
				//left
				glDrawArrays(GL_TRIANGLE_FAN,v+12,4);
				//front
				glDrawArrays(GL_TRIANGLE_FAN,v+16,4);
				//back
				glDrawArrays(GL_TRIANGLE_FAN,v+20,4);
			}
		}
		//cleanup
		if(rotation>0)
			rotation = rotation - 0.1;
		glUniform1f(transSlotX, 0);
		glUniform1f(transSlotY, 0);
		glUniform1f(rSlot,0);
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("display");
	}

	/**
	Call the specific move method for a specific key
	*/
	void move(char dir) {
		movement = true;
		// Prevent interruptiong current movement
		if (xMove > 0 || yMove > 0 || zMove > 0 || turning > 0)
			return;
		switch(dir) {
			case 'w':
				wMove();
				break;
			case 's':
				sMove();
				break;
			case 'e':
				turn(false);
				break;
			case 'q':
				turn(true);
				break;
			case 'd':
				dMove();
				break;
			case 'a':
				aMove();
				break;
			}
	}

	void tryMove(char direction) {
		switch (direction) {
			case 'T':
				if (!legalMove(pos.x, pos.y, 'T'))
					return;			
				if (!mapToggled)
					pos.y++;
				yMove = 10;
				yChange = -0.1;
				break;
			case 'B':	
				if (!legalMove(pos.x, pos.y, 'B'))
					return;				
				if (!mapToggled)
					pos.y--;
				yMove = 10;
				yChange = 0.1;
				break;
			case 'R':		
				if (!legalMove(pos.x, pos.y, 'R'))
					return;			
				if (!mapToggled)
					pos.x++;
				xMove = 10;
				xChange = -0.1;
				break;
			case 'L':	
				if (!legalMove(pos.x, pos.y, 'L'))
					return;				
				if (!mapToggled)
					pos.x--;
				xMove = 10;
				xChange = 0.1;
				break;
		}
	}

	/**
	Try move up, take into consideration the direction the player is facing
	*/
	void wMove() {
		switch(direction) {
			case 'N':
				tryMove('R');
				break;
			case 'S':
				tryMove('L');
				break;
			case 'W':
				tryMove('T');
				break;
			case 'E':				
				tryMove('B');
				break;
			}
	}

	/**
	Try move down, take into consideration the direction the player is facing
	*/
	void sMove() {
		switch(direction) {
			case 'N':
				tryMove('L');
				break;
			case 'S':
				tryMove('R');
				break;
			case 'W':
				tryMove('B');
				break;
			case 'E':
				tryMove('T');
				break;
			}
	}

	/**
	Try strafe left, take into consideration the direction the player is facing
	*/
	void aMove() {
		switch(direction) {
			case 'N':
				tryMove('T');
				break;
			case 'S':
				tryMove('B');
				break;
			case 'W':
				tryMove('L');
				break;
			case 'E':
				tryMove('R');
				break;
			}
	}

	/**
	Try strafe right, take into consideration the direction the player is facing
	*/
	void dMove() {
		switch(direction) {
			case 'N':
				tryMove('B');
				break;
			case 'S':
				tryMove('T');
				break;
			case 'W':
				tryMove('R');
				break;
			case 'E':
				tryMove('L');
				break;
			}
	}

	/**
	Turn 90 degrees left or right, and update the direction
	*/
	void turn(bool leftTurn) {
		turningLeft = leftTurn;
		if (mapToggled) {
			if (!turningLeft) {			
				zMove = 10;
				zChange = 0.1;
			}
			else {		
				zMove = 10;
				zChange = -0.1;			
			}
			return;
		}
		if (!turningLeft)
			turnRight();
		else
			turnLeft();

		// Turn smoothly by rotating the camera 15 times
		turning = 15;
	}

	/**
	Update direction upon left rotation
	*/
	void turnLeft() {
		if (direction == 'N')
			direction = 'W';
		else if (direction == 'W')
			direction = 'S';
		else if (direction == 'S')
			direction = 'E';
		else if (direction == 'E')
			direction = 'N';
	}
	/**
	Update direction upon left rotation
	*/
	void turnRight() {
		if (direction == 'N')
			direction = 'E';
		else if (direction == 'E')
			direction = 'S';
		else if (direction == 'S')
			direction = 'W';
		else if (direction == 'W')
			direction = 'N';
	}

	/**
	Create a smooth movement in x, y, or z axis
	*/
	void updateMovement() {
		if (!movement)
			return;
		if (xMove > 0) {
			M[3][0] += xChange;
			xMove--;
		}
		if (yMove > 0) {
			M[3][1] += yChange;
			yMove--;
		}		
		if (zMove > 0) {
			M[3][2] += zChange;
			zMove--;
		}
		if (turning > 0) {
			updateTurn();
			turning--;
		}
	}

	// Turn smoothly
	void updateTurn() {
		float angle;
		if (turningLeft)
			angle = -PI/30;
		else
			angle = PI/30;
		this->R = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		R[0] = glm::vec4(cos(angle), 0, -sin(angle), 0);
		R[2] = glm::vec4(sin(angle), 0, cos(angle), 0);
		this->P = this->P*R;
	}
	
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}


	// Check if a move is legal or not by inspecting the cell variable
	// accessed in Maze.
	bool legalMove(int x, int y, char direction, bool fromOutside = false) {
		if (mapToggled)
			return true;
		if (y < 0 || y >= maze.getHeight() || x < 0 || x >= maze.getWidth()) {
			if (fromOutside)		// If try to move to outside position from outside position
				return true;
			else {				// 1-recursion to check if there is a wall
				char wallHack;
				if (direction == 'T') {
					wallHack = 'B';
					y++;
				}
				else if (direction == 'B') {
					wallHack = 'T';
					y--;
				}
				else if (direction == 'R') {
					wallHack = 'L';
					x++;
				}
				else {
					wallHack = 'R';
					x--;
				}
				return legalMove(x, y, wallHack, true);
			}
		}
		if (maze.bottomBlocked(x, y) && direction == 'B')
			return false;
		if (maze.topBlocked(x, y) && direction == 'T')
			return false;
		if (maze.leftBlocked(x, y) && direction == 'L')
			return false;
		if (maze.rightBlocked(x, y) && direction == 'R')
			return false;
		return true;
	}
	
	void generateMaze(unsigned int const & seed = 300)
	{
		maze = Maze(w, h, seed);
		unsigned int x,y,s,t;
		bool retry = false;
		maze.getLeftOpening(x,y);
		maze.getRightOpening(s,t);
		if (x == 0 && y == 0)
			retry = true;
		model = MazeModel(maze);
		wallHModel = WallH(maze);
		wallVModel = WallV(maze);
		if(initialized)
			rebuildBuffers();
		else {
			glm::vec3 e, c, u, axis;
			this->P = glm::perspective(60.0f, (float)RESOLUTION/(float)RESOLUTION, 0.1f, 200.0f);
			e = glm::vec3((float)x-1.33,(float)y+0.66,0.3);			// Specifies the position of the eye point.
			c = glm::vec3(s,(float)y+0.66,0.3);						// Specifies the position of the reference point.
			u = glm::vec3(0,0,1);									// Specifies the direction of the up vector.
			this->C = glm::lookAt(e, c, u);
			pos = Position(x-2, y);
			setupBuffers();
		}
		if (retry)
			generateMaze(2);
	}

private:
	MazeModel model;
	Maze maze;
	WallH wallHModel;
	WallV wallVModel;
	bool initialized, turningLeft;

	Position pos, startPos, endPos;
	float xChange, yChange, zChange;
	int xMove, yMove, zMove, turning;
	bool mapToggled, movement;
	char direction, directionSwap;

	GLuint shaderProg;

	GLfloat rotation;

	GLuint positionBuffer;
	GLuint normalBuffer;
	GLuint elementBuffer;
	GLuint vertexArray;

	GLint positionSlot;
	GLint normalSlot;
	GLint timeSlot;
	GLint matSlot;
	GLint transSlotX;
	GLint transSlotY;
	GLint rSlot;
	GLint gSlot;
	GLint bSlot;

	sf::Clock clk;
	
	unsigned int w;
	unsigned int h;
	
	glm::mat4 P;
	glm::mat4 R;
	glm::mat4 C;
	glm::mat4 M;

	glm::mat4 Pswap;
	glm::mat4 Cswap;
	glm::mat4 Mswap;

	void setupGlew()
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(1);
		}
		printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}

	void setupShader()
	{
		char const * vertPath = "Shaders/simple.vert";
		char const * fragPath = "Shaders/simple.frag";

		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);

		// Find out where the shader expects the data
		positionSlot = glGetAttribLocation(shaderProg, "pos");
		normalSlot = glGetAttribLocation(shaderProg, "norm");
		timeSlot = glGetUniformLocation(shaderProg, "time");
		matSlot =      glGetUniformLocation(shaderProg, "M");
		transSlotX = glGetUniformLocation(shaderProg, "transInX");
		transSlotY = glGetUniformLocation(shaderProg, "transInY");
		rSlot = glGetUniformLocation(shaderProg, "r");
		gSlot = glGetUniformLocation(shaderProg, "g");
		bSlot = glGetUniformLocation(shaderProg, "b");
		
		checkGLError("shader");
	}

	void setupBuffers()
	{
		vector<GLfloat> all_positions;
		all_positions.reserve(sizeof(model.positions) + sizeof(wallHModel.positions)+ sizeof(wallVModel.positions));
		all_positions.insert( all_positions.end(), model.positions.begin(), model.positions.end());
		all_positions.insert( all_positions.end(), wallHModel.positions.begin(), wallHModel.positions.end());
		all_positions.insert( all_positions.end(), wallVModel.positions.begin(), wallVModel.positions.end());

		vector<GLfloat> all_normals;
		all_normals.reserve(sizeof(model.normals)+sizeof(wallHModel.normals)+ sizeof(wallVModel.normals));
		all_normals.insert( all_normals.end(), model.normals.begin(), model.normals.end());
		all_normals.insert( all_normals.end(), wallHModel.normals.begin(), wallHModel.normals.end());
		all_normals.insert( all_normals.end(), wallVModel.normals.begin(), wallVModel.normals.end());
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes()+wallHModel.getPositionBytes()+wallVModel.getPositionBytes(), &all_positions[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getNormalsBytes()+wallHModel.getNormalsBytes()+wallVModel.getNormalsBytes(), &all_normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getElementBytes(), model.getElements(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//setup the vertex array
		glGenVertexArrays(1, &vertexArray);
		
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glEnableVertexAttribArray(normalSlot);
		glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);
		
		checkGLError("setup");
	}
	
	void rebuildBuffers()
	{
		vector<GLfloat> all_positions;
		all_positions.reserve(sizeof(model.positions) + sizeof(wallHModel.positions)+ sizeof(wallVModel.positions));
		all_positions.insert( all_positions.end(), model.positions.begin(), model.positions.end());
		all_positions.insert( all_positions.end(), wallHModel.positions.begin(), wallHModel.positions.end());
		all_positions.insert( all_positions.end(), wallVModel.positions.begin(), wallVModel.positions.end());

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, model.getPositionBytes()+wallHModel.getPositionBytes()+wallVModel.getPositionBytes(), &all_positions[0]);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, model.getElementBytes(), model.getElements());
		
		
		glBindVertexArray(vertexArray);	
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);
		checkGLError("rebuild");
	}
};




#endif