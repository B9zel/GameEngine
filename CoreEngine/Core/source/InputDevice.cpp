#include <Core/includes/InputDevice.h>
#include <Core/includes/Application.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{

	bool InputDevice::GetIsKeyPressed(unsigned int key)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());
		
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
	bool InputDevice::GetIsButtonPressed(unsigned int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());

		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}


	void InputDevice::InviteEvent(Event& event)
	{
		m_dispatch.Call(event);
	}
}