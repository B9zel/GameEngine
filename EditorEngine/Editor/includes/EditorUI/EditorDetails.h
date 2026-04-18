#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>

class Object;
class Actor;

namespace CoreEngine::Reflection
{
	struct ClassField;
	struct PropertyField;
} // namespace CoreEngine::Reflection

namespace Editor
{
	class EditorDetails : public BaseEditorPanel
	{
	public:

		EditorDetails() = default;

		virtual void Draw() override;
		virtual void OnConstruct() override;

		void SetSelectableObject(Object* Object);

	private:

		void DrawDetailsRecursive(Object* SelectedObject, Object* SourceClass, bool IsDrawTree = true);
		void DrawProperty(CoreEngine::Reflection::PropertyField* Property, Object* SelectedObject,
						  CoreEngine::Reflection::ClassField* MainClass, Object* SourceClass);

		bool HasAnyPropertyDeep(CoreEngine::Reflection::ClassField* Class);
		bool HasAnyProperty(CoreEngine::Reflection::ClassField* Class);

	private:

		Object* SelectedObject{nullptr};
		HashTableSet<CoreEngine::Reflection::ClassField*> HasEditorRender;
	};
} // namespace Editor
