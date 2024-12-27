//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
void EmMainLoop(void* arg)
{
	Game* game = reinterpret_cast<Game*>(arg);
	game->EmRunIteration();
}
#endif

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();
	if (success)
	{
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(EmMainLoop, &game, 0, true);
#else
		game.RunLoop();
#endif
	}
	game.Shutdown();
	return 0;
}
