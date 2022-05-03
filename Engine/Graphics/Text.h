#pragma once
namespace sf
{
	class Font;
}
#include <SFML/Graphics/Text.hpp>

#include "../../Interfaces/ITransformable.h"
#include "../Objects/GameObject.h"

namespace maoutch
{
	class Text : public ITransformable
	{
	public:
		explicit Text(const std::string& text = "", const float& outlineThickness = 1);
		virtual ~Text();

		// Setters

		void SetText(const std::string& text);
		void SetSize(const int& size);
		void SetLineSpacing(const float& spacing);
		void SetLetterSpacing(const float& spacing);
		void SetColor(const sf::Color& color);
		void SetOutlineThickness(const float& thickness);
		void SetOutlineColor(const sf::Color& color);

		// Getters

		std::string GetText() const;
		int GetSize() const;
		float GetLineSpacing() const;
		float GetLetterSpacing() const;
		sf::Color GetColor() const;
		float GetOutlineThickness() const;
		sf::Color GetOutlineColor() const;

		void Draw(sf::RenderWindow& window, const sf::Transform& transform);

		sf::FloatRect GetBounds() const override;

	private:
		sf::Text* _text;

		void _SetFont(sf::Font& font);
	};
}
