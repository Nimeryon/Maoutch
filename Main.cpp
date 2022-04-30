#include <imgui-SFML.h>
#include <time.h>

#include "Game/Game.h"

int main()
{
	// Setup random seed to now
	srand((int)time(nullptr));

	maoutch::Game game("Maoutch");
	ImGui::SFML::Shutdown();
}