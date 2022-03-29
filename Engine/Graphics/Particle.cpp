#include <SFML/Graphics.hpp>
#include <array>

#include "Particle.h"
#include "../../Tools/Texture.h"
#include "../../Types/Vector2i.h"

namespace maoutch
{
	Particle::Particle(
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
	) :
		_isTextured(isTextured),
		_texturePosition(texturePosition),
		_textureRectSize(textureRectSize),
		_position(position),
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
		std::array<sf::Vertex, 4> vertices;

		float t = GetLifePercent();
		float half_size = _size.GetValue(t) / 2.f;

		// Position
		vertices[0].position = { _position.x - half_size, _position.y - half_size };
		vertices[1].position = { _position.x + half_size, _position.y - half_size };
		vertices[2].position = { _position.x + half_size, _position.y + half_size };
		vertices[3].position = { _position.x - half_size, _position.y + half_size };

		if (_rotationSpeed.GetValue() != 0)
		{
			sf::Transform transform;
			transform.rotate(_currentRotation, _position);
			vertices[0].position = transform.transformPoint(vertices[0].position);
			vertices[1].position = transform.transformPoint(vertices[1].position);
			vertices[2].position = transform.transformPoint(vertices[2].position);
			vertices[3].position = transform.transformPoint(vertices[3].position);
		}

		// Color
		sf::Color c = _color.GetValue(t);
		vertices[0].color = c;
		vertices[1].color = c;
		vertices[2].color = c;
		vertices[3].color = c;

		if (_isTextured) texture::SetTextureCoord(vertices, _texturePosition, _textureRectSize);

		for (sf::Vertex& vertex : vertices)
			vertexArray.append(vertex);
	}
}
