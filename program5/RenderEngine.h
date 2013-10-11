#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include <stdlib.h>
#include <cmath>
#include "MazeModel.h"
#include "WallH.h"
#include "WallV.h"

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		this->P = glm::ortho(-1, 1, -1, 1);
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

	void display(bool pickingEnabled=false)
	{
		GLfloat currentTime;
		currentTime = clk.GetElapsedTime();
		glEnable(GL_DEPTH_TEST);

		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//use shader
		glUseProgram(shaderProg);
		glm::mat4 T = P;
		updateMovement();
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
		
		//draw
		glUniform1f(timeSlot, currentTime);
		glBindVertexArray(vertexArray);
		glUniform1f(rSlot, 1);
		glUniform1f(gSlot, 1);
		glUniform1f(bSlot, 1);
		glDrawElements(GL_LINES, model.getElementCount(), GL_UNSIGNED_INT, 0);
		int v = model.getVertexCount();
		glDrawArrays(GL_TRIANGLE_FAN,v+36,4);
		for(int i = 0; i < model.getElementCount(); i+=2){
			if(model.elements[i]-model.elements[i+1]==1||model.elements[i]-model.elements[i+1]==-1){
				int e = model.elements[i];
				glUniform1f(rSlot, model.positions[3*e]/sqrt((double) model.getElementCount())+.1);
				glUniform1f(gSlot, model.positions[3*e+1]/sqrt((double) model.getElementCount()));
				glUniform1f(bSlot, model.positions[3*e+2]/sqrt((double) model.getElementCount()));
				glUniform1f(transSlotX, model.positions[3*e]-.05);
				glUniform1f(transSlotY, model.positions[3*e+1]);
				//bottom
				glDrawArrays(GL_TRIANGLE_FAN,v,4);
				//right
				glDrawArrays(GL_TRIANGLE_FAN,v+2,4);
				//top
				glDrawArrays(GL_TRIANGLE_FAN,v+4,4);
				//left
				glDrawArrays(GL_TRIANGLE_FAN,v+6,4);
				//front
				glDrawArrays(GL_TRIANGLE_FAN,v+10,4);
				//back
				glDrawArrays(GL_TRIANGLE_FAN,v+14,4);
			}
		}
		v = model.getVertexCount()+18;
		for(int i = 0; i < model.getElementCount(); i+=2){
			if(model.elements[i]-model.elements[i+1]!=1&&model.elements[i]-model.elements[i+1]!=-1){
				int e = model.elements[i];
				 
				glUniform1f(rSlot, model.positions[3*e+2]/sqrt((double) model.getElementCount()));
				glUniform1f(gSlot, model.positions[3*e+1]/sqrt((double) model.getElementCount()));
				glUniform1f(bSlot,model.positions[3*e]/sqrt((double) model.getElementCount())+.1);
				glUniform1f(transSlotX, model.positions[3*e]-.05);
				glUniform1f(transSlotY, model.positions[3*e+1]);
				//bottom
				glDrawArrays(GL_TRIANGLE_FAN,v,4);
				//right
				glDrawArrays(GL_TRIANGLE_FAN,v+2,4);
				//top
				glDrawArrays(GL_TRIANGLE_FAN,v+4,4);
				//left
				glDrawArrays(GL_TRIANGLE_FAN,v+6,4);
				//front
				glDrawArrays(GL_TRIANGLE_FAN,v+10,4);
				//back
				glDrawArrays(GL_TRIANGLE_FAN,v+14,4);
			}
		}
		//cleanup
		glUniform1f(transSlotX, 0);
		glUniform1f(transSlotY, 0);
		glUniform1f(rSlot,0);
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("display");
	}

	void move(char direction) {
		movement = true;
		switch(direction) {
			case 'l':
				xMove = 10;
				xChange = -0.01;
				break;
			case 'r':
				xMove = 10;
				xChange = 0.01;
				break;
			case 'u':
				yMove = 10;
				yChange = 0.01;
				break;
			case 'd':
				yMove = 10;
				yChange = -0.01;
				break;
			}
	}
	void updateMovement() {
		if (!movement)
			return;
		if (xMove > 0) {
			P[3][0] += xChange;
			xMove--;
		}
		if (yMove > 0) {
			P[3][2] += yChange;
			printf("%.3f\n", P[3][2]);
			yMove--;
		}


	}
	
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
	
	void generateMaze(unsigned int const & seed = 1)
	{
		Maze mazeLayout(w, h, seed);
		model = MazeModel(mazeLayout);
		wallHModel = WallH(mazeLayout);
		wallVModel = WallV(mazeLayout);
		
		this->P = glm::ortho(-model.getUnitSize(), (w+1)*model.getUnitSize(), -model.getUnitSize(), (h+1)*model.getUnitSize());
	
		if(initialized)
			rebuildBuffers();
		else
			setupBuffers();
	}

private:
	MazeModel model;
	WallH wallHModel;
	WallV wallVModel;
	bool initialized;


	float xChange, yChange, zChange;
	int xMove, yMove;
	bool movement;

	GLuint shaderProg;

	GLuint positionBuffer;
	GLuint elementBuffer;
	GLuint vertexArray;

	GLint positionSlot;
	GLint matSlot;
	GLint transSlotX;
	GLint transSlotY;
	GLint timeSlot;
	GLint rSlot;
	GLint gSlot;
	GLint bSlot;

	sf::Clock clk;
	
	unsigned int w;
	unsigned int h;
	
	glm::mat4 P;

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
		matSlot =      glGetUniformLocation(shaderProg, "M");
		transSlotX = glGetUniformLocation(shaderProg, "transInX");
		transSlotY = glGetUniformLocation(shaderProg, "transInY");
		timeSlot = glGetUniformLocation(shaderProg, "time");
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
		
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes()+wallHModel.getPositionBytes()+wallVModel.getPositionBytes(), &all_positions[0], GL_STATIC_DRAW);
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