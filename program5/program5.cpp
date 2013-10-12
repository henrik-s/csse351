//Replace with your usernames!
#define printusers() printf("Program by HARRISSA+sohlbehd\n");

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
#include "Maze.h"
#include "MazeModel.h"

#define RESOLUTION 512
#define TARGET_FPS 30                // controls spin update rate
#define TIME_WINDOW 3                // number of frames motion is valid after release

/**
Problem: Create a 3D maze with a floor that can be traversed
easily and provide a top down look at the maze.

Solution: Use current "elements" maze and create a horizontal
wall model and a vertical wall model. Draw these walls accordingly
to each horizontal and vertical wall in the maze. After that, draw
the floor and use transformation matrices to traverse the maze and 
change the perspective. 

Authors: Scott Harris and Henrik Sohlberg
*/
class Program5
{
public:
	Program5(unsigned int const & w, unsigned int const & h)
	{
		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program5");
		render.init(w, h);
		step = 2;
		render.printHelp();
		
		while (App->IsOpened())
		{			
			App->SetActive();
			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0)
				sf::Sleep(sleepTime);
			render.display();
			App->Display();
			handleEvents();
		}
	}
	
	~Program5()
	{
	}
	
private:
	sf::Window *App;
	RenderEngine render;
	unsigned int step;

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
			
			if (Event.Type == sf::Event::Resized)
			{ glViewport(0, 0, Event.Size.Width, Event.Size.Height); }
			
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
			{
				render.generateMaze(step);
				step++;
			}
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::W)
			{
				render.move('w');
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::S)
			{
				render.move('s');
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::A)
			{
				render.move('a');
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::D)
			{
				render.move('d');
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Q)
			{
				render.move('q');
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::E)
			{
				render.move('e');
			}
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::M)
			{
				render.toggleMap();
			}
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::H)
			{
				render.printHelp();
			}
		}
	}
};

int main(int argc, char ** argv)
{
	printusers();
	
	/* check that there are sufficient arguments */
	unsigned int w;
	unsigned int h;
	if (argc < 3) {
		w = 10;
		h = 10;
		fprintf(stderr, "The width and height can be specified as command line arguments. Defaulting to %i %i\n", w, h);
	}
	else {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}
	
	Program5 prog(w, h);
	
    return EXIT_SUCCESS;
}
