#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>
#include <filesystem>

class Asset;

namespace CoreEngine::Render
{
	class Texture2D;
}

namespace Editor
{
	class IEditorDrawingInterface;

	struct UIContentItem
	{
		static const int32 BufferSize = 128;
		int32 RenamingID = -1;
		char RenameBuffer[BufferSize];
	};

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

		void DrawCreateContextMenu();

		void OpenAsset(const String& Path);

	private:

		std::filesystem::path m_CurrentPath;
		std::filesystem::path m_LastPath;
		DArray<std::filesystem::directory_entry> m_StorageDirectories;
		// HashTableMap<String, Asset*> m_Assts;
		HashTableMap<String, IEditorDrawingInterface*> m_Assts;
		UIContentItem m_RenameItems;

		CoreEngine::Render::Texture2D* Folder;
		CoreEngine::Render::Texture2D* File;

		const String FileExtension = ".reflect";

		bool m_IsRenaming{false};
		bool m_IsDrawContextMenu{false};
	};
} // namespace Editor
