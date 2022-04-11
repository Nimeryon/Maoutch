#include <imgui-SFML.h>
#include <time.h>

#include "Game/Game.h"
#include "Types/Vector2i.h"

// Window
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

// Elements
maoutch::Vector2 ELEMENT_SIZE = { 38 };
maoutch::Vector2 ELEMENT_SCALE = { 1.4f };
float ELEMENT_SWIPE_DISTANCE = 30;

int main()
{
	// Setup random seed to now
	srand(time(0));

	maoutch::Game game("Maoutch");
	ImGui::SFML::Shutdown();
}