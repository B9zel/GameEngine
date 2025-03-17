#include <Core/includes/InputDevice.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>
#include <Core/includes/Log.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{

	bool InputDevice::GetIsKeyPressed(uint32 key)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());

		return glfwGetKey(window, key) == GLFW_PRESS;
	}
	bool InputDevice::GetIsButtonPressed(uint32 button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());

		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	void InputDevice::SetMousePos(const DVector2& Pos)
	{
		glfwSetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow()), Pos.x, Pos.y);
	}

	DVector2 InputDevice::GetMousePos()
	{
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow()), &x, &y);
		return DVector2(x, y);
	}


	void InputDevice::InviteEvent(Event& event)
	{

		m_dispatch.Call(event);
	}
}