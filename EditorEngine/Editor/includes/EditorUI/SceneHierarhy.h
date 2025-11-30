#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>

namespace CoreEngine::Runtime
{
	class SceneComponent;
	class Actor;
}

namespace Editor
{
	class SceneHierarhy : public BaseEditorPanel
	{
	public:

		SceneHierarhy() = default;

		virtual void Draw() override;

	private:

		void PushColorTree();

	private:

		void DrawAndWalkComponents(const DArray<CoreEngine::Runtime::SceneComponent*>& Components);
		bool IsChildComponent(CoreEngine::Runtime::Actor* Actor);
	};
}