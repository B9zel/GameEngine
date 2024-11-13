#pragma once
#include <Core/includes/UpdateFunction.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	class UpdateFunction;

	enum class EStageUpdate : uint8
	{
		PRE_UPDATE = 0,
		UPDATE,
		POST_UPDATE,

		MAX_STAGE
	};

	class UpdateManager
	{
	protected:

		UpdateManager() = default;

	public:

		static UniquePtr<UpdateManager> CreateInstance();

		void AddFunction(UpdateFunction* newFunc);
		void ExecuteGroup(float deltaTime, const EStageUpdate stage);

	protected:

		static UpdateManager* m_Instance;

	private:

		HashTableMap<EStageUpdate, DArray<UpdateFunction*>> m_GroupUpdate;
	};
}