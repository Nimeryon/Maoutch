#pragma warning( disable : 4244 )
#pragma once
#include "Vector2.h"

namespace maoutch
{
	struct Vector2i
	{
		int x;
		int y;

		Vector2i() : x(0), y(0) {}
		Vector2i(int value) : x(value), y(value) {}
		Vector2i(int x, int y) : x(x), y(y) {}

		// Static constructors
		static Vector2i Zero() { return Vector2i(0, 0); }
		static Vector2i One() { return Vector2i(1, 1); }
		static Vector2i Left() { return Vector2i(-1, 0); }
		static Vector2i Right() { return Vector2i(1, 0); }
		static Vector2i Up() { return Vector2i(0, -1); }
		static Vector2i Down() { return Vector2i(0, 1); }

		// Methods
		void Translate(const Vector2i& v)
		{
			*this += v;
		}
		void Scale(const Vector2i& v)
		{
			*this *= v;
		}
		void Scale(const int& f)
		{
			*this *= f;
		}

		// Operator when used with SFML
		static Vector2i From(sf::Vector2f vector) { return Vector2i((int)vector.x, (int)vector.y); }
		static Vector2i From(sf::Vector2i vector) { return Vector2i(vector.x, vector.y); }
		static Vector2i From(Vector2 vector) { return Vector2i((int)vector.x, (int)vector.y); }
		Vector2 ToFloat() { return Vector2((int)x, (int)y); }
		operator sf::Vector2f() const { return { (float)x, (float)y }; }
		operator sf::Vector2i() const { return { x, y }; }
		operator Vector2() const { return { (float)x, (float)y }; }
		operator std::string() const { return std::to_string(x) + "," + std::to_string(y); }

		// Operators
		Vector2i& operator=(const Vector2i& v2) = default;

		Vector2i& operator+=(const Vector2i& v2)
		{
			x += v2.x;
			y += v2.y;

			return *this;
		}
		Vector2i& operator-=(const Vector2i& v2)
		{
			x -= v2.x;
			y -= v2.y;

			return *this;
		}
		Vector2i& operator*=(const Vector2i& v2)
		{
			x *= v2.x;
			y *= v2.y;

			return *this;
		}
		Vector2i& operator*=(const int& f)
		{
			x *= f;
			y *= f;

			return *this;
		}
		Vector2i& operator/=(const Vector2i& v2)
		{
			x /= v2.x;
			y /= v2.y;

			return *this;
		}
	};

	// Operators
	inline std::ostream& operator<<(std::ostream& os, const Vector2i& v)
	{
		os << v.x << "," << v.y;
		return os;
	}

	inline bool operator==(const Vector2i& v1, const Vector2i& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	inline bool operator!=(const Vector2i& v1, const Vector2i& v2)
	{
		return !(v1 == v2);
	}

	inline Vector2i operator+(const Vector2i& v1, const Vector2i& v2)
	{
		return Vector2i(v1.x + v2.x, v1.y + v2.y);
	}
	inline Vector2i operator-(const Vector2i& v1, const Vector2i& v2)
	{
		return Vector2i(v1.x - v2.x, v1.y - v2.y);
	}
	inline Vector2i operator*(const Vector2i& v1, const Vector2i& v2)
	{
		return Vector2i(v1.x * v2.x, v1.y * v2.y);
	}
	inline Vector2i operator*(const Vector2i& v1, const int& f)
	{
		return Vector2i(v1.x * f, v1.y * f);
	}
	inline Vector2i operator/(const Vector2i& v1, const Vector2i& v2)
	{
		return Vector2i(v1.x / v2.x, v1.y / v2.y);
	}
	inline Vector2i operator/(const Vector2i& v1, const int& f)
	{
		return Vector2i(v1.x / f, v1.y / f);
	}
}