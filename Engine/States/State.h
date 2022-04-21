#pragma once
namespace sf
{
	class RenderWindow;
}

namespace maoutch
{

	class State
	{
	public:
		virtual ~State() = default;

		virtual void Init() {}

		virtual void ProcessInputs() {}

		virtual void EarlyUpdate(float dt) {}
		virtual void Update(float dt) {}
		virtual void FixedUpdate(float dt) {}
		virtual void LateUpdate(float dt) {}

		virtual void ImGui(float dt) {}

		virtual void Draw(sf::RenderWindow& window) {}
	};
}