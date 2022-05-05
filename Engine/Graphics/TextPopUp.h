#pragma once
#include "../Objects/GameObject.h"
#include "Text.h"

namespace maoutch
{
	class TextPopUp : public GameObject
	{
	public:
		TextPopUp(const std::string& text, const Vector2& position, const float& lifetime = 1, const int& textSize = 16, const sf::Color& color = sf::Color::White);
		~TextPopUp() override;

		void Update(float dt) override;

	private:
		Text _text;

		Vector2 _initialPosition;
		float _finalHeight;

		float _currentLifetime;
		float _lifetime;
		sf::Color _color;

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}
