#pragma once
namespace sf
{
	class VertexArray;
	class Texture;
	class RenderStates;
}

#include "../../Engine/Objects/GameObject.h"
#include "../Data/Element.h"
#include "../../Types/Grid.h"
#include "../../Types/Vector2i.h"

namespace maoutch
{
	class MatchElement;

	const int autoTilingArray[256] = {
		0, 0, 4, 4, 0, 0, 4, 4,
		1, 1, 8, 8, 1, 1, 5, 5,
		0, 0, 4, 4, 0, 0, 4, 4,
		1, 1, 8, 8, 1, 1, 5, 5,
		20, 20, 12, 12, 20, 20, 12, 12,
		16, 16, 10, 10, 16, 16, 32, 32,
		20, 20, 12, 12, 20, 20, 12, 12,
		21, 21, 24, 24, 21, 21, 13, 13,
		0, 0, 4, 0, 0, 0, 4, 4,
		1, 1, 8, 8, 1, 1, 5, 5,
		0, 0, 4, 4, 0, 0, 4, 4,
		1, 1, 8, 8, 1, 1, 5, 5,
		20, 20, 12, 12, 20, 20, 12, 12,
		16, 16, 10, 10, 16, 16, 32, 32,
		20, 20, 12, 12, 20, 20, 12, 12,
		21, 21, 24, 24, 21, 21, 13, 13,
		3, 3, 9, 7, 3, 3, 9, 7,
		2, 2, 11, 26, 2, 2, 27, 6,
		3, 3, 9, 7, 3, 3, 9, 7,
		2, 2, 11, 26, 2, 2, 27, 6,
		17, 17, 19, 33, 17, 17, 19, 33,
		18, 18, 46, 38, 18, 18, 39, 40,
		17, 17, 19, 33, 17, 17, 19, 33,
		35, 35, 31, 45, 35, 35, 43, 37,
		3, 3, 9, 7, 3, 3, 9, 7,
		2, 2, 11, 26, 2, 2, 27, 6,
		3, 3, 9, 7, 3, 3, 9, 7,
		2, 2, 11, 26, 2, 2, 27, 6,
		23, 23, 25, 15, 23, 23, 25, 15,
		34, 34, 30, 42, 34, 34, 44, 36,
		23, 23, 25, 15, 23, 23, 25, 15,
		22, 22, 41, 28, 22, 22, 29, 14
	};

	class MatchGridBackGround : public GameObject
	{
	public:
		explicit MatchGridBackGround(const Element& element = Element::ElementValue::None);
		~MatchGridBackGround() override;

		void Setup(Grid<MatchElement*>& grid);

	private:
		sf::RenderStates _renderState;
		sf::VertexArray* _vertexArray;

		Element _element;

		bool _IsTileValid(Grid<MatchElement*>& grid, const Vector2i& gridPos);
		Vector2i _CalculateAutoTiling(Grid<MatchElement*>& grid, const Vector2i& gridPos);

		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;
	};
}