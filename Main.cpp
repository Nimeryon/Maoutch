#include <imgui-SFML.h>
#include <time.h>

#include "Game/Game.h"

int main()
{
	// Setup random seed to now
	srand(time(0));

	maoutch::Game game("Maoutch");
	ImGui::SFML::Shutdown();
}