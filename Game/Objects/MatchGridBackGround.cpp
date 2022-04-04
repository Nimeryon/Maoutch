#include <SFML/Graphics.hpp>
#include <array>

#include "MatchGridBackGround.h"
#include "MatchElement.h"
#include "../../Engine/AssetLoader.h"
#include "../../Tools/Texture.h"

extern maoutch::Vector2 ELEMENT_SIZE;

namespace maoutch
{
	MatchGridBackGround::MatchGridBackGround() :
		GameObject("MatchGrid BackGround", -1),
		_vertexArray(new sf::VertexArray())
	{
		_renderState.texture = &AssetLoader::GetInstance()->GetTexture("Elements Background");
		_vertexArray->setPrimitiveType(sf::Quads);
	}
	MatchGridBackGround::~MatchGridBackGround()
	{
		delete _vertexArray;
	}

	void MatchGridBackGround::Setup(Grid<MatchElement*>& grid)
	{
		_vertexArray->clear();

		Vector2 halfElementSize = ELEMENT_SIZE / 2.f;
		Vector2 halfGridSize = { grid.GetWidth() / 2.f - .5f, grid.GetHeight() / 2.f - .5f };

		Vector2i gridPos;
		for (gridPos.y = 0; gridPos.y < grid.GetHeight(); ++gridPos.y)
		{
			for (gridPos.x = 0; gridPos.x < grid.GetWidth(); ++gridPos.x)
			{
				if (grid.GetGridElement(gridPos) == nullptr) continue;

				Vector2i texturePosition = _CalculateAutoTiling(grid, gridPos);

				std::array<sf::Vertex, 4> vertices;
				texture::SetTextureCoord(vertices, texturePosition, Vector2i::From(ELEMENT_SIZE));
				
				vertices[0].position = {
					-halfElementSize.x + ELEMENT_SIZE.x * (gridPos.x - halfGridSize.x),
					-halfElementSize.x + ELEMENT_SIZE.y * (gridPos.y - halfGridSize.y)
				};
				vertices[1].position = {
					halfElementSize.x + ELEMENT_SIZE.x * (gridPos.x - halfGridSize.x),
					-halfElementSize.x + ELEMENT_SIZE.y * (gridPos.y - halfGridSize.y)
				};
				vertices[2].position = {
					halfElementSize.x + ELEMENT_SIZE.x * (gridPos.x - halfGridSize.x),
					halfElementSize.y + ELEMENT_SIZE.y * (gridPos.y - halfGridSize.y)
				};
				vertices[3].position = {
					-halfElementSize.x + ELEMENT_SIZE.x * (gridPos.x - halfGridSize.x),
					halfElementSize.y + ELEMENT_SIZE.y * (gridPos.y - halfGridSize.y)
				};

				for (sf::Vertex& vertex : vertices)
					_vertexArray->append(vertex);
			}
		}
	}

	bool MatchGridBackGround::_IsTileValid(Grid<MatchElement*>& grid, const Vector2i& gridPos)
	{
		if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= grid.GetWidth() || gridPos.y >= grid.GetHeight()) return false;
		return grid.GetGridElement(gridPos) != nullptr;
	}
	Vector2i MatchGridBackGround::_CalculateAutoTiling(Grid<MatchElement*>& grid, const Vector2i& gridPos)
	{
		std::string neighbourBinary;
		neighbourBinary += _IsTileValid(grid, { gridPos.x - 1, gridPos.y }) ? '1' : '0'; // 0000 0001
		neighbourBinary += _IsTileValid(grid, { gridPos.x - 1, gridPos.y - 1 }) ? '1' : '0'; // 0000 0010
		neighbourBinary += _IsTileValid(grid, { gridPos.x, gridPos.y - 1 }) ? '1' : '0'; // 0000 0100
		neighbourBinary += _IsTileValid(grid, { gridPos.x + 1, gridPos.y - 1 }) ? '1' : '0'; // 0000 1000
		neighbourBinary += _IsTileValid(grid, { gridPos.x + 1, gridPos.y }) ? '1' : '0'; // 0001 0000
		neighbourBinary += _IsTileValid(grid, { gridPos.x + 1, gridPos.y + 1 }) ? '1' : '0'; // 0010 0000
		neighbourBinary += _IsTileValid(grid, { gridPos.x, gridPos.y + 1 }) ? '1' : '0'; // 0100 0000
		neighbourBinary += _IsTileValid(grid, { gridPos.x - 1, gridPos.y + 1 }) ? '1' : '0'; // 1000 0000

		int autoTilingIndex = std::stoi(neighbourBinary, 0, 2);
		int autoTile = autoTilingArray[autoTilingIndex];

		return { autoTile % 8, autoTile / 8 };
	}

	void MatchGridBackGround::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		_renderState.transform = transform;
		window.draw(*_vertexArray, _renderState);
	}
}
