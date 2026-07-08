#include <Editor/includes/EditorUI/ContentBrowser.h>
#include <Core/includes/Application.h>
#include <Editor/includes/Util/DrawUtils.h>
#include <Render/includes/Texture.h>
#include <Render/includes/RenderDevice.h>
#include <Core/includes/AssetManager.h>
#include <Core/includes/Engine.h>
#include <Core/includes/Asset.h>
#include <Core/includes/StringUtil.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorDrawingInterface.h>
#include <Core/includes/AssetManager.h>
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
		ImVec2 Size;
		Size.x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x;
		Size.y = ImGui::GetWindowSize().y;

		ImGui::BeginChild("#Files", Size);
		DrawCreateContextMenu();

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
		const float RegionWidth = ImGui::GetWindowSize().x;

		// ImGui::BeginChild("#ConteinFiles", ImVec2(), ImGuiChildFlags_AutoResizeX);

		bool IsChangePath = false;
		const auto& RenderDevice = Engine::Get()->GetRenderDevice();
		const ImVec4 BackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

		ImVec2 ButtonSize = ImVec2(100, 100);
		const float WidthOffset = 10;

		for (uint64 i = 0; i < m_StorageDirectories.size(); ++i)
		{
			auto entry = m_StorageDirectories[i];

			const String& FilenameString = entry.path().filename().string();
			ImGui::BeginChild((FilenameString + "Group").c_str(), ImVec2(0, ButtonSize.y + ImGui::GetFontSize() + ImGui::GetTextLineHeight() + WidthOffset),
							  ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (entry.is_directory())
			{
				if (ImGui::ImageButton(FilenameString.c_str(), RenderDevice->GetTextureID(Folder->GetConstTextureHandle()), ButtonSize, ImVec2(0, 0),
									   ImVec2(1, -1), BackgroundColor, ImVec4(1, 1, 1, 1)))
				{
					IsChangePath = true;
					SetCurrentPath(m_CurrentPath / entry.path());
				}
				if (m_IsRenaming && m_RenameItems.RenamingID == i)
				{
					if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
					{
						ImGui::SetKeyboardFocusHere(10);
						ImGui::SetWindowFocus();
					}
					if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
					{
						/*ImGui::SetNextWindowFocus();
						m_RenameItems.RenamingID = -1;
						m_IsRenaming = false;*/
					}

					{

						ImGui::SetNextItemWidth(ButtonSize.x);

						const String& SourceName = m_StorageDirectories[m_RenameItems.RenamingID].path().filename().string();
						std::copy(SourceName.begin(), SourceName.end(), m_RenameItems.RenameBuffer);

						if (ImGui::InputText("##rename", m_RenameItems.RenameBuffer, sizeof(m_RenameItems.RenameBuffer),
											 ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
						{
							std::filesystem::rename(m_StorageDirectories[i].path(), m_StorageDirectories[i].path().parent_path() / m_RenameItems.RenameBuffer);
							m_RenameItems.RenamingID = -1;
							std::fill(m_RenameItems.RenameBuffer, m_RenameItems.RenameBuffer + m_RenameItems.BufferSize, '\0');
							CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);

							m_IsRenaming = false;
						}
					}
				}
				else
				{
					ImGui::Text(FilenameString.c_str());
				}
			}
			else if (entry.is_regular_file())
			{
				if (ImGui::ImageButton(FilenameString.c_str(), RenderDevice->GetTextureID(File->GetConstTextureHandle()), ButtonSize, ImVec2(0, 0),
									   ImVec2(1, -1), BackgroundColor, ImVec4(1, 1, 1, 1)))
				{
					OpenAsset(entry.path().string());
				}

				if (m_IsRenaming && m_RenameItems.RenamingID == i)
				{
					if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
					{
						ImGui::SetKeyboardFocusHere(10);
						ImGui::SetWindowFocus();
					}

					{

						ImGui::SetNextItemWidth(ButtonSize.x);

						const String& SourceName = m_StorageDirectories[m_RenameItems.RenamingID].path().stem().string();
						std::copy(SourceName.begin(), SourceName.end(), m_RenameItems.RenameBuffer);

						if (ImGui::InputText("##rename", m_RenameItems.RenameBuffer, sizeof(m_RenameItems.RenameBuffer),
											 ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
						{

							std::filesystem::rename(m_StorageDirectories[i].path(),
													m_StorageDirectories[i].path().parent_path() / String(m_RenameItems.RenameBuffer).append(".reflect"));

							for (auto& asset : AssetManager::Get().GetLoadedAssets())
							{
								if (asset->GetName() == SourceName)
								{
									const auto& Fields = asset->GetClass()->GetWithParentPropertyFields();
									auto it = std::find_if(Fields.begin(), Fields.end(),
														   [&](CoreEngine::Reflection::PropertyField* Property) { return Property->Name == "Name"; });
									if (it != Fields.end())
									{
										asset->PreEditChangeProperty(*(*it));
										asset->SetName(m_RenameItems.RenameBuffer);
										asset->PostEditChangeProperty(*(*it));
									}
								}
							}

							m_RenameItems.RenamingID = -1;
							std::fill(m_RenameItems.RenameBuffer, m_RenameItems.RenameBuffer + m_RenameItems.BufferSize, '\0');
							CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);

							m_IsRenaming = false;
						}
					}
				}
				else
				{
					ImGui::Text(entry.path().stem().string().c_str());
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();

			bool IsSameLine = (ImGui::GetItemRectMax().x + ImGui::GetStyle().ItemSpacing.x + ButtonSize.x) < RegionWidth;
			if (IsSameLine)
			{
				ImGui::SameLine();
			}

			if (!m_IsRenaming)
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Rename"))
					{
						m_RenameItems.RenamingID = i;
						m_IsRenaming = true;
					}

					ImGui::EndPopup();
				}
			}

			if (IsChangePath)
			{
				break;
			}
		}
		// ImGui::EndChild();
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
				if (entry.is_regular_file() && m_Assts.count(entry.path().string()) <= 0)
				{
					Asset* NewAsset = Engine::Get()->GetAssetManager()->LoadAsset(entry.path().string());
					auto* EditorAsset = dynamic_cast<IEditorDrawingInterface*>(NewAsset);
					if (EditorAsset)
					{
						m_Assts.emplace(entry.path().string(), EditorAsset);
					}
				}
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
	void ContentBrowser::DrawCreateContextMenu()
	{
		if (ImGui::BeginPopupContextWindow("Assets"))
		{
			if (ImGui::MenuItem("Folder"))
			{
				const String& NewFolder = "NewFolder" + Utils::ConvertToString(m_StorageDirectories.size());
				std::filesystem::create_directory(m_CurrentPath / NewFolder);
				CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);
				auto FindedNewFolder =
					std::find_if(m_StorageDirectories.begin(), m_StorageDirectories.end(),
								 [&](std::filesystem::directory_entry& element) -> bool { return element.path().filename().string() == NewFolder; });
				m_RenameItems.RenamingID = std::distance(m_StorageDirectories.begin(), FindedNewFolder);
				m_IsRenaming = true;
			}
			else if (ImGui::MenuItem("Material"))
			{
				const String& NewMaterial = "NewMaterial" + Utils::ConvertToString(m_StorageDirectories.size());
				const String& Path = (m_CurrentPath / (NewMaterial + FileExtension)).string();
				Asset* NewAsset = AssetManager::Get().CreateAsset(Path, CoreEngine::EAssetType::Material);
				auto* EditorAsset = dynamic_cast<IEditorDrawingInterface*>(NewAsset);
				if (NewAsset)
				{
					NewAsset->SetName(NewMaterial);
					m_Assts.emplace(Path, EditorAsset);
					Engine::Get()->GetWorld()->GetSaveManager()->SaveAsset(Path, NewAsset);
				}

				CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);
			}
			else if (ImGui::MenuItem("Shader"))
			{
				const String& NewMaterial = "NewShader" + Utils::ConvertToString(m_StorageDirectories.size());
				const String& Path = (m_CurrentPath / (NewMaterial + FileExtension)).string();
				Asset* NewAsset = AssetManager::Get().CreateAsset(Path, CoreEngine::EAssetType::Shader);
				auto* EditorAsset = dynamic_cast<IEditorDrawingInterface*>(NewAsset);
				if (NewAsset)
				{
					NewAsset->SetName(NewMaterial);
					m_Assts.emplace(Path, EditorAsset);
					Engine::Get()->GetWorld()->GetSaveManager()->SaveAsset(Path, NewAsset);
				}

				CollectAllFilesAndDirectories(m_CurrentPath, m_StorageDirectories);
			}

			ImGui::EndPopup();
		}
	}
	void ContentBrowser::OpenAsset(const String& Path)
	{
		if (m_Assts.count(Path))
		{
			OwnerEditor->SetSelectedObject(dynamic_cast<Asset*>(m_Assts[Path]));
			return;
		}
		Asset* NewAsset = AssetManager::Get().LoadAsset(Path);
		m_Assts.emplace(Path, dynamic_cast<IEditorDrawingInterface*>(NewAsset));

		OwnerEditor->SetSelectedObject(NewAsset);
	}
} // namespace Editor
