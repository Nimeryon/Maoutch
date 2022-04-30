#pragma once
#include <cassert>
#include <vector>

#include "../../Types/Vector2i.h"

namespace maoutch
{
	template <typename T>
	struct Grid
	{
		Grid(const int& width, const int& height) : _width(width), _height(height)
		{
			_ReserveSpace();
		}
		Grid() : Grid(0, 0) {}
		virtual ~Grid() = default;

		void MapAll(void (*fn)(T value))
		{
			for (T& arrayElement : array)
				if (arrayElement) fn(arrayElement);
		}

		// Getters

		[[nodiscard]] bool Equals(const Grid<T>& grid) const { return this == grid; }
		[[nodiscard]] int GetGridIndex(const Vector2i& gridPos) const { return gridPos.x + gridPos.y * _width; }
		[[nodiscard]] int GetWidth() const { return _width; }
		[[nodiscard]] int GetHeight() const { return _height; }
		[[nodiscard]] int GetSize() const { return _width * _height; }
		[[nodiscard]] bool IsEmpty() const { return _width == 0 && _height == 0; }
		T& GetGridElement(const Vector2i& gridPos)
		{
			return GetGridElement(GetGridIndex(gridPos));
		}
		T& GetGridElement(const int& index)
		{
			assert(index >= 0 && index < _width * _height);
			return array[index];
		}

		// Setters

		void Resize(const int& width, const int& height)
		{
			_width = width;
			_height = height;
			_ReserveSpace();
		}
		void EmplaceBack(const T& element) { array.emplace_back(element); }
		void Clear() { array.clear(); }

		std::vector<T> array = {};

	private:
		int _width;
		int _height;

		void _ReserveSpace()
		{
			const int size = _width * _height;
			array.reserve(size);
		}
	};

	// Operators
	template <typename T>
	bool operator==(const Grid<T>& g1, const Grid<T>& g2)
	{
		if (g1.GetSize() != g2.GetSize()) return false;

		for (int y = 0; y < g1.GetHeight(); ++y)
			for (int x = 0; x < g1.GetWidth(); ++x)
				if (g1.GetGridElement(Vector2i(x, y)) != g2.GetGridElement(Vector2i(x, y)))
					return false;

		return true;
	}
	template <typename T>
	bool operator!=(const Grid<T>& g1, const Grid<T>& g2)
	{
		return !(g1 == g2);
	}
}
