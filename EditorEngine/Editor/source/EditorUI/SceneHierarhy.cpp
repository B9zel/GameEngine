#include <Editor/includes/EditorUI/SceneHierarhy.h>
#include <imgui/imgui.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/Level.h>
#include <Editor/includes/EditorEngine.h>
#include <Runtime/includes/SceneComponent.h>
#include <Editor/includes/Utills/DrawUtills.h>

namespace Editor
{

	void SceneHierarhy::Draw()
	{
		ImGui::Begin("Scene hierarchy");

		for (auto* Level : Engine::Get()->GetWorld()->GetLevels())
		{
			for (auto* Actor : Level->GetActors())
			{
				ImGuiTreeNodeFlags Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
				const bool IsSelect = Actor == OwnerEditor->GetSelectedObject();
				if (IsSelect || IsChildComponent(Actor))
				{
					Flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected;
					if (IsSelect)
					{
						PushColorTree();
					}
				}
				bool IsOpen = ImGui::TreeNodeEx(Actor->GetName().c_str(), Flag);

				DragDropTarget(Actor->GetRootComponent());

				if (IsSelect)
				{
					ImGui::PopStyleColor(3);
				}
				if (ImGui::IsItemClicked())
				{
					OwnerEditor->SetSelectedObject(Actor);
				}
				if (IsOpen)
				{
					DrawAndWalkComponents(Actor->GetRootComponent()->GetChildrenAttaches());

					ImGui::TreePop();
				}

				Object* SelectedObject = OwnerEditor->GetSelectedObject();

				DrawComponentContextDraw(OwnerEditor, SelectedObject);
			}
		}

		ImGui::End();
	}

	void SceneHierarhy::OnConstruct()
	{
	}

	void SceneHierarhy::DrawAndWalkComponents(const DArray<SceneComponent*>& Components)
	{
		if (Components.empty()) return;

		for (auto* sceneComponent : Components)
		{
			ImGuiTreeNodeFlags Flag =
				ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (OwnerEditor->GetSelectedObject() == sceneComponent)
			{
				PushColorTree();
				Flag |= (OwnerEditor->GetSelectedObject() == sceneComponent ? ImGuiTreeNodeFlags_Selected : 0);
			}
			if (sceneComponent->GetChildrenAttaches().empty())
			{
				Flag |= ImGuiTreeNodeFlags_Leaf;
			}

			bool IsOpen = ImGui::TreeNodeEx((sceneComponent->GetName()).c_str(), Flag);

			if (ImGui::BeginDragDropSource())
			{
				ComponentDrag = sceneComponent;
				ImGui::SetDragDropPayload("SCENE_COMPONENT", &sceneComponent, sizeof(sceneComponent));
				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Dragging SceneComponent: {}", (uint64)sceneComponent);

				ImGui::Text("%s", sceneComponent->GetName().c_str());

				ImGui::EndDragDropSource();
			}

			DragDropTarget(sceneComponent);

			if (OwnerEditor->GetSelectedObject() == sceneComponent)
			{
				ImGui::PopStyleColor(3);
			}

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				OwnerEditor->SetSelectedObject(sceneComponent);
			}
			if (IsOpen)
			{
				DrawAndWalkComponents(sceneComponent->GetChildrenAttaches());
				ImGui::TreePop();
			}
		}
	}

	void SceneHierarhy::DragDropTarget(SceneComponent* sceneComponent)
	{
		HasTargetValid = false;
		if (!ComponentDrag || !sceneComponent) return;

		const ImGuiPayload* dragPayload = ImGui::GetDragDropPayload();
		if (!dragPayload || !dragPayload->IsDataType("SCENE_COMPONENT")) return;

		const bool HasSameOwner = ComponentDrag->GetOwner() == sceneComponent->GetOwner();

		if (ImGui::BeginDragDropTarget())
		{
			if (!HasSameOwner)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.25f, 0.25f, 1.0f));
				ImGui::SetTooltip("Cannot attach a component to %s", sceneComponent->GetName().c_str());
				ImGui::PopStyleColor();
			}
			else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_COMPONENT"))
			{
				SceneComponent* dropped = *static_cast<SceneComponent**>(payload->Data);

				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Dragging SceneComponent end: {}", (uint64)payload->Data);
				if (dropped != sceneComponent && sceneComponent->GetOwner() == dropped->GetOwner())
				{
					dropped->SetupToAttachment(sceneComponent);
				}
			}

			ImGui::EndDragDropTarget();
		}
		HasTargetValid = HasSameOwner;
	}

	bool SceneHierarhy::IsChildComponent(Actor* Actor)
	{
		for (auto* Component : Actor->GetComponents())
		{
			if (OwnerEditor->GetSelectedObject() == Component)
			{
				return true;
			}
		}
		return false;
	}

} // namespace Editor
