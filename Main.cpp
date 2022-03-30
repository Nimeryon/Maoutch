#include <imgui-SFML.h>
#include <time.h>

#include "Game/Game.h"
#include "Types/Vector2i.h"

// Window
int WINDOW_WIDTH = 1080;
int WINDOW_HEIGHT = 720;

// Elements
maoutch::Vector2 ELEMENT_SIZE = { 34 };
maoutch::Vector2 ELEMENT_SCALE = { 1.25f };
float ELEMENT_SNAP_DISTANCE = 2.f;
float ELEMENT_DT_MULTIPLIER = 10;
float ELEMENT_SWIPE_DISTANCE = 20;

int main()
{
	// Setup random seed to now
	srand(time(0));

	maoutch::Game game("Maoutch");
	ImGui::SFML::Shutdown();
}