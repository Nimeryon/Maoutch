#include <SFML/Graphics.hpp>
#include <fstream>

#include "ParticleEmitter.h"
#include "Particle.h"
#include "../../Tools/Random.h"
#include "../../Tools/Colors.h"
#include "../../Tools/String.h"
#include "../AssetLoader.h"

namespace maoutch
{
	ParticleEmitter::ParticleEmitter(
		int particleCount,
		bool playOnAwake,
		bool isLooping,
		bool isTextured,
		float emitterLifetime,
		float emitterEmissionSpeed,
		float minParticleLifetime,
		float maxParticleLifetime,
		float minParticleStartRotation,
		float maxParticleStartRotation,
		EmitterValue<Vector2> particleDirection,
		EmitterValue<sf::Color> particleColor,
		EmitterValue<float> particleSize,
		EmitterValue<float> particleSpeed,
		EmitterValue<float> particleFriction,
		EmitterValue<float> particleGravity,
		EmitterValue<float> particleRotationSpeed
	) :
		GameObject("ParticleEmitter"),
		_emitterMaxParticleCount(particleCount),
		_emitterPlayOnAwake(playOnAwake),
		_emitterIsPlaying(playOnAwake),
		_emitterIsLooping(isLooping),
		_emitterIsTextured(isTextured),
		_emitterCurrentLifetime(0),
		_emitterLifetime(emitterLifetime),
		_emitterEmissionSpeed(emitterEmissionSpeed),
		_emitterEmissionAccumulator(0),
		_minParticleLifetime(minParticleLifetime),
		_maxParticleLifetime(maxParticleLifetime),
		_minParticleStartRotation(minParticleStartRotation),
		_maxParticleStartRotation(maxParticleStartRotation),
		_particleDirection(particleDirection),
		_particleColors(particleColor),
		_particleSize(particleSize),
		_particleSpeed(particleSpeed),
		_particleFriction(particleFriction),
		_particleGravity(particleGravity),
		_particleRotationSpeed(particleRotationSpeed)
	{
		ReserveParticlesSpace();
		_emitterTextureName = AssetLoader::GetInstance()->GetTexturesMap().begin()->first;
		_particleVertex.setPrimitiveType(sf::Quads);
	}

	void ParticleEmitter::Update(float dt)
	{
		// Handle playing
		if (_emitterIsPlaying)
		{
			_emitterCurrentLifetime += dt;
			if (_emitterCurrentLifetime > _emitterLifetime)
			{
				if (_emitterIsLooping) _emitterCurrentLifetime -= _emitterLifetime;
				else _emitterIsPlaying = false;
			}

			// Handle particle spawn
			if (_emitterEmissionSpeed == 0.f)
				for (int i = 0; i < _emitterMaxParticleCount - GetCurrentParticleCount(); ++i)
					SpawnParticle();
			else
			{
				_emitterEmissionAccumulator += dt;
				while (GetCurrentParticleCount() < _emitterMaxParticleCount && _emitterEmissionAccumulator > _emitterEmissionSpeed)
				{
					SpawnParticle();
					_emitterEmissionAccumulator -= _emitterEmissionSpeed;
				}
			}
		}
	}
	void ParticleEmitter::FixedUpdate(float dt)
	{
		if (GetCurrentParticleCount() == 0 && !_emitterIsPlaying) return;

		// Delete deactivated particles
		auto particlesToRemove = std::remove_if(_particles.begin(), _particles.end(), [](const Particle& particle) { return !particle.IsAlive(); });
		_particles.erase(particlesToRemove, _particles.end());

		// Handle particle and vertex updates
		_particleVertex.clear();
		for (int i = 0; i < _particles.size(); ++i)
		{
			_particles[i].Update(dt);
			_particles[i].WriteVertices(_particleVertex);
		}
	}

	void ParticleEmitter::_OnDraw(sf::RenderWindow& window, const sf::Transform& transform)
	{
		if (_emitterIsTextured)
			window.draw(_particleVertex, &AssetLoader::GetInstance()->GetTexture(_emitterTextureName));
		else
			window.draw(_particleVertex);
	}

