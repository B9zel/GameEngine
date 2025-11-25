#pragma once
#include <Core/includes/Application.h>

namespace Editor
{
	class EditorApplication : public CoreEngine::Application
	{
	public:

		EditorApplication(CoreEngine::ApplicationOptions& option);
		~EditorApplication();

	public:

		virtual void CreateApp() override;
		virtual void Start() override;

	protected:

		virtual void ConstructEngine() override;


	private:


	};


}