#include <Core/includes/UpdateManager.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	UpdateManager* UpdateManager::m_Instance = nullptr;

	UniquePtr<UpdateManager> UpdateManager::CreateInstance()
	{
		if (m_Instance)
		{
			EG_LOG(CORE, ELevelLog::ERROR, "Update manager already exists");
			return UniquePtr<UpdateManager>(m_Instance);
		}
		
		m_Instance = new UpdateManager();

		return UniquePtr<UpdateManager>(m_Instance);;
	}

	void UpdateManager::AddFunction(UpdateFunction* newFunc)
	{
		if (!m_GroupUpdate.count(newFunc->GetStage()))
		{
			//Pair<EStageUpdate, DArray<UpdateFunction*>(newFunc, DArray<UpdateFunction*>())
			m_GroupUpdate.insert(Pair<EStageUpdate, DArray<UpdateFunction*>>(newFunc->GetStage(), DArray<UpdateFunction*>())); 
		}
		m_GroupUpdate.find(newFunc->GetStage())->second.push_back(newFunc);
	}

	void UpdateManager::ExecuteGroup(float deltaTime, const EStageUpdate stage)
	{
		if (!m_GroupUpdate.count(stage)) return;

		for (UpdateFunction* func : m_GroupUpdate.find(stage)->second)
		{
			func->ExecuteUpdate(deltaTime);
		}
	}
}