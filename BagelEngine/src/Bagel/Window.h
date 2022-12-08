#pragma once

#include "bgpch.h"

#include "Bagel/Core.h"
#include "Bagel/Events/Event.h"
#include "Bagel/Renderer/RendererAPI.h"

namespace Bagel {
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Bagel Engine", unsigned int width = 1280, unsigned int height = 720) : title(title), width(width), height(height) 
		{
		
		}
	};

	//Base class/interface responsible for representing a window, contains common methods that all platform windows will require
	class BG_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth()  const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		//Implemented on a per platform basis
		static Window* Create(const WindowProps& properties = WindowProps());


		virtual void* GetNativeWindow() const = 0;
	};
}