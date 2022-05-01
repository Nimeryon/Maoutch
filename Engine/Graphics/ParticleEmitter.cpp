#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>

#include "json.hpp"
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
		_finishedPlaying(false),
		_hasPlayed(false),
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
		_textureName = Assets::Instance()->GetTexturesMap().begin()->first;
		_particleVertex.setPrimitiveType(sf::Quads);

		if (_playOnAwake) Play();
	}
	ParticleEmitter::~ParticleEmitter() = default;

	void ParticleEmitter::Update(float dt)
	{
		// Handle playing
		if (!_finishedPlaying && _isPlaying)
		{
			_currentLifetime += dt;
			if (_currentLifetime > _lifetime)
			{
				if (_destroyAfterPlaying)
					_finishedPlaying = true;

				if (_isLooping) _currentLifetime -= _lifetime;
				else
				{
					_isPlaying = false;
					_finishedPlaying = true;
				}

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
		if (GetCurrentParticleCount() == 0 && _finishedPlaying)
		{
			if (_hasPlayed && _destroyAfterPlaying) GameObjectHandler::Instance()->Destroy(this);
			return;
		}

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
		//_renderState.transform = transform;
		_renderState.texture = _isTextured ? &Assets::Instance()->GetTexture(_textureName) : nullptr;
		
		window.draw(_particleVertex, _renderState);
	}

	void ParticleEmitter::Reset()
	{
		ReserveParticlesSpace();
		_currentMaxParticleCount = _maxParticleCount;
		_isPlaying = _playOnAwake;
		_finishedPlaying = false;
		_currentLifetime = 0;
		_emissionAccumulator = 0;
		_particleVertex.clear();
	}
	void ParticleEmitter::Play()
	{
		if (!_isPlaying && _currentLifetime >= _lifetime) Reset();
		_hasPlayed = true;
		_isPlaying = true;
		_finishedPlaying = false;
	}
	void ParticleEmitter::Stop()
	{
		_isPlaying = false;
		_finishedPlaying = true;
	}

	bool ParticleEmitter::IsPlaying() const { return _isPlaying; }

	void ParticleEmitter::SpawnParticle()
	{
		_particles.emplace_back(Particle(
			GetGlobalPosition(),
			GetScale(),
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

	void ParticleEmitter::SaveToFile(const std::string& fileName, bool destroyAfterPlaying)
	{
		nlohmann::json json;
		std::ofstream file(Assets::Config<std::string>("Particle", "Path") + fileName + ".json");
		if (file.is_open())
		{
			std::vector<std::string> positions;
			for (Vector2i& position : _texturePositions)
				positions.emplace_back(position);

			json["MaxParticleCount"] = _maxParticleCount;
			json["PlayOnAwake"] = _playOnAwake;
			json["DestroyAfterPlaying"] = destroyAfterPlaying;
			json["RespawnDeadParticle"] = _respawnDeadParticle;
			json["IsLooping"] = _isLooping;
			json["IsTextured"] = _isTextured;
			json["Texture"] = _textureName;
			json["TexturePositions"] = positions;
			json["TextureRect"] = (std::string)_textureRectSize;
			json["Lifetime"] = _lifetime;
			json["EmissionSpeed"] = _emissionSpeed;
			json["MinParticleLifetime"] = _minParticleLifetime;
			json["MaxParticleLifetime"] = _maxParticleLifetime;
			json["Direction"] = _particleDirection.ToJson();
			json["Colors"] = _particleColors.ToJson();
			json["Size"] = _particleSize.ToJson();
			json["Speed"] = _particleSpeed.ToJson();
			json["Friction"] = _particleFriction.ToJson();
			json["Gravity"] = _particleGravity.ToJson();
			json["MinStartRotation"] = _minParticleStartRotation;
			json["MaxStartRotation"] = _maxParticleStartRotation;
			json["RotationSpeed"] = _particleRotationSpeed.ToJson();

			file << std::setw(4) << json << std::endl;
			file.close();
		}
	}
	void ParticleEmitter::SetupFromFile(const std::string& fileName)
	{
		std::string parsedFileName = string::Replace(fileName, Assets::Config<std::string>("Particle", "Path"), "");
		std::ifstream file(Assets::Config<std::string>("Particle", "Path") + parsedFileName);
		nlohmann::json json = nlohmann::json::parse(file);

		std::vector<std::string> positions = json["TexturePositions"].get<std::vector<std::string>>();
		std::vector<Vector2i> vectorPositions;
		for (std::string& position : positions)
			vectorPositions.emplace_back(Vector2i::FromString(position));

		_maxParticleCount = json["MaxParticleCount"].get<int>();
		_playOnAwake = json["PlayOnAwake"].get<bool>();
		_destroyAfterPlaying = json["DestroyAfterPlaying"].get<bool>();
		_respawnDeadParticle = json["RespawnDeadParticle"].get<bool>();
		_isLooping = json["IsLooping"].get<bool>();
		_isTextured = json["IsTextured"].get<bool>();
		_textureName = json["Texture"].get<std::string>();
		_texturePositions = vectorPositions;
		_textureRectSize = Vector2i::FromString(json["TextureRect"].get<std::string>());
		_lifetime = json["Lifetime"].get<float>();
		_emissionSpeed = json["EmissionSpeed"].get<float>();
		_minParticleLifetime = json["MinParticleLifetime"].get<float>();
		_maxParticleLifetime = json["MaxParticleLifetime"].get<float>();
		_particleDirection.FromJson(json["Direction"].get<nlohmann::json>());
		_particleColors.FromJson(json["Colors"].get<nlohmann::json>());
		_particleSize.FromJson(json["Size"].get<nlohmann::json>());
		_particleSpeed.FromJson(json["Speed"].get<nlohmann::json>());
		_particleFriction.FromJson(json["Friction"].get<nlohmann::json>());
		_particleGravity.FromJson(json["Gravity"].get<nlohmann::json>());
		_minParticleStartRotation = json["MinStartRotation"].get<float>();
		_maxParticleStartRotation = json["MaxStartRotation"].get<float>();
		_particleRotationSpeed.FromJson(json["RotationSpeed"].get<nlohmann::json>());
		
		if (_playOnAwake) Play();
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
	void ParticleEmitter::SetDestroyAfterPlaying(const bool& destroyAfterPlaying) { _destroyAfterPlaying = destroyAfterPlaying; }
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
