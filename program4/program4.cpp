#define printusers() printf("Program by HARRISSA+SOHLBEHD\n");

#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include "glew/glew.h"
#include <SFML/Window.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ShaderManager.h"
#include "GLHelper.h"
#include "RenderEngine.h"
#include "TrackBall.h"

#define RESOLUTION 512
#define TARGET_FPS 30                // controls spin update rate
#define TIME_WINDOW 3                // number of frames motion is valid after release

/**
Problem:
The problem for this week is to use a trackball interface 
to translate and rotate a cube in the intuitive way. We also
need to keep the cube spinning after rotate and resize the
window with the correct aspect ratio.

Solution:
First we created a rotation matrix using glm::rotate and 
multiplied it by the current rotation transform. This keep
track of how much it has been rotated. Using spin cube, we
keep track of if the left click is down and keep the cube spinning

For the translate, we change the values of the third column
of the matrix to move it on the x, y and z axis.

For the aspect ratio, we update the projection transform
to match the size of the window and we update the trackball
size to match the size of the window.

Authors:
Scott Harris
Hendrik Sohlberg

Date:
October 4th 2013
*/
class Program4
{
public:
	Program4()
	{
		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program4");
		render.init();
		
		Cube temp;
		translateToOrigin = glm::translate(glm::mat4(1), -temp.getCentroid());
		render.setModelTransform(translateToOrigin);
		
		motionTime = .0f;
		previousPos = glm::vec2(0);
		buttonDown[0]=false; buttonDown[1]=false; buttonDown[2]=false;
		spinning = false;
		trackball.setSize(RESOLUTION, RESOLUTION);
		
		glm::mat4 P = glm::perspective(60.0f, (float)RESOLUTION/(float)RESOLUTION, 0.1f, 100.0f);
		render.setProjectionTransform(P);
		
		while (App->IsOpened())
		{			
			App->SetActive();
			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0)
				sf::Sleep(sleepTime);
			spinCube();
			render.display();
			App->Display();
			handleEvents();
		}
	}
	
	~Program4()
	{
	}
	
