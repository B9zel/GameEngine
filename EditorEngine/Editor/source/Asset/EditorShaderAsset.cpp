#include <Editor/includes/Asset/EditorShaderAsset.h>
#include <imgui.h>

EditorShaderAsset::EditorShaderAsset(const CoreEngine::InitializeObject& Initialize) : ShaderAsset(Initialize)
{
}

void EditorShaderAsset::DrawElements()
{
	auto Callback = [](ImGuiInputTextCallbackData* data) -> int
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			auto* Str = static_cast<String*>(data->UserData);
			Str->resize(data->BufTextLen);
			data->Buf = Str->data();
		}
		return 0;
	};

	ImGui::BeginGroup();

	const bool IsEqual = ChangedCustomShader == CustomShader;
	// if (IsEqual)
	{
		ImGui::BeginDisabled(IsEqual);
	}
	if (ImGui::Button("Apply", ImVec2(ImGui::GetWindowSize().x, 0)))
	{
		ApplyChange();
	}
	// if (IsEqual)
	{
		ImGui::EndDisabled();
	}

	ImGui::Text("Vertex shader");
	ImGui::InputTextMultiline("##VertexShader", ChangedCustomShader.VertexShader.data(), ChangedCustomShader.VertexShader.capacity() + 1,
							  ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiInputTextFlags_CallbackResize, Callback, &ChangedCustomShader.VertexShader);
	ImGui::Text("Fragment shader");
	ImGui::InputTextMultiline("##FragmentShader", ChangedCustomShader.FragmentShader.data(), ChangedCustomShader.FragmentShader.capacity() + 1,
							  ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiInputTextFlags_CallbackResize, Callback,
							  &ChangedCustomShader.FragmentShader);

	ImGui::EndGroup();
}

void EditorShaderAsset::OnDeserialize(CoreEngine::SerializeAchive& Achive)
{
	ShaderAsset::OnDeserialize(Achive);

	ChangedCustomShader = CustomShader;
}

void EditorShaderAsset::ApplyChange()
{
	SetCustomShader(ChangedCustomShader);
}
