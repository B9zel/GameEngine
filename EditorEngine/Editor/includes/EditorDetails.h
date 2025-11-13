#pragma once
#include <Editor/includes/BaseEditorPanel.h>


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
	class EditorDetails : public BaseEdtiorPanel
	{
	public:

		EditorDetails() = default;

		virtual void Draw() override;

		void SetSelectableObject(CoreEngine::Runtime::Object* Object);

	private:

		void DrawDetailsRecursive(CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Runtime::Object* SourceClass, bool IsDrawTree=true);
		void DrawProperty(CoreEngine::Reflection::PropertyField* Property, CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Reflection::ClassField* MainClass, CoreEngine::Runtime::Object* SourceClass);
		bool HasAnyProperty(CoreEngine::Reflection::ClassField* Class);

	private:

		CoreEngine::Runtime::Object* SelectedObject{ nullptr };
	};
}