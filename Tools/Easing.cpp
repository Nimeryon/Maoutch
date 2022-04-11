#include <cmath>

#include "Easing.h"
#include "Math.h"

namespace maoutch
{
	namespace easing
	{
        float Ease(const EaseType& type, const float& t)
        {
	        switch (type)
	        {
				case EaseType::None: return t;
                case EaseType::EaseInSine: return EaseInSine(t);
                case EaseType::EaseOutSine: return EaseOutSine(t);
                case EaseType::EaseInOutSine: return EaseInOutSine(t);
                case EaseType::EaseInQuad: return EaseInQuad(t);
                case EaseType::EaseOutQuad: return EaseOutQuad(t);
                case EaseType::EaseInOutQuad: return EaseInOutQuad(t);
                case EaseType::EaseInCubic: return EaseInCubic(t);
                case EaseType::EaseOutCubic: return EaseOutCubic(t);
                case EaseType::EaseInOutCubic: return EaseInOutCubic(t);
                case EaseType::EaseInQuart: return EaseInQuart(t);
                case EaseType::EaseOutQuart: return EaseOutQuart(t);
                case EaseType::EaseInOutQuart: return EaseInOutQuart(t);
                case EaseType::EaseInQuint: return EaseInQuint(t);
                case EaseType::EaseOutQuint: return EaseOutQuint(t);
                case EaseType::EaseInOutQuint: return EaseInOutQuint(t);
                case EaseType::EaseInExpo: return EaseInExpo(t);
                case EaseType::EaseOutExpo: return EaseOutExpo(t);
                case EaseType::EaseInOutExpo: return EaseInOutExpo(t);
                case EaseType::EaseInCirc: return EaseInCirc(t);
                case EaseType::EaseOutCirc: return EaseOutCirc(t);
                case EaseType::EaseInOutCirc: return EaseInOutCirc(t);
                case EaseType::EaseInBack: return EaseInBack(t);
                case EaseType::EaseOutBack: return EaseOutBack(t);
                case EaseType::EaseInOutBack: return EaseInOutBack(t);
                case EaseType::EaseInElastic: return EaseInElastic(t);
                case EaseType::EaseOutElastic: return EaseOutElastic(t);
                case EaseType::EaseInOutElastic: return EaseInOutElastic(t);
                case EaseType::EaseInBounce: return EaseInBounce(t);
                case EaseType::EaseOutBounce: return EaseOutBounce(t);
                case EaseType::EaseInOutBounce: return EaseInOutBounce(t);
	        }   
        }

        float EaseInSine(float t)
		{
            return sinf(1.5707963f * t);
        }

        float EaseOutSine(float t)
		{
            return 1 + sinf(1.5707963f * (--t));
        }

        float EaseInOutSine(float t)
		{
            return 0.5f * (1 + sinf(3.1415926f * (t - 0.5f)));
        }

        float EaseInQuad(float t)
		{
            return t * t;
        }

        float EaseOutQuad(float t)
		{
            return t * (2 - t);
        }

        float EaseInOutQuad(float t)
		{
            return t < 0.5f ? 2 * t * t : t * (4 - 2 * t) - 1;
        }

        float EaseInCubic(float t)
		{
            return t * t * t;
        }

        float EaseOutCubic(float t)
		{
            return 1 + (--t) * t * t;
        }

        float EaseInOutCubic(float t)
		{
            return t < 0.5f ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
        }

        float EaseInQuart(float t)
		{
            t *= t;
            return t * t;
        }

        float EaseOutQuart(float t)
		{
            t = (--t) * t;
            return 1 - t * t;
        }

        float EaseInOutQuart(float t)
		{
            if (t < 0.5f)
            {
                t *= t;
                return 8 * t * t;
            }

            t = (--t) * t;
            return 1 - 8 * t * t;
        }

        float EaseInQuint(float t)
		{
            float t2 = t * t;
            return t * t2 * t2;
        }

        float EaseOutQuint(float t)
		{
            float t2 = (--t) * t;
            return 1 + t * t2 * t2;
        }

        float EaseInOutQuint(float t)
		{
            float t2;
            if (t < 0.5f)
            {
                t2 = t * t;
                return 16 * t * t2 * t2;
            }

            t2 = (--t) * t;
            return 1 + 16 * t * t2 * t2;
        }

        float EaseInExpo(float t)
		{
            return (powf(2, 8 * t) - 1) / 255;
        }

        float EaseOutExpo(float t)
		{
            return 1 - powf(2, -8 * t);
        }

        float EaseInOutExpo(float t)
		{
            if (t < 0.5f) return (powf(2, 16 * t) - 1) / 510;

            return 1 - 0.5f * powf(2, -16 * (t - 0.5f));
        }

        float EaseInCirc(float t)
		{
            return 1 - sqrtf(1 - t);
        }

        float EaseOutCirc(float t)
		{
            return sqrtf(t);
        }

        float EaseInOutCirc(float t)
		{
            if (t < 0.5f) return (1 - sqrtf(1 - 2 * t)) * 0.5f;

            return (1 + sqrtf(2 * t - 1)) * 0.5f;
        }

        float EaseInBack(float t)
		{
            return t * t * (2.70158f * t - 1.70158f);
        }

        float EaseOutBack(float t)
		{
            return 1 + (--t) * t * (2.70158f * t + 1.70158f);
        }

        float EaseInOutBack(float t)
		{
            if (t < 0.5f) return t * t * (7 * t - 2.5f) * 2;

            return 1 + (--t) * t * 2 * (7 * t + 2.5f);
        }

        float EaseInElastic(float t)
		{
            float t2 = t * t;
            return t2 * t2 * sinf(t * PI * 4.5f);
        }

        float EaseOutElastic(float t)
		{
            float t2 = (t - 1) * (t - 1);
            return 1 - t2 * t2 * cosf(t * PI * 4.5f);
        }

        float EaseInOutElastic(float t)
		{
            float t2;
            if (t < 0.45f)
            {
                t2 = t * t;
                return 8 * t2 * t2 * sinf(t * PI * 9);
            }

            if (t < 0.55f) return 0.5f + 0.75f * sinf(t * PI * 4);

            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sinf(t * PI * 9);
        }

        float EaseInBounce(float t)
		{
            return powf(2, 6 * (t - 1)) * abs(sinf(t * PI * 3.5f));
        }

        float EaseOutBounce(float t)
		{
            return 1 - powf(2, -6 * t) * abs(cosf(t * PI * 3.5f));
        }

        float EaseInOutBounce(float t)
		{
            if (t < 0.5f) return 8 * powf(2, 8 * (t - 1)) * abs(sinf(t * PI * 7));

        	return 1 - 8 * powf(2, -8 * t) * abs(sinf(t * PI * 7));
        }
	}
}
