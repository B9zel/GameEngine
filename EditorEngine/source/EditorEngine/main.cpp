#pragma once

#include <iostream>
#include <Core/includes/Log.h>
#include <Core/includes/Application.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Core/includes/GBNotify.h>
#include <Templates/Function.h>
#include <Core/includes/Base.h>
#include <iostream>
DECLARE_LOG_CATEGORY_EXTERN(EDITOR)


float arr[] = { -1.f, 1,
				1, 1,
				-1,-1,
				1, -1,
				1,1,
				-1,-1, };

const char* vv = "\n"
"#version 330 core\n"
"layout (location = 0) in vec2 position; \n"
"void main() \n"
"{ \n"
"	gl_Position = vec4(position.x,position.y, 1,1); \n"
"}";

const char* ff = "\n"
"#version 330 core\n"
"uniform float time;"
"out vec4 color; \n"
"void main() \n"
"{ \n"
"	float x = gl_FragCoord.x / 800; \n"
"	float y =  gl_FragCoord.y / 400; \n"	
"	color = vec4(1 * x, 1 * x, 1 * x,0); \n"
"}";

class EditorLayer : public CoreEngine::Layer
{
	virtual void OnAttach() override {}
	virtual void OnDetach() override {}
	virtual void Update(float deltaTime) override {}
};

		
class EditorApplication : public CoreEngine::Application
{
public:
	EditorApplication(const CoreEngine::ApplicationOptions& options) : Application(options)
	{
		PushLayer(new EditorLayer());
	}
	
};

class Test
{
public:
	Test() { std::cout << "Test" << std::endl; }

	void Notfy(CoreEngine::Runtime::Object* p, CoreEngine::Runtime::Object* newPtr)
	{
		std::cout << p << "   " << newPtr << std::endl;
	}
};



int main(int argc, char** argv)
{	
	Test t;
	int b = 3;
	int c = 6;
	int* p = &b;
	//int** pp = &p;

	
	CoreEngine::ApplicationOptions options("Test", argv[0]);
	auto app = MakeUniquePtr<EditorApplication>(options);

	//CoreEngine::GB::GBNotify<int*> a(p, Function<void(CoreEngine::Runtime::Object*, CoreEngine::Runtime::Object*)>(&Test::Notfy,&t));
	//a = &c;
	app->Start();

}