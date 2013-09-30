#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "GLHelper.h"
#include "Square.h"
#include "Checker.h"
//.#include "CellData.h"
struct cell{
	int id;
};


class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &colorBuffer);
			glDeleteBuffers(1, &identityBuffer);
		}
	}

	void init()
	{
		initialized = true;
		model = Square();
		m2 = Checker();
		clk.Reset();

		// glew will help us use GL functions, so set it up here
		setupGlew();

		//setup all shader parts
		setupShader();

		//setup buffers
		setupBuffers();
	}

	void display(int **board, bool cheatMode, bool pickingEnabled=false)
	{
		GLfloat currentTime;

		//clear the old frame
		glClearColor(.0f, .5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//use shader
		glUseProgram(redProg);

		//upload uniform
		currentTime = clk.GetElapsedTime();
		glUniform1f(timeSlot, currentTime);
		//setup position attribute

		if(pickingEnabled)
			glUniform1i(pickingSlot, GL_TRUE);
		else
			glUniform1i(pickingSlot, GL_FALSE);
		if(cheatMode)
			glUniform1i(colorIDSlot, -2);
		else
			glUniform1i(colorIDSlot, 0);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//color attribute
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//identity attribute
		glBindBuffer(GL_ARRAY_BUFFER, identityBuffer);
		glEnableVertexAttribArray(identitySlot);
		glVertexAttribIPointer(identitySlot, 1, GL_UNSIGNED_INT, 0, 0);

		//draw model
		for(int i = 0; i < model.getVertexCount(); i=i+4){
			glDrawArrays(GL_TRIANGLE_FAN, i, 4);
		}
		if(!pickingEnabled){

			for(int s=0; s<7; s++){
				for(int t = 0; t<7;t++){
				//h = CellData(s,t);
				h.id = s+2;
				cel[s][t]=h;
				}
			}
			cel[0][0].id=3;
			for(int w=0; w<8; w++){
				for(int v=0;v<8;v++){
					int q = board[w][v];
					if(q==0&&cheatMode)
						glUniform1i(colorIDSlot, -2);
					else						
						glUniform1i(colorIDSlot, q);
					glUniform1f(transSlotX, (w*.25)-.875);
					glUniform1f(transSlotY, (v*.25)-.875);
					glDrawArrays(GL_TRIANGLE_FAN, model.getVertexCount(), 362);
				}

			}
		}
		glUniform1f(colorIDSlot, 0);
		glUniform1f(transSlotX, 0);
		glUniform1f(transSlotY, 0);
		//cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		glDisableVertexAttribArray(positionSlot);
		glDisableVertexAttribArray(colorSlot);
		glDisableVertexAttribArray(identitySlot);

		glUseProgram(0);
	}

private:
	Square model;
	Checker m2;
	bool initialized;

	GLuint colorfulProg;
	GLuint redProg;

	GLuint positionBuffer;
	GLuint colorBuffer;
	GLuint identityBuffer;

	GLint positionSlot;
	GLint colorSlot;
	GLint identitySlot;
	GLint timeSlot;
	GLint transSlotX;
	GLint transSlotY;
	GLint colorIDSlot;
	GLint pickingSlot;
	cell cel[8][8];
	cell h;

	sf::Clock clk;

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
		char const * redPath = "Shaders/alwaysRed.frag";

		redProg = ShaderManager::shaderFromFile(&vertPath, &redPath, 1, 1);

		// Find out where the shader expects the data
		positionSlot = glGetAttribLocation(redProg, "pos");
		colorSlot =    glGetAttribLocation(redProg, "colorIn");
		identitySlot = glGetAttribLocation(redProg, "id");
		timeSlot =    glGetUniformLocation(redProg, "timeIn");
		transSlotX = glGetUniformLocation(redProg, "transInX");
		transSlotY = glGetUniformLocation(redProg, "transInY");
		colorIDSlot = glGetUniformLocation(redProg, "colorID");
		pickingSlot = glGetUniformLocation(redProg, "pickingEnabled");

		checkGLError("shader");
	}

	void setupBuffers()
	{
		vector<GLfloat> all_positions;
		all_positions.reserve(sizeof(model.positions) + sizeof(m2.positions));
		all_positions.insert( all_positions.end(), model.positions.begin(), model.positions.end());
		all_positions.insert( all_positions.end(), m2.positions.begin(), m2.positions.end());

		vector<GLfloat> all_colors;
		all_colors.reserve(sizeof(model.colors) + sizeof(m2.colors));
		all_colors.insert( all_colors.end(), model.colors.begin(), model.colors.end());
		all_colors.insert( all_colors.end(), m2.colors.begin(), m2.colors.end());

		//setup board position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes()+m2.getPositionBytes(), &all_positions[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Do the same thing for the color data
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getColorBytes()+m2.getColorBytes(), &all_colors[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//setup identity buffer
		glGenBuffers(1, &identityBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, identityBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getIdentityBytes(),&model.identity[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		checkGLError("setup");
	}
};


#endif