private:
	sf::Window *App;
	RenderEngine render;
	TrackBall trackball;
	
	sf::Clock motionClock;
	float timeSinceMotion;
	float motionTime;
	glm::ivec2 previousPos;
	bool buttonDown[3];
	bool spinning;
	
	glm::mat4 translateToOrigin;
	glm::mat4 translateFromInput;
	glm::mat4 rotationFromInput;
	glm::mat4 rotationSpinStep;
	/**
	Update Rotate uses the trackball info and computes the
	phi and axis of rotation. It then uses glm::rotate to
	calculate the rotation matrix. It then incrementally
	changes the complete rotation matrix.

	RotationFromInput keeps track of the total rotations
	*/
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{

		float phi;
		glm::vec3 axis;
		
		//computes the appropriate rotation data and stores in phi and axis
		trackball.getRotation(phi, axis, oldPos, newPos);
		//calculates rotation matrix
		rotationSpinStep = glm::rotate(glm::mat4(), phi, axis);
		//applies it to the current rotation
		rotationFromInput = rotationSpinStep*rotationFromInput;
		//applies it to the cube
		render.setModelTransform(translateFromInput*rotationFromInput*translateToOrigin);

	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{

		#define XY_SENSITIVITY 0.006f  //may be helpful to reduce transform amount
		//
		//
		// Put your code for a translation in the x,y direction here.
		//
		//
		float newXVal = newPos.x - oldPos.x;
		float newYVal = newPos.y - oldPos.y;
		translateFromInput[3][0] += newXVal*XY_SENSITIVITY;
		translateFromInput[3][1] -= newYVal*XY_SENSITIVITY;
		render.setModelTransform(translateFromInput*rotationFromInput*translateToOrigin);
	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define Z_SENSITIVITY 0.006f //may be helpful to reduce transform amount
		//
		//
		// Put your code for a translation in the z direction here.
		//
		//
		float newZVal = oldPos.y - newPos.y;
		translateFromInput[3][2] += newZVal*Z_SENSITIVITY;
		render.setModelTransform(translateFromInput*rotationFromInput*translateToOrigin);
	}
	
	void updateAspectRatio(int const & newWidth, int const & newHeight)
	{
		//
		//
		// Put your code to adjust for the change in viewport here.
		//
		//
		glm::mat4 P = glm::perspective(60.0f, (float)newWidth/(float)newHeight, 0.1f, 100.0f);
		render.setProjectionTransform(P);
		trackball.setSize(newWidth, newHeight);
	}
	/**
	Spincube checks to see if the cube is spinning
	(if left click was let go after moving).
	If so, it keeps the rotation.
	*/
	void spinCube()
	{
		//is it spinning?
		if(spinning){
			//keep rotating!
			rotationFromInput = rotationSpinStep*rotationFromInput;
			render.setModelTransform(translateFromInput*rotationFromInput*translateToOrigin);
		}
	}

	void handleEvents()
	{
		const sf::Input& Input = App->GetInput();
		bool shiftDown = Input.IsKeyDown(sf::Key::LShift) || Input.IsKeyDown(sf::Key::RShift);
		sf::Event Event;
		while (App->GetEvent(Event))
		{
			if (Event.Type == sf::Event::Closed)
				App->Close();
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App->Close();
			
			// This is for grading your code. DO NOT REMOVE
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Equal)
			{
				unsigned char *dest;
				unsigned int w = App->GetWidth();
				unsigned int h = App->GetHeight();
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
				glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
				
				FILE *file;
				file = fopen("_program1.ppm", "wb");
				fprintf(file, "P6\n%i %i\n%i\n", w, h, 255);
				for(int i=h-1; i>=0; i--)
					fwrite(&dest[i*w*3], sizeof(unsigned char), w*3, file);
				fclose(file);
				free(dest);
			}
			
			if (Event.Type == sf::Event::MouseButtonPressed)
			{
				previousPos = glm::vec2(Event.MouseButton.X, Event.MouseButton.Y);
				
				if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
				{
					buttonDown[0] = 1;
					spinning = false;
				}
				if(Event.MouseButton.Button == sf::Mouse::Right)
					buttonDown[1] = true;
				if(Event.MouseButton.Button == sf::Mouse::Middle)
					buttonDown[2] = true;
				if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
					buttonDown[2] = true;
			}
			
			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
				{
					buttonDown[0] = false;
					spinning=true;
					timeSinceMotion = motionClock.GetElapsedTime();
					float maxTime = 1.0f/(float)TARGET_FPS * TIME_WINDOW;
					if(timeSinceMotion > maxTime)
						spinning = false;
				}
				if(Event.MouseButton.Button == sf::Mouse::Right)
					buttonDown[1] = false;
				if(Event.MouseButton.Button == sf::Mouse::Middle)
					buttonDown[2] = false;
				if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
					buttonDown[2] = false;				
			}
			
			if (Event.Type == sf::Event::MouseMoved && (buttonDown[0] || buttonDown[1] || buttonDown[2]) )
			{
				glm::ivec2 newPos = glm::ivec2(Event.MouseMove.X, Event.MouseMove.Y);
				
				timeSinceMotion = motionClock.GetElapsedTime();
				motionClock.Reset();
				
				if(buttonDown[0])
					updateRotate(previousPos, newPos);
				if(buttonDown[1])
					updateXYTranslate(previousPos, newPos);
				if(buttonDown[2])
					updateZTranslate(previousPos, newPos);
				
				previousPos = newPos;
			}
			
			if (Event.Type == sf::Event::Resized) {
				render.reshape( Event.Size.Width, Event.Size.Height );
				updateAspectRatio(Event.Size.Width, Event.Size.Height);
			}
		}
	}
};

int main()
{
	printusers();
	Program4 prog;
	
    return EXIT_SUCCESS;
}
