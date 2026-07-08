#include <Editor/includes/Asset/EditorShaderAsset.h>
#include <imgui.h>

EditorShaderAsset::EditorShaderAsset(const CoreEngine::InitializeObject& Initialize) : ShaderAsset(Initialize)
{
}

void EditorShaderAsset::DrawElements()
{
	static String VertexShader;
	static String FragmentShader;
	VertexShader = ChangedCustomShader.VertexShader;
	FragmentShader = ChangedCustomShader.FragmentShader;

	VertexShader.reserve(ChangedCustomShader.VertexShader.capacity());
	FragmentShader.reserve(ChangedCustomShader.FragmentShader.capacity());

	uint32 CapacityVertex = VertexShader.capacity() == 0 ? 1 : VertexShader.capacity();
	uint32 CapacityFragment = FragmentShader.capacity() == 0 ? 1 : FragmentShader.capacity();

	if (((VertexShader.size() + 1) / static_cast<float>(CapacityVertex)) >= m_PercenBordertCapacity)
	{
		VertexShader.resize(VertexShader.capacity() * (1.0f + (1.0f - m_PercenBordertCapacity)));
	}

	if (((FragmentShader.size() + 1) / static_cast<float>(CapacityFragment)) >= m_PercenBordertCapacity)
	{
		float a = (1.0f + (1.0f - m_PercenBordertCapacity));
		FragmentShader.reserve(FragmentShader.capacity() * a);
	}

	auto Callback = [](ImGuiInputTextCallbackData* data) -> int
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			const String Buf = data->Buf;
			auto* Str = (String*)data->UserData;
			Str->resize(data->BufSize);
			*Str = Buf;
			data->Buf = (char*)Str->c_str();
		}
		return 1;
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
	ImGui::InputTextMultiline("##VertexShader", ChangedCustomShader.VertexShader.data(), VertexShader.capacity(), ImVec2(ImGui::GetContentRegionAvail().x, 0),
							  ImGuiInputTextFlags_CallbackResize, Callback, &ChangedCustomShader.VertexShader);
	ImGui::Text("Fragment shader");
	ImGui::InputTextMultiline("##FragmentShader", ChangedCustomShader.FragmentShader.data(), ChangedCustomShader.FragmentShader.capacity(),
							  ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackEdit, Callback,
							  &ChangedCustomShader.FragmentShader);

	ImGui::EndGroup();

	// CustomShader.FragmentShader = FragmentShader;
	// CustomShader.VertexShader = VertexShader;
	/*	CustomShader.VertexShader = VertexShader.data();
	CustomShader.FragmentShader = FragmentShader.data();

	CustomShader.VertexShader.reserve(VertexShader.capacity());
	CustomShader.FragmentShader.reserve(FragmentShader.capacity());*/
}

void EditorShaderAsset::OnDeserialize(CoreEngine::SerializeAchive& Achive)
{
	ShaderAsset::OnDeserialize(Achive);

	ChangedCustomShader = CustomShader;
}

void EditorShaderAsset::ApplyChange()
{
	CustomShader = ChangedCustomShader;
}
