#pragma once
#include <mutex>

#include "Effect.h"
#include "../../Types/Vector2.h"

namespace maoutch
{
	class ITransformable;

	struct ShakeData : EffectData
	{
		ShakeData(ITransformable* object, const float& time, const float& maxRadius = 1.f, const float& magnitude = 1.f, const float& decreaseFactor = 0.f);
		
		Vector2 initialPosition;
		float maxRadius;
		float magnitude;
		float decreaseFactor;

		void operator=(const ShakeData& data);
	};

	class Shaker : public Effect<ShakeData>
	{
	public:
		static Shaker* Instance();

		Shaker(Shaker const&) = delete;
		void operator=(Shaker const&) = delete;

	protected:
		Shaker();
		~Shaker();

	private:
		static Shaker* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _UpdateEffect(const float& dt, ShakeData& data, ITransformable* transformable) override;
	};
}