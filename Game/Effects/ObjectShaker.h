#pragma once
#include <mutex>
#include <vector>

#include "../../Types/Vector2.h"

namespace maoutch
{
	class GameObject;

	class ObjectShaker
	{
	public:
		ObjectShaker(ObjectShaker const&) = delete;
		void operator=(ObjectShaker const&) = delete;

		static void Shake(GameObject* object, const float& time, const float& maxRadius = 1.f, const float& magnitude = 1.f, const float& decreaseFactor = 0.f);

	protected:
		ObjectShaker();
		~ObjectShaker();

	private:
		static ObjectShaker* _GetInstance();

		static ObjectShaker* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _isShaking;

		struct ShakeData
		{
			ShakeData(GameObject* object, const float& time, const float& maxRadius, const float& magnitude, const float& decreaseFactor);

			GameObject* objectToShake;
			Vector2 initialPosition;
			float maxRadius;
			float magnitude;
			float decreaseFactor;
			float currentTime;
			float time;

			void operator=(const ShakeData& data);
		};
		std::vector<ShakeData> _objectsToShake;

		void _AddObjectToShake(ShakeData shakeData);
		void _Update(const float& dt);
	};
}