#include "bgpch.h"
#include "BagelApplication.h"
#include <GLFW/glfw3.h>

#include "Bagel/Renderer/Renderer.h"

namespace Bagel {
	BagelApplication* BagelApplication::_instance = nullptr;

	BagelApplication::BagelApplication()
	{
		BG_ASSERT(!_instance, "Application already exists!");
		_instance = this;

		_pWindow = std::unique_ptr<Window>(Window::Create());
		_pWindow->SetEventCallback(BG_BIND_EVENT_FN(BagelApplication::OnEvent));

		Renderer::Init();

		_pImGuiLayer = new ImGuiLayer();
		PushOverlay(_pImGuiLayer);
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		while (_running) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized) {
				for (Layer* layer : _layerStack) {
					layer->OnUpdate(timestep);
				}
			}

			_pImGuiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_pImGuiLayer->End();

			_pWindow->OnUpdate();
		}
	}

	void BagelApplication::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BG_BIND_EVENT_FN(BagelApplication::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BG_BIND_EVENT_FN(BagelApplication::OnWindowResize));

		//Goes backwards through the layer stack, allows overlays to handle events before a layer can
		for (auto it = _layerStack.end(); it != _layerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.Handled) break;
		}
	}

	void BagelApplication::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
	}

	void BagelApplication::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
	}

	bool BagelApplication::OnWindowClosed(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool BagelApplication::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) {
			_minimized = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		_minimized = false;

		return false;
	}
}