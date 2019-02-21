#include "App.h"

int main(int argc, char** argv) {
	App app;
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	app.run("Zombies Game by Ndongmo S. F.", 1024, 768, NS2::FULLSCREEN);

	return 0;
}