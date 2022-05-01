#include <SFML/Graphics.hpp>

#include "TextPopUp.h"
#include "../Objects/GameObjectHandler.h"

namespace maoutch
{
	TextPopUp::TextPopUp(const std::string& text, const Vector2& position, const float& lifetime, const int& textSize, const sf::Color& color) :
		GameObject("TextPopUp", 200),
		_initialPosition(position),
		_finalHeight(position.y - 64),
		_currentLifetime(0),
		_lifetime(lifetime),
		_color(color)
	{
		_text = new Text(text);
		_text->SetPosition(position);
		_text->SetColor(_color);
		_text->SetSize(textSize);

		sf::FloatRect rect = _text->GetBounds();
		_text->SetOrigin(Vector2(rect.width, rect.height) / 2.f);
	}
	TextPopUp::~TextPopUp()
	{
		delete _text;
	}

	void TextPopUp::Update(float dt)
	{
		_currentLifetime += dt;
		if (_currentLifetime >= _lifetime)
			GameObjectHandler::Instance()->Destroy(this);

		const float t = _currentLifetime / _lifetime;

		// Colors
		const float alphaT = 1 - Ease(easing::EaseType::EaseInExpo, t);
		sf::Color outlineColor = _text->GetOutlineColor();
		_color.a = outlineColor.a = 255.f * alphaT;
		_text->SetColor(_color);
		_text->SetOutlineColor(outlineColor);

		// Position
		_text->SetPosition(Vector2(_initialPosition.x, math::Lerp(_initialPosition.y, _finalHeight, Ease(easing::EaseType::EaseInQuad, t))));
	}

	void TextPopUp::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_text->Draw(window, transform);
	}
}
