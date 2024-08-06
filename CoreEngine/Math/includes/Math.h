#pragma once

struct Math
{
	template<class T1, class T2=T1, class T3=T1 >
	static inline T1 Clamp(T1 current, T2 min, T3 max)
	{
		return current < min ? min : current > max ? max : current;
	}
};
