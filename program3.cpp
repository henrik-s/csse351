//Replace with your usernames!
#define printusers() printf("Program by sohlbehd+harrissa\n");

#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include "glew/glew.h"
#include <SFML/Window.hpp>
#include "ShaderManager.h"
#include "GLHelper.h"
#include "RenderEngine.h"
#include "GameEngine.h"

#define RESOLUTION 512
struct coord{
	int x;
	int y;
};



class Program3
{
public:
	Program3()
	{

		// Create the main window
		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program3");
		render.init();

		gameEngine.init();
		
		// Start render loop
		while (App->IsOpened())
		{			
			// Set the active window before using OpenGL commands
			// It's not needed here because the active window is always the same,
			// but don't forget it if you use multiple windows or controls
			App->SetActive();

			// Render the scene
			render.display(gameEngine.getBoard(), gameEngine.isCheatModeOn());
			
			// Finally, display rendered frame on screen
			App->Display();
			
			// Handle any events that are in the queue
			handleEvents();
		}
	}
	
	~Program3()
	{
	}
private:
	sf::Window *App;
	RenderEngine render;
	GameEngine gameEngine;

float round(float d)
	{return (int) floor(d+0.5);}
	void handleEvents()
	{
		sf::Event Event;
		while (App->GetEvent(Event))
		{
			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				App->Close();

			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::N)) {
				printf("New game!\n");
				gameEngine.init();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Space)) {
				gameEngine.cheat();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::K)) {
				gameEngine.hailToTheKing();
			}
			// Escape key : exit
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
			if(Event.Type == sf::Event::MouseButtonPressed)
				{
					int x = Event.MouseButton.X;
					int y = Event.MouseButton.Y;
					y = RESOLUTION - y;


					coord coordBoard[32];
					int c = 0;
					for (int a = 1; a < 8; a+=2){
						for(int b = 0; b <8; b+=2){
							coord square1;
							square1.x = a;
							square1.y = b;
							coordBoard[c]=square1;
							c+=2;
						}
					}
					c=1;
					for (int d = 0; d < 8; d+=2){
						for(int e = 1; e <8; e+=2){
							coord square2;
							square2.x = d;
							square2.y = e;
							coordBoard[c]=square2;
							c+=2;
						}
					}

					render.display(0, gameEngine.isCheatModeOn(), true);

					float clickColor[4];

					glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, clickColor);
					int p =  round(clickColor[0]*100);
					coord clickCoord = coordBoard[p-1];
					//printf("clicked X%i Y%i\n", clickCoord.x, clickCoord.y);
					
					gameEngine.handleClick(clickCoord.x, clickCoord.y);
						
				}
		}
		
	}
	
};



int main()
{
	printusers();
	Program3 prog;
	
    return EXIT_SUCCESS;
}
