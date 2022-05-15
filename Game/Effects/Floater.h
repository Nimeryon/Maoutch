#pragma once
#include <mutex>

#include "Effect.h"
#include "../../Types/Vector2.h"

namespace maoutch
{
	class ITransformable;

	struct FloaterData : EffectData
	{
		FloaterData(
			ITransformable* object,
			const float& time,
			const float& height,
			const easing::EaseType& inEaseType = easing::EaseType::None,
			const easing::EaseType& outEaseType = easing::EaseType::None,
			const bool& isLooping = true
		);

		bool in;
		float initialHeight;
		float height;
		easing::EaseType inEaseType;
		easing::EaseType outEaseType;

		void operator=(const FloaterData& data);
	};

	class Floater : public Effect<FloaterData>
	{
	public:
		static Floater* Instance();

		Floater(Floater const&) = delete;
		void operator=(Floater const&) = delete;

	protected:
		Floater();
		~Floater() override;

	private:
		static Floater* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _UpdateEffect(const float& dt, FloaterData& data, ITransformable* transformable) override;
	};
}