	void ParticleEmitter::Reset()
	{
		ReserveParticlesSpace();
		_emitterIsPlaying = _emitterPlayOnAwake;
		_emitterCurrentLifetime = 0;
		_emitterEmissionAccumulator = 0;
		_particleVertex.clear();
	}
	void ParticleEmitter::Play()
	{
		if (!_emitterIsPlaying && _emitterCurrentLifetime >= _emitterLifetime) Reset();
		_emitterIsPlaying = true;
	}
	void ParticleEmitter::Stop() { _emitterIsPlaying = false; }

	bool ParticleEmitter::IsPlaying() const { return _emitterIsPlaying; }

	void ParticleEmitter::SpawnParticle()
	{
		_particles.emplace_back(Particle(
			GetPosition(),
			random::Float(_minParticleLifetime, _maxParticleLifetime),
			random::Float(_minParticleStartRotation, _maxParticleStartRotation),
			_particleDirection.GetValue(),
			_particleColors.GetValue(),
			_particleSize.GetValue(),
			_particleSpeed.GetValue(),
			_particleFriction.GetValue(),
			_particleGravity.GetValue(),
			_particleRotationSpeed.GetValue(),
			_emitterIsTextured,
			GetRandomTexturePosition(),
			_emitterTextureRectSize
		));
	}

	void ParticleEmitter::SaveToFile(std::string fileName)
	{
		std::ofstream file(particlesPath + fileName);
		if (file.is_open())
		{
			file << _emitterMaxParticleCount << std::endl;
			file << _emitterPlayOnAwake << "," << _emitterIsLooping << "," << _emitterIsTextured << std::endl;
			file << _emitterTextureName << std::endl;
			file << _emitterTexturePositions.size() << std::endl;
			for (Vector2i& position : _emitterTexturePositions)
				file << position << std::endl;
			file << _emitterTextureRectSize << std::endl;
			file << _emitterLifetime << std::endl;
			file << _emitterEmissionSpeed << std::endl;
			file << _minParticleLifetime << "," << _maxParticleLifetime << std::endl;
			file << _minParticleStartRotation << "," << _maxParticleStartRotation << std::endl;
			file << _particleDirection << std::endl;
			file << _particleColors << std::endl;
			file << _particleSize << std::endl;
			file << _particleSpeed << std::endl;
			file << _particleFriction << std::endl;
			file << _particleGravity << std::endl;
			file << _particleRotationSpeed;
			file.close();
		}
	}
	void ParticleEmitter::SetupFromFile(std::string fileName)
	{
		std::ifstream file(fileName);
		std::string line;

		// Particle count
		getline(file, line);
		_emitterMaxParticleCount = std::atoi(&line[0]);

		getline(file, line);
		std::vector<std::string> values = string::Split(line, ',');
		_emitterPlayOnAwake = values[0] == "1"; // Play On Awake
		_emitterIsLooping = values[1] == "1"; // Is Looping
		_emitterIsTextured = values[2] == "1"; // Is Textured
		if (_emitterPlayOnAwake) _emitterIsPlaying = true;

		getline(file, line);
		_emitterTextureName = line; // Texture name

		getline(file, line);
		int positionCount = std::atoi(&line[0]);
		_emitterTexturePositions.reserve(positionCount); // Texture Positions
		for (int i = 0; i < positionCount; ++i)
		{
			getline(file, line);
			values = string::Split(line, ',');
			_emitterTexturePositions.emplace_back(Vector2i(std::atoi(&values[0][0]), std::atoi(&values[1][0]))); // Add Texture Position
		}

		getline(file, line);
		values = string::Split(line, ',');
		_emitterTextureRectSize = Vector2i(std::atoi(&values[0][0]), std::atoi(&values[1][0])); // Texture Rect Size

		getline(file, line);
		_emitterLifetime = std::atof(&line[0]); // Lifetime
		getline(file, line);
		_emitterEmissionSpeed = std::atof(&line[0]); // Emission Speed

		getline(file, line);
		values = string::Split(line, ',');
		_minParticleLifetime = std::atof(&values[0][0]); // Min Particle Lifetime
		_maxParticleLifetime = std::atof(&values[1][0]); // Max Particle Lifetime
		getline(file, line);
		values = string::Split(line, ',');
		_minParticleStartRotation = std::atof(&values[0][0]); // Min Particle Start Rotation
		_maxParticleStartRotation = std::atof(&values[1][0]); // Max Particle Start Rotation

		getline(file, line);
		_particleDirection.FromString(line); // Particle Direction
		getline(file, line);
		_particleColors.FromString(line); // Particle Color
		getline(file, line);
		_particleSize.FromString(line); // Particle Size
		getline(file, line);
		_particleSpeed.FromString(line); // Particle Speed
		getline(file, line);
		_particleFriction.FromString(line); // Particle Friction
		getline(file, line);
		_particleGravity.FromString(line); // Particle Gravity
		getline(file, line);
		_particleRotationSpeed.FromString(line); // Particle Rotation Speed
	}

