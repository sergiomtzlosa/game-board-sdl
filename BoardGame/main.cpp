// ============================================================================
// [Include Section]
// ============================================================================

#include "Game.h"

#define WINDOW_WIDTH 755
#define WINDOW_HEIGHT 600

using namespace std;

// ============================================================================
// [SdlApplication]
// ============================================================================
#define APPTITLE "Gems Game"
// SdlApplication is nothing more than thin wrapper to SDL library. You need
// just to instantiate it and call run() to enter the SDL event loop.
struct SdlApplication
{
	SdlApplication();
	~SdlApplication();
	
	// Application state (just convenience instead of 0, 1, ...).
	enum APP_STATE
	{
		APP_OK = 0,
		APP_FAILED = 1
	};
	
	// Initialize application, called by run(), don't call manually.
	int init(int width, int height);
	
	// Destroy application, called by destructor, don't call manually.
	void destroy();
	
	// Run application, called by your code.
	int run(int width, int height);
	
	// Called to process SDL event.
	void onEvent(SDL_Event* ev);
	
	// Called to render content into buffer.
	void Render();

	// Whether the application is in event loop.
	bool _running;
	SDL_Window *win;
	SDL_Renderer *renderer;
    SDL_Event ev;
    
    // Game stuff class
    Game *newGame;
};

SdlApplication::SdlApplication() :
_running(false)
{
}

SdlApplication::~SdlApplication()
{
	destroy();
}

/**
 *	Init method, we create the window and set the icon for the game
 *
 *	@param	width	Window width
 *	@param	height	Window height
 *
 *	@return	Game status
 */
int SdlApplication::init(int width, int height)
{
	// Initialize the SDL library.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
		return APP_FAILED;
	}
	
	win = SDL_CreateWindow(APPTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (SDL_GetWindowFlags(win) & SDL_WINDOW_OPENGL)
        printf("using openGL");
    
    SDL_Surface *icon = IMG_Load("Red.png");
    SDL_SetWindowIcon(win, icon);
    SDL_FreeSurface(icon);
                      
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	// Success.
	return APP_OK;
}

/**
 *	Destroy method
 *
 */
void SdlApplication::destroy()
{
	if (win)
	{
        //destroy game
        delete newGame;
        newGame = NULL;
        
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(renderer);
        SDL_Quit();
	}
}

/**
 *	Main method that runs the game, here we set the refresh time (60 fps)
 *
 *	@param	width	Window width
 *	@param	height	Window height
 *
 *	@return	Game state
 */
int SdlApplication::run(int width, int height)
{
	// Initialize application.
	int state = init(width, height);
	if (state != APP_OK) return state;
	
	// Enter to the SDL event loop.
	_running = true;
	
    const int FPS = 60;
    Uint32 start;
    
    newGame = new Game();
    
	while (SDL_WaitEvent(&ev))
	{
        start = SDL_GetTicks();
        
		onEvent(&ev);
		Render();

		if (_running == false)
		{
			break;
		}
        
        if (1000 / FPS > SDL_GetTicks() - start)
        {
            SDL_Delay(1000 / FPS - (SDL_GetTicks()-start));
        }
	}
	
	return APP_OK;
}

/**
 *	Handle key press event, in this case, here we handle game quit
 *
 *	@param	ev	Event to track
 */
void SdlApplication::onEvent(SDL_Event* ev)
{
	switch (ev->type)
	{
		case SDL_QUIT:
			_running = false;
			break;
			
		case SDL_KEYDOWN:
		{
			if (ev->key.keysym.sym == SDLK_ESCAPE)
			{
				_running = false;
			}
            break;
		}
	}
}

/**
 *	Main render method of the game, here we update the render object to update canvas
 *
 */
void SdlApplication::Render()
{
    SDL_RenderClear(renderer);
    
    newGame->RenderGame(renderer, win);
    newGame->SetEvent(&ev);
    newGame->DrawBoard();

    SDL_RenderPresent(renderer);
}

// ============================================================================
// [Entry-Point]
// ============================================================================

int main(int argc, char* argv[])
{
	SdlApplication app;
	return app.run(WINDOW_WIDTH, WINDOW_HEIGHT);
}
