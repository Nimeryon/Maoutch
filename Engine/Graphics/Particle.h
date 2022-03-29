#pragma once
namespace sf
{ 
	class Color;
}

#include "../../Types/Vector2i.h"
#include "../../Types/LerpableValue.h"

namespace maoutch
{
	struct Particle
	{
		explicit Particle(
			Vector2 position,
			float lifetime,
			float startRotation,
			LerpableValue<Vector2> direction,
			LerpableValue<sf::Color> color,
			LerpableValue<float> size,
			LerpableValue<float> speed,
			LerpableValue<float> friction,
			LerpableValue<float> gravity,
			LerpableValue<float> rotationSpeed,
			bool isTextured,
			Vector2i texturePosition,
			Vector2i textureRectSize
		);
		~Particle();
		
		void Update(float dt);
		
		bool IsAlive() const;
		float GetLifePercent() const;
		void WriteVertices(sf::VertexArray& vertexArray);

	private:
		bool _isTextured;
		Vector2i _texturePosition;
		Vector2i _textureRectSize;
		Vector2 _position;
		float _currentLifetime;
		float _lifetime;
		float _currentRotation;
		LerpableValue<Vector2> _direction;
		LerpableValue<sf::Color> _color;
		LerpableValue<float> _size;
		LerpableValue<float> _speed;
		LerpableValue<float> _friction;
		LerpableValue<float> _gravity;
		LerpableValue<float> _rotationSpeed;
	};
}