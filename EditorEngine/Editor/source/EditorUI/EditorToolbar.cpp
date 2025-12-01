#include <Editor/includes/EditorUI/EditorToolbar.h>
#include <Editor/includes/EditorEngine.h>
#include <imgui.h>


namespace Editor
{
	EditorToolbar::EditorToolbar()
	{
		PlayTexture = CoreEngine::Render::Texture2D::Create("../../Resources/Play.png");
		StopTexture = CoreEngine::Render::Texture2D::Create("../../Resources/Stop.png");
		
	}

	void EditorToolbar::Draw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float Size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (Size * 0.5));
		
		uint32 Id = OwnerEditor->GetCurrentStateWorld() == EStateWorld::Edit ? PlayTexture->GetTextureID() : StopTexture->GetTextureID();
		if (ImGui::ImageButton("Stop", Id, ImVec2(Size, Size - 5)))
		{
			if (OwnerEditor->GetCurrentStateWorld() == EStateWorld::Edit)
			{
				OwnerEditor->SetCurrentStateWorld(EStateWorld::Play);

			}
			else if (OwnerEditor->GetCurrentStateWorld() == EStateWorld::Play)
			{
				OwnerEditor->SetCurrentStateWorld(EStateWorld::Edit);

			}
		}
		//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (Size * 0.5));
		/*ImGui::SameLine();
		if (ImGui::ImageButton("Play", PlayTexture->GetTextureID(), ImVec2(Size, Size - 5)))
		{
			OwnerEditor->SetCurrentStateWorld(EStateWorld::Play);
		}*/
	

		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}
}