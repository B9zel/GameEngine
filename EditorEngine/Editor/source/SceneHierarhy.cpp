#include <Editor/includes/SceneHierarhy.h>
#include <imgui/imgui.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/Level.h>
#include <Editor/includes/EditorEngine.h>
#include <Runtime/includes/SceneComponent.h>


namespace Editor
{

	void SceneHierarhy::Draw()
	{
		ImGui::Begin("Scene hierarchy");

		for (auto* Level : CoreEngine::Engine::Get()->GetWorld()->GetLevels())
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
					/*for (auto* Component : Actor->GetComponents())
					{


						if (Component->GetClass()->IsChildClassOf(CoreEngine::Runtime::SceneComponent::GetStaticClass()))
						{
							if (ImGui::TreeNode(Component->GetName().c_str()))
							{


								ImGui::TreePop();
							}
						}
					}*/
					ImGui::TreePop();
				}
				
				/*if (ImGui::Selectable(Actor->GetClass()->Name.c_str(), Actor == OwnerEditor->GetSelectedObject()))
				{
					OwnerEditor->SetSelectedObject(Actor);
				}*/
			}
		}


		ImGui::End();
	}


	void SceneHierarhy::DrawAndWalkComponents(const DArray<CoreEngine::Runtime::SceneComponent*>& Components)
	{
		if (Components.empty()) return;

		for (auto* SceneComponent : Components)
		{
			ImGuiTreeNodeFlags Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (OwnerEditor->GetSelectedObject() == SceneComponent)
			{
				PushColorTree();
				Flag |= (OwnerEditor->GetSelectedObject() == SceneComponent ? ImGuiTreeNodeFlags_Selected : 0);
			}
			if (SceneComponent->GetChildrenAttaches().empty())
			{
				Flag |= ImGuiTreeNodeFlags_Leaf;
			}

			bool IsOpen = ImGui::TreeNodeEx((SceneComponent->GetName()).c_str(), Flag);

			if (OwnerEditor->GetSelectedObject() == SceneComponent)
			{
				ImGui::PopStyleColor(3);
			}

			if (ImGui::IsItemClicked())
			{
				OwnerEditor->SetSelectedObject(SceneComponent);
			}
			if (IsOpen)
			{
				/*for (auto* SceneComponent : SceneComponent->GetChildrenAttaches())
				{
					DrawAndWalkComponents(SceneComponent);
				}*/
				DrawAndWalkComponents(SceneComponent->GetChildrenAttaches());
				ImGui::TreePop();
			}
		}
		
		
	}
	bool SceneHierarhy::IsChildComponent(CoreEngine::Runtime::Actor* Actor)
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

	void SceneHierarhy::PushColorTree()
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.22f, 0.45f, 0.85f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.32f, 0.55f, 0.95f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.22f, 0.45f, 0.85f, 1.f));
	}

}