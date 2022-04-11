#pragma warning( disable : 4244 )
#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>

#include "../Tools/Math.h"
#include "../Tools/Random.h"
#include "../Tools/Easing.h"
#include "../Tools/String.h"

namespace  maoutch
{
	struct Vector2
	{
		float x;
		float y;

		Vector2() : x(0), y(0) {}
		Vector2(float value) : x(value), y(value) {}
		Vector2(float x, float y) : x(x), y(y) {}

		// Static constructors
		static Vector2 Zero() { return Vector2(0, 0); }
		static Vector2 One() { return Vector2(1, 1); }
		static Vector2 Left() { return Vector2(-1, 0); }
		static Vector2 Right() { return Vector2(1, 0); }
		static Vector2 Up() { return Vector2(0, -1); }
		static Vector2 Down() { return Vector2(0, 1); }

		// Static methods
		static Vector2 FromString(const std::string& value)
		{
			const auto values = string::Split(value, '|');
			return Vector2(
				std::atof(&values[0][0]),
				std::atof(&values[1][0])
			);
		}
		static float Magnitude(const Vector2& v)
		{
			return sqrtf((v.x * v.x) + (v.y * v.y));
		}
		static float Distance(const Vector2& v1, const Vector2& v2)
		{
			float x = v2.x - v1.x;
			float y = v2.y - v1.y;

			return Magnitude(Vector2(x, y));
		}
		static float Determinant(const Vector2& v1, const Vector2& v2)
		{
			return v1.x * v2.y - v2.x * v1.y;
		}
		static Vector2 Normalize(const Vector2& v)
		{
			float magn = Magnitude(v);
			return Vector2(
				v.x / magn,
				v.y / magn
			);
		}
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, const float& t, const easing::EaseType& easeType = easing::EaseType::None)
		{
			return Vector2(
				math::Lerp(v1.x, v2.x, Ease(easeType, t)),
				math::Lerp(v1.y, v2.y, Ease(easeType, t))
			);
		}
		static Vector2 RandomCireclePoint()
		{
			return Vector2(
				std::cosf(random::Float() * 2 * PI),
				std::sinf(random::Float() * 2 * PI)
			);
		}

		// Methods
		void Translate(const Vector2& v)
		{
			*this += v;
		}
		void Scale(const Vector2& v)
		{
			*this *= v;
		}
		void Scale(const float& f)
		{
			*this *= f;
		}
		float Magnitude() const
		{
			return Magnitude(*this);
		}
		float Determinant(const Vector2& v) const
		{
			return Determinant(*this, v);
		}
		float Distance(const Vector2& v) const
		{
			return Distance(*this, v);
		}
		Vector2 Normalized() const
		{
			return Normalize(*this);
		}
		Vector2 Lerp(const Vector2& v, const float& t, const easing::EaseType& easeType = easing::EaseType::None) const
		{
			return Lerp(*this, v, t, easeType);
		}

		// Operator when used with SFML
		static Vector2 From(sf::Vector2f vector) { return Vector2(vector.x, vector.y); }
		static Vector2 From(sf::Vector2i vector) { return Vector2(vector.x, vector.y); }
		operator sf::Vector2f() const { return { x, y }; }
		operator sf::Vector2i() const { return { (int)x, (int)y }; }
		operator std::string() const { return std::to_string(x) + "," + std::to_string(y); }

		// Operators
		Vector2& operator=(const Vector2& v2) = default;

		Vector2& operator+=(const Vector2& v2)
		{
			x += v2.x;
			y += v2.y;

			return *this;
		}
		Vector2& operator-=(const Vector2& v2)
		{
			x -= v2.x;
			y -= v2.y;

			return *this;
		}
		Vector2& operator*=(const Vector2& v2)
		{
			x *= v2.x;
			y *= v2.y;

			return *this;
		}
		Vector2& operator*=(const float& f)
		{
			x *= f;
			y *= f;

			return *this;
		}
		Vector2& operator/=(const Vector2& v2)
		{
			x /= v2.x;
			y /= v2.y;

			return *this;
		}
	};

	// Operators
	inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << v.x << "|" << v.y;
		return os;
	}

	inline bool operator==(const Vector2& v1, const Vector2& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	inline bool operator!=(const Vector2& v1, const Vector2& v2)
	{
		return !(v1 == v2);
	}

	inline Vector2 operator+(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x + v2.x, v1.y + v2.y);
	}
	inline Vector2 operator-(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}
	inline Vector2 operator*(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x * v2.x, v1.y * v2.y);
	}
	inline Vector2 operator*(const Vector2& v1, const float& f)
	{
		return Vector2(v1.x * f, v1.y * f);
	}
	inline Vector2 operator/(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x / v2.x, v1.y / v2.y);
	}
	inline Vector2 operator/(const Vector2& v1, const float& f)
	{
		return Vector2(v1.x / f, v1.y / f);
	}
}