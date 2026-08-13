#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>

class SceneComponent;
class Actor;

namespace Editor
{
	class SceneHierarhy : public BaseEditorPanel
	{
	public:

		SceneHierarhy() = default;

		virtual void Draw() override;
		virtual void OnConstruct() override;

	private:

		void DrawAndWalkComponents(const DArray<SceneComponent*>& Components);
		void DragDropTarget(SceneComponent* sceneComponent);
		bool IsChildComponent(Actor* Actor);

	private:

		SceneComponent* ComponentDrag{nullptr};
		bool HasTargetValid{false};
	};
} // namespace Editor
