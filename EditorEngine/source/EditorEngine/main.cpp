
#include <iostream>
#include <Core/includes/Log.h>
#include <Core/includes/Application.h>

DECLARE_LOG_CATEGORY_EXTERN(EDITOR)



int main(int argc, char** argv)
{	
	CoreEngine::ApplicationOptions options("Test", argv[0]);
	auto app = MakeUniquePtr<CoreEngine::Application>(options);
	app->Start();

}