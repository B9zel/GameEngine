#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{
	class UUID
	{
	public:

		void GenerateID();

		uint64 GetID() const;
		void Release();

		bool IsValid() const;

	private:

		static uint64 Counter;
		static Stack<uint64> FreeID;
		
		uint64 CurrentID = 0;
	};
}