#include <Editor/includes/Asset/EditorShaderAsset.h>
#include <Editor/includes/EditorEngine.h>
#include <imgui.h>

EditorShaderAsset::EditorShaderAsset(const CoreEngine::InitializeObject& Initialize) : ShaderAsset(Initialize)
{
	for (TextEditor* Editor : {&VertexEditor, &FragmentEditor})
	{
		Editor->SetLanguage(TextEditor::Language::Glsl());
		Editor->SetPalette(TextEditor::GetDefaultPalette());
		Editor->SetTabSize(4);
		Editor->SetInsertSpacesOnTabs(true);
		Editor->SetAutoIndentEnabled(true);
		Editor->SetShowLineNumbersEnabled(true);
		Editor->SetShowMatchingBrackets(true);
		Editor->SetCompletePairedGlyphs(true);
		Editor->SetLineSpacing(1.15f);
	}

	VertexEditor.SetChangeCallback([this]() { ChangedCustomShader.VertexShader = VertexEditor.GetText(); });
	FragmentEditor.SetChangeCallback([this]() { ChangedCustomShader.FragmentShader = FragmentEditor.GetText(); });
}

void EditorShaderAsset::DrawElements()
{
	if (ImGui::Button("Open Shader Editor", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
	{
		if (auto* Editor = dynamic_cast<EditorEngine*>(Engine::Get()))
		{
			Editor->OpenShaderEditor(this);
		}
	}
	ImGui::TextDisabled("Vertex / Fragment - GLSL");
}

void EditorShaderAsset::InitializeEditors()
{
	ChangedCustomShader = CustomShader;
	VertexEditor.SetText(ChangedCustomShader.VertexShader);
	FragmentEditor.SetText(ChangedCustomShader.FragmentShader);
	m_EditorsInitialized = true;
}

void EditorShaderAsset::OpenEditor()
{
	if (!m_EditorsInitialized)
	{
		InitializeEditors();
	}
	m_IsEditorOpen = true;
	m_FocusEditor = true;
}

bool EditorShaderAsset::DrawEditor()
{
	if (!m_IsEditorOpen) return false;

	ImGui::SetNextWindowSize(ImVec2(820, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(440, 300), ImVec2(FLT_MAX, FLT_MAX));
	if (m_FocusEditor)
	{
		ImGui::SetNextWindowFocus();
		m_FocusEditor = false;
	}

	const bool HasChanges = ChangedCustomShader != CustomShader;
	const String Title = GetName() + " - Shader Editor###ShaderEditor_";
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
	const bool IsVisible = ImGui::Begin(Title.c_str(), &m_IsEditorOpen, HasChanges ? ImGuiWindowFlags_UnsavedDocument : 0);
	if (IsVisible)
	{
		ImGui::BeginDisabled(!HasChanges);
		if (ImGui::Button("Apply")) ApplyChange();
		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::TextDisabled(ChangedCustomShader == CustomShader ? "Applied" : "Unapplied changes");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(120);
		ImGui::SliderFloat("Font size", &m_CodeFontSize, 13.0f, 22.0f, "%.0f px");
		ImGui::Separator();

		if (ImGui::BeginTabBar("ShaderStages"))
		{
			auto DrawStage = [this](const char* Name, TextEditor& Editor, bool IsChanged)
			{
				if (ImGui::BeginTabItem(Name, nullptr, IsChanged ? ImGuiTabItemFlags_UnsavedDocument : 0))
				{
					if (ImGui::SmallButton("Find / Replace")) Editor.OpenFindReplaceWindow();
					ImGui::SameLine();
					ImGui::TextDisabled("Ctrl+F  |  Tab / Shift+Tab");

					const float FooterHeight = ImGui::GetFrameHeightWithSpacing();
					const float EditorHeight = std::max(80.0f, ImGui::GetContentRegionAvail().y - FooterHeight);
					ImGui::PushFont(nullptr, m_CodeFontSize);
					Editor.Render("Code", ImVec2(-1, EditorHeight), ImGuiChildFlags_Borders);
					ImGui::PopFont();

					const auto Cursor = Editor.GetMainCursorPosition();
					ImGui::TextDisabled("GLSL  |  Ln %d, Col %d  |  Spaces: 4  |  UTF-8", static_cast<uint32>(Cursor.line + 1),
										static_cast<int>(Cursor.index + 1));
					ImGui::EndTabItem();
				}
			};
			DrawStage("Vertex", VertexEditor, ChangedCustomShader.VertexShader != CustomShader.VertexShader);
			DrawStage("Fragment", FragmentEditor, ChangedCustomShader.FragmentShader != CustomShader.FragmentShader);
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();

	return m_IsEditorOpen;
}

void EditorShaderAsset::OnDeserialize(CoreEngine::SerializeAchive& Achive)
{
	ShaderAsset::OnDeserialize(Achive);

	InitializeEditors();
}

void EditorShaderAsset::ApplyChange()
{
	ChangedCustomShader.VertexShader = VertexEditor.GetText();
	ChangedCustomShader.FragmentShader = FragmentEditor.GetText();
	SetCustomShader(ChangedCustomShader);
}
