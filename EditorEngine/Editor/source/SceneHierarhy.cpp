#include <Editor/includes/SceneHierarhy.h>
#include <imgui/imgui.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/Level.h>
#include <Editor/includes/EditorEngine.h>



namespace Editor
{

	void SceneHierarhy::Draw()
	{
		ImGui::Begin("Scene hierarchy");

		for (auto* Level : CoreEngine::Engine::Get()->GetWorld()->GetLevels())
		{
			for (auto* Actor : Level->GetActors())
			{
				if (ImGui::Selectable(Actor->GetClass()->Name.c_str(), Actor == OwnerEditor->GetSelectedObject()))
				{
					OwnerEditor->SetSelectedObject(Actor);
				}
			}
		}


		ImGui::End();
	}
}