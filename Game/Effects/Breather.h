#pragma once
#include <mutex>

#include "Effect.h"
#include "../../Types/Vector2.h"

namespace maoutch
{
	class ITransformable;

	struct BreathData : EffectData
	{
		BreathData(
			ITransformable* object,
			const float& time,
			const Vector2& scale,
			const easing::EaseType& inEaseType = easing::EaseType::None,
			const easing::EaseType& outEaseType = easing::EaseType::None,
			const bool& isLooping = true
		);

		bool in;
		Vector2 initialScale;
		Vector2 scale;
		easing::EaseType inEaseType;
		easing::EaseType outEaseType;

		void operator=(const BreathData& data);
	};

	class Breather : public Effect<BreathData>
	{
	public:
		static Breather* Instance();

		Breather(Breather const&) = delete;
		void operator=(Breather const&) = delete;

	protected:
		Breather();
		~Breather() override;

	private:
		static Breather* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _UpdateEffects(const float& dt) override;
	};
}