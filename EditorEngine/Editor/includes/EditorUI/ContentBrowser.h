#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>
#include <filesystem>

namespace CoreEngine::Render
{
	class Texture2D;
}

namespace Editor
{


	class ContentBrowser : public BaseEditorPanel
	{
	public:

		ContentBrowser() = default;
		virtual void Draw() override;
		virtual void OnConstruct() override;

	private:

		void DrawMainContent();
		void DrawFoldersAndFiles();
		void DrawFilderTree();
		void DrawLeftDirectoriesPanel(const std::filesystem::path& Path);

		bool HasAnySubdirectory(const std::filesystem::path& Path);

		void CollectAllFilesAndDirectories(const std::filesystem::path& Path, DArray<std::filesystem::directory_entry>& StorageDirectories);
		void SetCurrentPath(const std::filesystem::path& NewPath);

	private:

		std::filesystem::path m_CurrentPath;
		std::filesystem::path m_LastPath;
		DArray<std::filesystem::directory_entry> m_StorageDirectories;

		CoreEngine::Render::Texture2D* Folder;
		CoreEngine::Render::Texture2D* File;
	};
} // namespace Editor
