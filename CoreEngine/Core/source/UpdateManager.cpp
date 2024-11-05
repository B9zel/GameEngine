#include <Core/includes/UpdateManager.h>
#include <Runtime/CoreObject/Include/Object.h>



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

	void UpdateManager::AddFunction(const UpdateFunction& newFunc)
	{
		m_GroupUpdate.find(newFunc.GetStage())->second.push_back(newFunc);
	}

	void UpdateManager::ExecuteGroup(float deltaTime, const EStageUpdate stage)
	{
		if (!m_GroupUpdate.count(stage)) return;

		for (auto& func : m_GroupUpdate[stage])
		{
			func.ExecuteUpdate(deltaTime);
		}
	}
}