#pragma once
#include <Editor/includes/BaseEditorPanel.h>

namespace CoreEngine::Runtime
{
	class SceneComponent;
}

namespace Editor
{
	class SceneHierarhy : public BaseEdtiorPanel
	{
	public:

		SceneHierarhy() = default;

		virtual void Draw() override;

	private:

		void DrawAndWalkComponent(CoreEngine::Runtime::SceneComponent* Component);
	};
}