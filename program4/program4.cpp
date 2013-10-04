//Replace with your usernames!
#define printusers() printf("Program by USER1+USER2\n");

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
	
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		float phi;
		glm::vec3 axis;
		
		//computes the appropriate rotation data and stores in phi and axis
		trackball.getRotation(phi, axis, oldPos, newPos);
		printf("%.2f\n", phi);
		printf("%.2f, %.2f, %.2f\n", axis.x, axis.y, axis.z);
		float p = sqrt(pow(axis.x,2)+pow(axis.y,2)+pow(axis.z,2));
		glm::vec3 w = glm::vec3(axis.x/p, axis.y/p, axis.z/p);
		glm::vec3 t= glm::vec3(1, w.y, w.z);
		if(w.x > w.y && w.z > w.y)
			t = glm::vec3(w.x, 1, w.z);
		if(w.x > w.z && w.y > w.z)
			t = glm::vec3(w.x, w.y, 1);
		
		glm::vec3 cross = glm::cross(t, w);
		float c = sqrt(pow(cross.x,2)+pow(cross.y,2)+pow(cross.z,2));
		glm::vec3 u = glm::vec3(cross.x/c, cross.y/c, cross.z/c);
		glm::vec3 v = glm::cross(w, u);

		glm::mat4 r1 = glm::mat4(glm::vec4(w, 0),glm::vec4(u, 0),glm::vec4(v,0),glm::vec4(0,0, 0, 1));
		glm::mat4 r2 = glm::mat4(cos(phi), -sin(phi), 0, 0,  sin(phi), cos(phi),0,0,0,0, 0, 0, 0, 0,  0, 1);
		glm::mat4 r3 = glm::transpose(r1);
		glm::mat4 r4 =  r1*r2;
		render.setModelTransform(r4);
		render.setModelTransform(r3);
		

		//
		//
		// Put your code for a rotation of phi about axis here.
		//
		//

	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define XY_SENSITIVITY 0.01f  //may be helpful to reduce transform amount
		//
		//
		// Put your code for a translation in the x,y direction here.
		//
		//
	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		#define Z_SENSITIVITY 0.01f //may be helpful to reduce transform amount
		//
		//
		// Put your code for a translation in the z direction here.
		//
		//
	}
	
	void updateAspectRatio(int const & newWidth, int const & newHeight)
	{
		//
		//
		// Put your code to adjust for the change in viewport here.
		//
		//
	}
	
	void spinCube()
	{
		//
		//
		// Put your code for an incremental rotation here.  
		//
		//
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
					buttonDown[0] = false;
				if(Event.MouseButton.Button == sf::Mouse::Right)
					buttonDown[1] = false;
				if(Event.MouseButton.Button == sf::Mouse::Middle)
					buttonDown[2] = false;
				if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
					buttonDown[2] = false;
				
				timeSinceMotion = motionClock.GetElapsedTime();
				float maxTime = 1.0f/(float)TARGET_FPS * TIME_WINDOW;
				if(timeSinceMotion < maxTime)
					spinning = false;
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
