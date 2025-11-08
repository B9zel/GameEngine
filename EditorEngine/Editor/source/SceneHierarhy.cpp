#include <Editor/includes/SceneHierarhy.h>
#include <imgui/imgui.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/Level.h>



namespace Editor
{
	void SceneHierarhy::DrawSceneHierarchy()
	{
		ImGui::Begin("Scene hierarchy");
		
		for (auto* Level : CoreEngine::Engine::Get()->GetWorld()->GetLevels())
		{
			for (auto* Actor : Level->GetActors())
			{
				ImGui::Button(Actor->GetClass()->Name.c_str());
			}
		}

		//ImGui::TreeNode

		ImGui::End();
	}
}