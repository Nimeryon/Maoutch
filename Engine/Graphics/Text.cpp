#include <SFML/Graphics.hpp>

#include "Text.h"
#include "../Assets.h"

namespace maoutch
{
	Text::Text(const std::string& text, const float& outlineThickness) :
		_text(new sf::Text())
	{
		_text->setFont(Assets::Instance()->GetFont());

		SetText(text);
		SetOutlineThickness(outlineThickness);
		SetOutlineColor(sf::Color::Black);

		Assets::Instance()->onFontChange += EventHandler::Bind<sf::Font&>(&Text::_SetFont, this);
	}
	Text::~Text()
	{
		Assets::Instance()->onFontChange -= EventHandler::Bind<sf::Font&>(&Text::_SetFont, this);
	}
	
	void Text::SetText(const std::string& text) { _text->setString(text); }
	void Text::SetSize(const int& size) { _text->setCharacterSize(size); }
	void Text::SetLineSpacing(const float& spacing) { _text->setLineSpacing(spacing); }
	void Text::SetLetterSpacing(const float& spacing) { _text->setLetterSpacing(spacing); }
	void Text::SetColor(const sf::Color& color) { _text->setFillColor(color); }
	void Text::SetOutlineThickness(const float& thickness) { _text->setOutlineThickness(thickness); }
	void Text::SetOutlineColor(const sf::Color& color) { _text->setOutlineColor(color); }

	std::string Text::GetText() const { return _text->getString(); }
	int Text::GetSize() const { return _text->getCharacterSize(); }
	float Text::GetLineSpacing() const { return _text->getLineSpacing(); }
	float Text::GetLetterSpacing() const { return _text->getLetterSpacing(); }
	sf::Color Text::GetColor() const { return _text->getFillColor(); }
	float Text::GetOutlineThickness() const { return _text->getOutlineThickness(); }
	sf::Color Text::GetOutlineColor() const { return _text->getOutlineColor(); }

	void Text::Draw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		window.draw(*_text, _localTransform.getTransform());
	}

	void Text::_SetFont(sf::Font& font) { _text->setFont(font); }

	sf::FloatRect Text::GetBounds() const { return _text->getGlobalBounds(); }
}
