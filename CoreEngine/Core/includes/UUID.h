#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{
	class UUID
	{
	public:

		UUID() = default;
		UUID(const uint64& id) : CurrentID(id)
		{
		}
		UUID(const UUID& other) = default;
		UUID(UUID&& other) = default;
		UUID& operator=(const UUID& other) = default;
		UUID& operator=(UUID&& other) = default;

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