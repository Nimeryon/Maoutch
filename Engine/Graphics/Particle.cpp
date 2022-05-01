#include <SFML/Graphics.hpp>
#include <array>

#include "Particle.h"
#include "../../Tools/Texture.h"
#include "../../Tools/Transform.h"
#include "../../Types/Vector2i.h"

namespace maoutch
{
	Particle::Particle(
		Vector2 position,
		Vector2 scale,
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
	) :
		_isTextured(isTextured),
		_texturePosition(texturePosition),
		_textureRectSize(textureRectSize),
		_position(position),
		_scale(scale),
		_currentLifetime(0),
		_lifetime(lifetime),
		_currentRotation(startRotation),
		_direction(direction),
		_color(color),
		_size(size),
		_speed(speed),
		_friction(friction),
		_gravity(gravity),
		_rotationSpeed(rotationSpeed) {}
	Particle::~Particle() = default;
	
	void Particle::Update(float dt)
	{
		_currentLifetime += dt;
		float t = GetLifePercent();

		Vector2 drift = {
			_friction.GetValue(t)* dt,
			_gravity.GetValue(t)* dt
		};
		_direction.startVector += drift;
		_direction.endVector += drift;

		_currentRotation += _rotationSpeed.GetValue(t) * dt;
		_position += _direction.GetValue(t) * _speed.GetValue(t) * dt;
	}
	
	bool Particle::IsAlive() const { return _currentLifetime < _lifetime; }
	float Particle::GetLifePercent() const { return (1.f / _lifetime) * _currentLifetime; }
	void Particle::WriteVertices(sf::VertexArray& vertexArray)
	{
		const float t = GetLifePercent();
		const Vector2 size = _size.GetValue(t) * _scale;
		
		std::array<sf::Vertex, 4> vertices = transform::GetVertices({ _position - size / 2.f, size });
		if (_currentRotation != 0)
		{
			sf::Transform transform;
			transform.rotate(_currentRotation, _position);
			for (int i = 0; i < 4; ++i)			
				vertices[i].position = transform.transformPoint(vertices[i].position);
		}

		// Color
		const sf::Color c = _color.GetValue(t);
		for (int i = 0; i < 4; ++i)
			vertices[i].color = c;

		if (_isTextured) texture::SetTextureCoord(vertices, _texturePosition, _textureRectSize);

		for (sf::Vertex& vertex : vertices)
			vertexArray.append(vertex);
	}
}
