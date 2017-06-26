// GLEW
#include <GL\glew.h>
// SDL
#include <SDL.h>
#include <SDL_opengl.h>
// imgui
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"
// Standard Template Libary
#include <iostream>
#include <sstream>

// Our stuff
#include "official_game.h"
#include "Tester.h"
#include <vector>
#include "genetic_game_master.h"
#include "battle_arena.h"
#include <ctime>

SDL_Window *win; //pointer to window
SDL_GLContext context; //id of context created for opengl (also a pointer)

int quit_from_app(int stages = 4, const char* errormessage = "", const char* errormessage2 = "")
{	//quits program with error message if added, stage translates to how much cleanup is needed
	std::cout << errormessage << errormessage2 << std::endl;
	switch(stages)
	{
	case 4:ImGui_ImplSdlGL3_Shutdown();
	case 3:SDL_GL_DeleteContext(context);
	case 2:SDL_DestroyWindow(win);
	case 1:SDL_Quit();
	case 0: break;
	default: std::cout << "Invalid cleanup stage." << std::endl; break;
	}
	int ret = errormessage[0] != '\0';
	if(ret)
	{
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
	}
	exit(ret);
}

int main( int argc, char* args[] )
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)	quit_from_app(0, "[SDL indítása]Hiba az SDL inicializálása közben: ", SDL_GetError());
	// Setting up OpenGL framebuffers
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,		1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          24);
	// Antialiasing and V-sync
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	SDL_GL_SetSwapInterval(0);
	// Creating an SDL window
    win = SDL_CreateWindow( "Hello SDL&OpenGL&Imgui!",100,100,800,600,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(win == 0) quit_from_app(1, "[Creating window] Error creating SDL window", SDL_GetError());
	// Creating the virtual 'workplace' for opengl to operate in 
	context	= SDL_GL_CreateContext(win);
	if(context == 0) quit_from_app(2, "[Creating OpenGL context] Error initializing: ", SDL_GetError());

	GLenum error = glewInit();	//'glews' opengls functions to their respective function names (unimportant)
	if(error != GLEW_OK) quit_from_app(2, "[GLEW] Error initializing glew");
	//Getting Version numbers
	int glVersion[2] = {-1, -1};
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
	std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;
	if(glVersion[0] == -1 && glVersion[1] == -1)
		quit_from_app(3, "[OGL Version/Context Error] Maybe invalid attributes was passed to SDL_GL_SetAttribute(...).");
	std::stringstream window_title;
	window_title << "OpenGL " << glVersion[0] << "." << glVersion[1];
	SDL_SetWindowTitle(win, window_title.str().c_str());
	//Initialization of ImGui
	ImGui_ImplSdlGL3_Init(win);
	//Variables
	bool quit = false;	//set to true inside the main loop to exit program
	SDL_Event ev;		//Keyboard, Mouse, Window resize, Quit, ect events are stored in this
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f); //background color [0,1]-RGBA
	
/*  ============================
/*  Add tasks here: */
	srand((int)time(0));
	Updater gametasks;

	const int rounds = 30;

	//gametasks.AddCyborgVsHeuristics(10);

	GeneticGameMaster ggm = GeneticGameMaster(5, 12);
	//ggm.createGames(1);
	for(int i = 0; i < rounds; ++i)
	{
		ggm.addGames(gametasks);
	}

	gametasks.SetToFirstTask();

/*  =============================  */

	while (!quit)		//the main 'infinite' loop for rendering frame-by-frame
	{
		while ( SDL_PollEvent(&ev) )	//SDL has a stack for events. we empty the stack since last frame
		{
			ImGui_ImplSdlGL3_ProcessEvent(&ev);		//Imgui alters internal state by processing SDL event
			bool is_mouse_captured = ImGui::GetIO().WantCaptureMouse; //Uniportant now.
			bool is_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard; //Uniportant now.
			switch(ev.type)
			{
			case SDL_QUIT:	quit = true;	break; //fun-fact: try removing this line ;)
			case SDL_KEYDOWN:if(ev.key.keysym.sym == SDLK_ESCAPE)quit = true; break;
			case SDL_KEYUP:					break;
			case SDL_MOUSEBUTTONDOWN:		break;
			case SDL_MOUSEBUTTONUP:			break;
			case SDL_MOUSEWHEEL:			break;
			case SDL_MOUSEMOTION:			break;
			case SDL_WINDOWEVENT:	glViewport(0, 0, ev.window.data1, ev.window.data2);	break; //resize framebuffer
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears framebuffer (try without it!)
		ImGui_ImplSdlGL3_NewFrame(win); //all imgui calls happen after this and before render
		
		gametasks.Update(); //don't change

		//ImGui::ShowTestWindow(); //Shocases ImGui features
		ImGui::Render();
		SDL_GL_SwapWindow(win);	//Swaps front and black buffers ==> Appears on screen, this waits if vsync is on
	}
	quit_from_app(); //clenup
	return 0;
}