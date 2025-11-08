#include <Core/includes/UUID.h>


namespace CoreEngine
{
	Stack<uint64> UUID::FreeID;
	uint64 UUID::Counter = 1;

	void UUID::GenerateID()
	{
		if (IsValid())
		{
			Release();
		}

		if (!FreeID.empty())
		{
			CurrentID = FreeID.top();
			FreeID.pop();
			return;
		}
		CurrentID = Counter++;
	}

	uint64 UUID::GetID() const
	{
		return CurrentID;
	}

	bool UUID::IsValid() const
	{
		return CurrentID != 0;
	}

	void UUID::Release()
	{
		if (IsValid())
		{
			FreeID.push(CurrentID);
			CurrentID = 0;
		}
	}
}