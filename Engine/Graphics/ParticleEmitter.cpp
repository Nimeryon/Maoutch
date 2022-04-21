#include <SFML/Graphics.hpp>
#include <fstream>

#include "ParticleEmitter.h"
#include "Particle.h"
#include "../Objects/GameObjectHandler.h"
#include "../../Tools/Random.h"
#include "../../Tools/String.h"
#include "../Assets.h"

namespace maoutch
{
	ParticleEmitter::ParticleEmitter(
		int particleCount,
		bool playOnAwake,
		bool destroyAfterPlaying,
		bool respawnDeadParticle,
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
		_maxParticleCount(particleCount),
		_currentMaxParticleCount(_maxParticleCount),
		_playOnAwake(playOnAwake),
		_destroyAfterPlaying(destroyAfterPlaying),
		_respawnDeadParticle(respawnDeadParticle),
		_isPlaying(playOnAwake),
		_isLooping(isLooping),
		_isTextured(isTextured),
		_currentLifetime(0),
		_lifetime(emitterLifetime),
		_emissionSpeed(emitterEmissionSpeed),
		_emissionAccumulator(0),
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
		_textureName = Assets::GetInstance()->GetTexturesMap().begin()->first;
		_particleVertex.setPrimitiveType(sf::Quads);
	}
	ParticleEmitter::~ParticleEmitter() = default;

	void ParticleEmitter::Update(float dt)
	{
		// Handle playing
		if (_isPlaying)
		{
			_currentLifetime += dt;
			if (_currentLifetime > _lifetime)
			{
				if (_destroyAfterPlaying)
					GameObjectHandler::GetInstance()->Destroy(this);

				if (_isLooping) _currentLifetime -= _lifetime;
				else _isPlaying = false;

				_currentMaxParticleCount = _maxParticleCount;
			}
			else
			{
				// Handle particle spawn
				if (_emissionSpeed == 0.f)
					for (int i = 0; i < _maxParticleCount - GetCurrentParticleCount(); ++i)
					{
						if (_respawnDeadParticle || _currentMaxParticleCount > 0)
						{
							SpawnParticle();
							_currentMaxParticleCount--;
						}
					}
				else
				{
					_emissionAccumulator += dt;
					while (GetCurrentParticleCount() < _maxParticleCount && _emissionAccumulator > _emissionSpeed)
					{
						if (_respawnDeadParticle || _currentMaxParticleCount > 0)
						{
							SpawnParticle();
							_currentMaxParticleCount--;
						}
						_emissionAccumulator -= _emissionSpeed;
					}
				}
			}
		}
	}
	void ParticleEmitter::FixedUpdate(float dt)
	{
		if (GetCurrentParticleCount() == 0 && !_isPlaying) return;

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
		_renderState.transform = transform;
		_renderState.texture = _isTextured ? &Assets::GetInstance()->GetTexture(_textureName) : nullptr;
		
		window.draw(_particleVertex, _renderState);
	}

	void ParticleEmitter::Reset()
	{
		ReserveParticlesSpace();
		_currentMaxParticleCount = _maxParticleCount;
		_isPlaying = _playOnAwake;
		_currentLifetime = 0;
		_emissionAccumulator = 0;
		_particleVertex.clear();
	}
	void ParticleEmitter::Play()
	{
		if (!_isPlaying && _currentLifetime >= _lifetime) Reset();
		_isPlaying = true;
	}
	void ParticleEmitter::Stop() { _isPlaying = false; }

	bool ParticleEmitter::IsPlaying() const { return _isPlaying; }

	void ParticleEmitter::SpawnParticle()
	{
		_particles.emplace_back(Particle(
			random::Float(_minParticleLifetime, _maxParticleLifetime),
			random::Float(_minParticleStartRotation, _maxParticleStartRotation),
			_particleDirection.GetValue(),
			_particleColors.GetValue(),
			_particleSize.GetValue(),
			_particleSpeed.GetValue(),
			_particleFriction.GetValue(),
			_particleGravity.GetValue(),
			_particleRotationSpeed.GetValue(),
			_isTextured,
			GetRandomTexturePosition(),
			_textureRectSize
		));
	}