	// Emitter Getter/Setter
	bool* ParticleEmitter::PlayOnAwake() { return &_emitterPlayOnAwake; }
	bool* ParticleEmitter::IsPlaying() { return &_emitterIsPlaying; }
	bool* ParticleEmitter::IsLooping() { return &_emitterIsLooping; }
	bool* ParticleEmitter::IsTextured() { return &_emitterIsTextured; }
	float* ParticleEmitter::GetLifetime() { return &_emitterLifetime; }
	float* ParticleEmitter::GetCurrentLifetime() { return &_emitterCurrentLifetime; }
	float* ParticleEmitter::GetEmissionSpeed() { return &_emitterEmissionSpeed; }
	std::string& ParticleEmitter::GetTextureName() { return _emitterTextureName; }
	std::vector<Vector2i>& ParticleEmitter::GetTexturePositions() { return _emitterTexturePositions; }
	Vector2i& ParticleEmitter::GetTextureRectSize() { return _emitterTextureRectSize; }

	Vector2i ParticleEmitter::GetRandomTexturePosition() const
	{
		if (_emitterTexturePositions.empty()) return Vector2i::Zero();
		return _emitterTexturePositions[random::Int(0, _emitterTexturePositions.size())];
	}

	void ParticleEmitter::AddTexturePosition(const Vector2i& position) { _emitterTexturePositions.emplace_back(position);	}
	void ParticleEmitter::RemoveTexturePosition() { _emitterTexturePositions.pop_back(); }

	void ParticleEmitter::SetTextureName(const std::string& textureName) { _emitterTextureName = textureName; }
	void ParticleEmitter::SetTextureRectSize(const Vector2i& rectSize) { _emitterTextureRectSize = rectSize; }
	void ParticleEmitter::ReserveParticlesSpace()
	{
		_particles.clear();
		_particles.reserve(_emitterMaxParticleCount);
	}

	// Particles Getter/Setter
	int ParticleEmitter::GetCurrentParticleCount() const { return _particles.size(); }
	int* ParticleEmitter::GetMaxParticleCount() { return &_emitterMaxParticleCount; }
	float* ParticleEmitter::GetMinParticleLifetime() { return &_minParticleLifetime; }
	float* ParticleEmitter::GetMaxParticleLifetime() { return &_maxParticleLifetime; }
	float* ParticleEmitter::GetMinParticleStartRotation() { return &_minParticleStartRotation; }
	float* ParticleEmitter::GetMaxParticleStartRotation() { return &_maxParticleStartRotation; }
	EmitterValue<Vector2>* ParticleEmitter::GetParticleDirection() { return &_particleDirection; }
	EmitterValue<sf::Color>* ParticleEmitter::GetParticleColors() { return &_particleColors; }
	EmitterValue<float>* ParticleEmitter::GetParticleSize() { return &_particleSize; }
	EmitterValue<float>* ParticleEmitter::GetParticleSpeed() { return &_particleSpeed; }
	EmitterValue<float>* ParticleEmitter::GetParticleFriction() { return &_particleFriction; }
	EmitterValue<float>* ParticleEmitter::GetParticleGravity() { return &_particleGravity; }
	EmitterValue<float>* ParticleEmitter::GetParticleRotationSpeed() { return &_particleRotationSpeed; }
}
