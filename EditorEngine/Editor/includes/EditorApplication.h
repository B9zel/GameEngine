#pragma once
#include <Core/includes/Application.h>
#include "EditorLayer.h"

namespace Editor
{
	class EditorApplication : public CoreEngine::Application
	{
	public:

		EditorApplication(CoreEngine::ApplicationOptions& option);
		~EditorApplication();

	public:

		virtual void CreateApp() override;

	protected:

		virtual void ConstructEngine() override;


	private:


	};


}