#pragma once

#include "Bagel/Window.h"


#include <GLFW/glfw3.h>

namespace Bagel {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		// Inherited via Window
		virtual unsigned int GetWidth() const override { return _data.width; };
		virtual unsigned int GetHeight() const override { return _data.height; };
		virtual void SetEventCallback(const EventCallbackFn& callback) override { _data.eventCallback = callback; };

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return _data.vsync; };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* _pWindow;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vsync;

			EventCallbackFn eventCallback;
		};

		WindowData _data;

		// Inherited via Window
		inline virtual void* GetNativeWindow() const override { return _pWindow; }
	};
}


