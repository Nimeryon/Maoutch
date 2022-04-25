#pragma once
namespace maoutch
{
	class IStateDependant
	{
	public:
		IStateDependant();
		virtual ~IStateDependant();

	protected:
		virtual void _OnStateChange() = 0;
	};
}
