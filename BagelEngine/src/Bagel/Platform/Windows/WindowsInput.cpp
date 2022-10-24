#include "bgpch.h"
#include "WindowsInput.h"

#include "Bagel/BagelApplication.h"
#include <GLFW/glfw3.h>

namespace Bagel {
	Input* Input::_instance = new WindowsInput();

	bool WindowsInput::IsKeyPressImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(BagelApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsKeyUpImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(BagelApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_RELEASE;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(BagelApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(BagelApplication::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::make_pair((float)xPos, (float)yPos);
	}
}