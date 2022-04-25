#pragma once

namespace maoutch
{
	class IDamageable
	{
		virtual void Damage(const float& damage ) = 0;
	};
}