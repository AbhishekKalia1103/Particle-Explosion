#include<SDL.h>
#include<iostream>
#include "Screen.h"
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include "Swarm.h"
using namespace std;
using namespace caveofprogramming;
int main(int argv, char** args) {

	srand(time(NULL));
	Screen screen;
	if (screen.init() == false) {
		cout << "Error" << endl;
	}
	Swarm swarm;

// if window created
	//bool quit = false;

	while (true) {
		//update particles
		//Draw particles
		int elapsed = SDL_GetTicks();

		//screen.clear();
		swarm.update(elapsed);

				unsigned char green = (unsigned char) (1 + sin(elapsed * 0.001)) * 128;
				unsigned char red = (unsigned char) (1 + sin(elapsed * 0.002)) * 128;
				unsigned char blue = (unsigned char) (1 + sin(elapsed * 0.003)) * 128;

		const Particle *const pParticles = swarm.getParticles();

		for(int i=0; i<Swarm::NPARTICLES; i++){
			Particle particle = pParticles[i];

			//plot particles
			int x=(particle.m_x+1) * Screen::SCREEN_WIDTH/2;
			int y=particle.m_y * Screen::SCREEN_WIDTH/2+Screen::SCREEN_HEIGTH/2;

			screen.setPixel(x,y,red,green,blue);
		}

		screen.boxBlur();
		//draw the screen
		screen.update();

		//check for events
		if (screen.processEvents() == false) {
			break;
		}
	}

	screen.close();
//destroy window before quitting the program

	return 0;
}
