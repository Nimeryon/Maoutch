#pragma once
#include <mutex>

#include "Effect.h"
#include "../../Types/Vector2.h"

namespace maoutch
{
	class ITransformable;

	struct SquishData : EffectData
	{
		SquishData(
			ITransformable* object,
			const float& time,
			const float& timeOut,
			const Vector2& scale,
			const easing::EaseType& inEaseType = easing::EaseType::None,
			const easing::EaseType& outEaseType = easing::EaseType::None,
			const bool& isLooping = false
		);

		bool in;
		float currentOutTime;
		float timeOut;
		Vector2 initialScale;
		Vector2 scale;
		easing::EaseType inEaseType;
		easing::EaseType outEaseType;

		void operator=(const SquishData& data);
	};

	class Squisher : public Effect<SquishData>
	{
	public:
		static Squisher* Instance();

		Squisher(Squisher const&) = delete;
		void operator=(Squisher const&) = delete;

	protected:
		Squisher();
		~Squisher() override;

	private:
		static Squisher* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _UpdateEffect(const float& dt, SquishData& data, ITransformable* transformable) override;
	};
}