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
				ImGuiTreeNodeFlags Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_Framed;
				bool IsOpen = ImGui::TreeNodeEx(Actor->GetName().c_str(), Flag);
				if (ImGui::IsItemClicked())
				{
					OwnerEditor->SetSelectedObject(Actor);
				}
				if (IsOpen)
				{
					DrawAndWalkComponent(Actor->GetRootComponent());
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

	void SceneHierarhy::DrawAndWalkComponent(CoreEngine::Runtime::SceneComponent* Component)
	{
		if (!Component) return;

		ImGuiTreeNodeFlags Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
		if (Component->GetChildrenAttaches().empty())
		{
			Flag |= ImGuiTreeNodeFlags_Leaf;
		}

		bool IsOpen = ImGui::TreeNodeEx((Component->GetName()).c_str(), Flag);
		if (ImGui::IsItemClicked())
		{
			OwnerEditor->SetSelectedObject(Component);
		}
		if (IsOpen)
		{
			for (auto* SceneComponent : Component->GetChildrenAttaches())
			{
				DrawAndWalkComponent(SceneComponent);
			}
			ImGui::TreePop();
		}
		
	}
}