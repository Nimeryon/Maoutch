#pragma once

namespace sf
{
	class VertexArray;
	class Color;
}

#include <vector>

#include "../Objects/GameObject.h"
#include "../../Types/Vector2i.h"
#include "../../Types/EmitterValue.h"

namespace  maoutch
{
	const std::string particlesPath = "Assets\\Particles\\";
	
	struct Particle;

	class ParticleEmitter : public GameObject
	{
	public:
		explicit ParticleEmitter(
			int maxParticleCount = 1000,
			bool playOnAwake = false,
			bool isLooping = true,
			bool isTextured = false,
			float emitterLifetime = 5.f,
			float emitterEmissionSpeed = .025f,
			float minParticleLifetime = 1.f,
			float maxParticleLifetime = 2.f,
			float minParticleStartRotation = 0.f,
			float maxParticleStartRotation = 0.f,
			EmitterValue<Vector2> particleDirection = EmitterValue<Vector2>(Vector2(-1, 1), Vector2(-1, 1), 1),
			EmitterValue<sf::Color> particleColor = {},
			EmitterValue<float> particleSize = { 5 },
			EmitterValue<float> particleSpeed = { 50 },
			EmitterValue<float> particleFriction = {0 },
			EmitterValue<float> particleGravity = { 0 },
			EmitterValue<float> particleRotationSpeed = { 0 }
		);

		// GameObject properties

		void Update(float dt) override;
		void FixedUpdate(float dt) override;
		void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) override;

		// Emitter properties

		void Reset();
		void Play();
		void Stop();

		bool IsPlaying() const;
		void SpawnParticle();

		void SaveToFile(std::string fileName);
		void SetupFromFile(std::string fileName);

		// Emitter Getter/Setter
		bool* PlayOnAwake();
		bool* IsPlaying();
		bool* IsLooping();
		bool* IsTextured();
		float* GetLifetime();
		float* GetCurrentLifetime();
		float* GetEmissionSpeed();
		std::string& GetTextureName();
		std::vector<Vector2i>& GetTexturePositions();
		Vector2i& GetTextureRectSize();

		Vector2i GetRandomTexturePosition() const;

		void AddTexturePosition(const Vector2i& position);
		void RemoveTexturePosition();

		void SetTextureName(const std::string& textureName);
		void SetTextureRectSize(const Vector2i& rectSize);
		void ReserveParticlesSpace();

		// Particles Getter/Setter
		int GetCurrentParticleCount() const;
		int* GetMaxParticleCount();
		float* GetMinParticleLifetime();
		float* GetMaxParticleLifetime();
		float* GetMinParticleStartRotation();
		float* GetMaxParticleStartRotation();
		EmitterValue<Vector2>* GetParticleDirection();
		EmitterValue<sf::Color>* GetParticleColors();
		EmitterValue<float>* GetParticleSize();
		EmitterValue<float>* GetParticleSpeed();
		EmitterValue<float>* GetParticleFriction();
		EmitterValue<float>* GetParticleGravity();
		EmitterValue<float>* GetParticleRotationSpeed();

	private:
		std::vector<Particle> _particles;
		sf::VertexArray _particleVertex;

		// Emitter parameters
		int _emitterMaxParticleCount;
		bool _emitterPlayOnAwake;
		bool _emitterIsPlaying;
		bool _emitterIsLooping;
		bool _emitterIsTextured;
		std::string _emitterTextureName;
		std::vector<Vector2i> _emitterTexturePositions;
		Vector2i _emitterTextureRectSize;
		float _emitterCurrentLifetime, _emitterLifetime;
		float _emitterEmissionSpeed;
		float _emitterEmissionAccumulator;

		// Particle parameters
		float _minParticleLifetime, _maxParticleLifetime;
		float _minParticleStartRotation, _maxParticleStartRotation;
		EmitterValue<Vector2> _particleDirection;
		EmitterValue<sf::Color> _particleColors;
		EmitterValue<float> _particleSize;
		EmitterValue<float> _particleSpeed;
		EmitterValue<float> _particleFriction;
		EmitterValue<float> _particleGravity;
		EmitterValue<float> _particleRotationSpeed;
	};
}
