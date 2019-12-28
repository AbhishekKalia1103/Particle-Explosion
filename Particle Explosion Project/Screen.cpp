#include "Screen.h"
namespace caveofprogramming {

Screen::Screen() :
		m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(
				NULL) {
}
bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	// to create window
	m_window = SDL_CreateWindow("Particle Fire Explosion",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			SCREEN_HEIGTH, SDL_WINDOW_SHOWN);

	// if window not created then we quit
	if (m_window == NULL) {
		SDL_Quit();
		return false;
	}

	// renderer is used to draw on window as screen refreshes
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

	//texture is like a bitmap which we draw directly to.
	// so we'll be creating renderer doing stuff with texture and then
	//passing the texture to renderer & telling the renderer to display in the window.
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGTH);

	if (m_renderer == NULL) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	if (m_texture == NULL) {
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	//buffer is an area of memory where we're going to insert information that relates to pixels
	// we use that memory to write to the texture which will pass to renderer which will display on window.

	// in this way these three are related in someway.

	//Here we allocating 32 bits to each pixel
	m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGTH];
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGTH];

	memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGTH * sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGTH * sizeof(Uint32));

	return true;
}
//clearing the screen to visualize updation in position of particles

void Screen::boxBlur() {
//here we are copying from one Buffer to another
//swap the buffers, so pixel is in m_buffer2 and we are drawing to m_buffer1

	Uint32 *temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	for (int y = 0; y < SCREEN_HEIGTH; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;
			for (int row = -1; row <=1; row++) {
				for (int col = -1; col <= 1; col++) {
					int currentX = x + col;
					int currentY = y + row;

					if (currentX >= 0 && currentX < SCREEN_WIDTH
							&& currentY >= 0 && currentY < SCREEN_HEIGTH) {
						Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];


						Uint8 red = color>>24;
						Uint8 green = color>>16;
						Uint8 blue = color>>8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}
				}
			}
			Uint8 red = redTotal/9;
			Uint8 green = greenTotal/9;
			Uint8 blue = blueTotal/9;

			setPixel(x,y,red,green,blue);

		}
	}
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGTH) {
		return;
	}
	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}
void Screen::update() {
	//update texture with the values contained in buffer
	SDL_UpdateTexture(m_texture, NULL, m_buffer1,
			SCREEN_WIDTH * sizeof(Uint32));
	//updating renderer
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

}
bool Screen::processEvents() {
	SDL_Event event;
	//SDL_QUIT event is occurred when cross 'X" button on window is clicked
	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
void Screen::close() {
	delete[] m_buffer1;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
}
