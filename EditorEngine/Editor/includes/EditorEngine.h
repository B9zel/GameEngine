#pragma once
#include <Core/includes/Engine.h>


namespace Editor
{
	using namespace CoreEngine;

	class EditorEngine : public CoreEngine::Engine
	{
	public:

		using ThisClass = EditorEngine;

	public:

		EditorEngine() = default;

	public:

		virtual void Update() override;

	protected:

		void RenderEditor();

	private:

		float f;
		String buf;
		float my_color[4];
		bool my_tool_active{ true };

	};
}