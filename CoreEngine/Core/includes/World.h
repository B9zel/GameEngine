#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>



namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
	}
	namespace Render
	{
		class SceneInterface;
	}
}

namespace CoreEngine
{
	class TimerManager;
	class UpdateManager;
	class Level;

	class World : public CoreEngine::Runtime::Object
	{
	public:

		World();

	public:

		virtual void InitProperties() override {}

		virtual void WorldUpdate();
		UpdateManager* GetUpdateManager();

		float GetWorldDeltaTime() const;
		const DArray<Level*>& GetLevels() const;

	private:

		UniquePtr<UpdateManager> m_UpdateManager;

		DArray<Level*> m_Levels;
		Render::SceneInterface* m_Scene;


		float m_DeltaTime;
		float m_LastTime;
	};
}