	void ParticleEmitter::SaveToFile(const std::string& fileName)
	{
		std::ofstream file(Assets::Config<std::string>("Particle", "Path") + fileName);
		if (file.is_open())
		{
			file << _maxParticleCount << std::endl;
			file << _playOnAwake << "," << _destroyAfterPlaying << "," << _respawnDeadParticle << "," << _isLooping << "," << _isTextured << std::endl;
			file << _textureName << std::endl;
			file << _texturePositions.size() << std::endl;
			for (Vector2i& position : _texturePositions)
				file << position << std::endl;
			file << _textureRectSize << std::endl;
			file << _lifetime << std::endl;
			file << _emissionSpeed << std::endl;
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
	void ParticleEmitter::SetupFromFile(const std::string& fileName, bool canDestroy)
	{
		std::ifstream file(fileName);
		std::string line;

		// Particle count
		getline(file, line);
		_maxParticleCount = std::atoi(&line[0]);
		_currentMaxParticleCount = _maxParticleCount;

		getline(file, line);
		std::vector<std::string> values = string::Split(line, ',');
		_playOnAwake = values[0] == "1"; // Play On Awake
		_destroyAfterPlaying = canDestroy ? values[1] == "1" : false; // Destroy After Playing
		_respawnDeadParticle = values[2] == "1"; // Respawn Dead Particle
		_isLooping = values[3] == "1"; // Is Looping
		_isTextured = values[4] == "1"; // Is Textured
		if (_playOnAwake) _isPlaying = true;

		getline(file, line);
		_textureName = line; // Texture name

		getline(file, line);
		int positionCount = std::atoi(&line[0]);
		_texturePositions.clear();
		_texturePositions.reserve(positionCount); // Texture Positions
		for (int i = 0; i < positionCount; ++i)
		{
			getline(file, line);
			_texturePositions.emplace_back(Vector2i::FromString(line)); // Add Texture Position
		}

		getline(file, line);
		_textureRectSize = Vector2i(Vector2i::FromString(line)); // Texture Rect Size

		getline(file, line);
		_lifetime = std::atof(&line[0]); // Lifetime
		getline(file, line);
		_emissionSpeed = std::atof(&line[0]); // Emission Speed

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
	bool* ParticleEmitter::PlayOnAwake() { return &_playOnAwake; }
	bool* ParticleEmitter::DestroyAfterPlaying() { return &_destroyAfterPlaying; }
	bool* ParticleEmitter::RespawnDeadParticle() { return &_respawnDeadParticle; }
	bool* ParticleEmitter::IsPlaying() { return &_isPlaying; }
	bool* ParticleEmitter::IsLooping() { return &_isLooping; }
	bool* ParticleEmitter::IsTextured() { return &_isTextured; }
	float* ParticleEmitter::GetLifetime() { return &_lifetime; }
	float* ParticleEmitter::GetCurrentLifetime() { return &_currentLifetime; }
	float* ParticleEmitter::GetEmissionSpeed() { return &_emissionSpeed; }
	std::string& ParticleEmitter::GetTextureName() { return _textureName; }
	std::vector<Vector2i>& ParticleEmitter::GetTexturePositions() { return _texturePositions; }
	Vector2i& ParticleEmitter::GetTextureRectSize() { return _textureRectSize; }

	Vector2i ParticleEmitter::GetRandomTexturePosition() const
	{
		if (_texturePositions.empty()) return Vector2i::Zero();
		return _texturePositions[random::Int(0, _texturePositions.size())];
	}

	void ParticleEmitter::AddTexturePosition(const Vector2i& position) { _texturePositions.emplace_back(position);	}
	void ParticleEmitter::RemoveTexturePosition() { _texturePositions.pop_back(); }

	void ParticleEmitter::SetTextureName(const std::string& textureName) { _textureName = textureName; }
	void ParticleEmitter::SetTextureRectSize(const Vector2i& rectSize) { _textureRectSize = rectSize; }
	void ParticleEmitter::ReserveParticlesSpace()
	{
		_particles.clear();
		_particles.reserve(_maxParticleCount);
	}

	// Particles Getter/Setter
	int ParticleEmitter::GetCurrentParticleCount() const { return _particles.size(); }
	int ParticleEmitter::GetCurrentMaxParticleCount() const { return _currentMaxParticleCount; }
	int* ParticleEmitter::GetMaxParticleCount() { return &_maxParticleCount; }
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
