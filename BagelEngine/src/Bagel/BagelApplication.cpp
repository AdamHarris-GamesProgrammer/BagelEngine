#include "bgpch.h"
#include "BagelApplication.h"

#include<glad/glad.h>

#include "Input.h"

namespace Bagel {
	BagelApplication* BagelApplication::_instance = nullptr;

	BagelApplication::BagelApplication()
	{
		BG_ASSERT(!_instance, "Application already exists!");
		_instance = this;

		_pWindow = std::unique_ptr<Window>(Window::Create());
		_pWindow->SetEventCallback(BG_BIND_EVENT_FN(BagelApplication::OnEvent));

		_pImGuiLayer = new ImGuiLayer();
		PushOverlay(_pImGuiLayer);
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		while (_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : _layerStack) {
				layer->OnUpdate();
			}

			//auto [x, y] = Input::GetMousePosition();
			//BG_CORE_TRACE("{0}, {1}", x, y);

			_pWindow->OnUpdate();

			_pImGuiLayer->Begin();
			for (Layer* layer : _layerStack)
				layer->OnImGuiRender();
			_pImGuiLayer->End();
		}
	}

	void BagelApplication::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BG_BIND_EVENT_FN(BagelApplication::OnWindowClosed));

		//Goes backwards through the layer stack, allows overlays to handle events before a layer can
		for (auto it = _layerStack.end(); it != _layerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.Handled) break;
		}

		//TEST CODE
		//BG_TRACE(event.ToString());
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

}
