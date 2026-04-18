#include <Editor/includes/EditorUI/ContentBrowser.h>
#include <Core/includes/Application.h>
#include <Editor/includes/Util/DrawUtils.h>
#include <Render/includes/Texture.h>
#include <Render/includes/RenderDevice.h>
#include <Core/includes/AssetManager.h>
#include <Core/includes/Engine.h>
#include <imgui.h>

namespace Editor
{
	static std::filesystem::path ContentBrowserPath;

	void ContentBrowser::OnConstruct()
	{
		const auto& Options = CoreEngine::Application::Get()->GetAppOptions();
		ContentBrowserPath = Options.pathToProject;

		ContentBrowserPath /= "Sandbox";
		ContentBrowserPath /= "Content";

		SetCurrentPath(ContentBrowserPath);

		Folder = AssetManager::Get().LoadTexture(Options.pathToProject + "/Resources/folder.png");
		File = AssetManager::Get().LoadTexture(Options.pathToProject + "/Resources/file.png");
	}

	void ContentBrowser::Draw()
	{
		ImGui::Begin("Content Browser");

		DrawFilderTree();

		ImGui::SameLine();
		DrawMainContent();

		ImGui::End();
	}

	void ContentBrowser::DrawMainContent()
	{
		ImGui::BeginChild("#Files");
		if (m_CurrentPath != ContentBrowserPath)
		{
			if (ImGui::ArrowButton("<-", ImGuiDir_Left))
			{
				SetCurrentPath(m_CurrentPath.parent_path());
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Update"))
		{
			CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);
		}

		DrawFoldersAndFiles();
		ImGui::EndChild();
	}

	void ContentBrowser::DrawFoldersAndFiles()
	{
		bool IsChangePath = false;
		const auto& RenderDevice = Engine::Get()->GetRenderDevice();
		const ImVec4 BackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

		for (const auto& entry : m_StorageDirectories)
		{
			ImGui::BeginChild((entry.path().filename().string() + "Group").c_str(), ImVec2(0, 0),
							  ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (entry.is_directory())
			{
				if (ImGui::ImageButton(entry.path().filename().string().c_str(), RenderDevice->GetTextureID(Folder->GetConstTextureHandle()), ImVec2(100, 100),
									   ImVec2(0, 0), ImVec2(1, -1), BackgroundColor, ImVec4(1, 1, 1, 1)))
				{
					IsChangePath = true;
					SetCurrentPath(m_CurrentPath / entry.path());
				}
				ImGui::Text(entry.path().filename().string().c_str());
			}
			else if (entry.is_regular_file())
			{
				if (ImGui::ImageButton(entry.path().filename().string().c_str(), RenderDevice->GetTextureID(File->GetConstTextureHandle()), ImVec2(100, 100),
									   ImVec2(0, 0), ImVec2(1, -1), BackgroundColor, ImVec4(1, 1, 1, 1)))
				{
				}
				ImGui::Text(entry.path().stem().string().c_str());
			}
			ImGui::PopStyleColor();
			ImGui::EndChild();
			ImGui::SameLine();

			if (IsChangePath)
			{
				break;
			}
		}
	}

	void ContentBrowser::DrawFilderTree()
	{
		ImGui::BeginChild("#Directory", ImVec2(250, 0), ImGuiChildFlags_ResizeX);

		ImDrawList* DrawList = ImGui::GetWindowDrawList();
		ImVec2 Min = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y);
		ImVec2 Max = ImVec2(Min.x, Min.y + ImGui::GetWindowSize().y);

		DrawList->AddLine(Min, Max, ImGui::GetColorU32(ImGuiCol_Border), 5);

		std::filesystem::path StartPath = ContentBrowserPath;
		if (ImGui::TreeNodeEx(StartPath.filename().string().c_str(),
							  ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::IsItemClicked())
			{
				SetCurrentPath(StartPath);
			}
			DrawLeftDirectoriesPanel(StartPath);
			ImGui::TreePop();
		}
		ImGui::EndChild();
	}
	void ContentBrowser::DrawLeftDirectoriesPanel(const std::filesystem::path& Path)
	{
		DArray<std::filesystem::directory_entry> StorageDirectories;
		// StorageDirectories.clear();

		for (const auto& entry : std::filesystem::directory_iterator(Path))
		{
			if (entry.is_directory())
			{
				StorageDirectories.push_back(entry);
			}
		}

		ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;

		for (auto& Director : StorageDirectories)
		{
			ImGuiTreeNodeFlags CurrFlags = Flags;
			// Dynamic parameters
			const bool IsSelectedDirect = Director.path() == m_CurrentPath;
			if (IsSelectedDirect)
			{
				PushColorTree();
				CurrFlags |= ImGuiTreeNodeFlags_Selected;
			}

			if (!HasAnySubdirectory(Director))
			{
				CurrFlags |= ImGuiTreeNodeFlags_Leaf;
			}
			//

			const bool IsOpen = ImGui::TreeNodeEx(Director.path().filename().string().c_str(), CurrFlags);

			if (IsSelectedDirect)
			{
				ImGui::PopStyleColor(3);
			}

			if (ImGui::IsItemClicked())
			{
				SetCurrentPath(Director);
			}
			if (IsOpen)
			{
				DrawLeftDirectoriesPanel(Director.path());

				ImGui::TreePop();
			}
		}
	}
	bool ContentBrowser::HasAnySubdirectory(const std::filesystem::path& Path)
	{
		if (!std::filesystem::exists(Path) || !std::filesystem::is_directory(Path)) return false;

		for (const auto& entry : std::filesystem::directory_iterator(Path))
		{
			if (entry.is_directory())
			{
				return true;
			}
		}

		return false;
	}
	void ContentBrowser::CollectAllFilesAndDirectories(const std::filesystem::path& Path, DArray<std::filesystem::directory_entry>& StorageDirectories)
	{
		StorageDirectories.clear();
		for (const auto& entry : std::filesystem::directory_iterator(Path))
		{
			if (entry.is_directory() || entry.is_regular_file())
			{
				StorageDirectories.push_back(entry);
			}
		}
	}
	void ContentBrowser::SetCurrentPath(const std::filesystem::path& NewPath)
	{
		if (std::filesystem::exists(NewPath))
		{
			m_CurrentPath = NewPath;
		}
		CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);
	}
} // namespace Editor
