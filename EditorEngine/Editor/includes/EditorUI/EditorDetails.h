#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>

namespace CoreEngine::Runtime
{
	class Object;
}
namespace CoreEngine::Reflection
{
	struct ClassField;
	struct PropertyField;
}

namespace Editor
{
	class EditorDetails : public BaseEditorPanel
	{
	public:

		EditorDetails() = default;

		virtual void Draw() override;

		void SetSelectableObject(CoreEngine::Runtime::Object* Object);

	private:

		void DrawDetailsRecursive(CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Runtime::Object* SourceClass, bool IsDrawTree=true);
		void DrawProperty(CoreEngine::Reflection::PropertyField* Property, CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Reflection::ClassField* MainClass, CoreEngine::Runtime::Object* SourceClass);

		bool HasAnyPropertyDeep(CoreEngine::Reflection::ClassField* Class);
		bool HasAnyProperty(CoreEngine::Reflection::ClassField* Class);

	private:

		CoreEngine::Runtime::Object* SelectedObject{ nullptr };
		HashTableSet<CoreEngine::Reflection::ClassField*> HasEditorRender;
	};
}