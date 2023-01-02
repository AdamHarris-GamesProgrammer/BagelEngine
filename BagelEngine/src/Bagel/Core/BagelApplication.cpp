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
		//_pWindow->SetVSync(false);

		Renderer::Init();

		_pImGuiLayer = new ImGuiLayer();
		PushOverlay(_pImGuiLayer);
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		BG_PROFILE_FUNCTION();

		while (_running) {
			BG_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized) {
				{
					BG_PROFILE_SCOPE("Application::Run(): Updating Layers")

						for (Layer* layer : _layerStack) {
							layer->OnUpdate(timestep);
						}
				}
			}

			_pImGuiLayer->Begin();
			{
				BG_PROFILE_SCOPE("ImGUI Render");
				for (Layer* layer : _layerStack)
					layer->OnImGuiRender();
			}
			_pImGuiLayer->End();

			_pWindow->OnUpdate();
		}
	}

	void BagelApplication::OnEvent(Event& event)
	{
		BG_PROFILE_FUNCTION();

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
		BG_PROFILE_FUNCTION();
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void BagelApplication::PushOverlay(Layer* layer)
	{
		BG_PROFILE_FUNCTION();
		_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool BagelApplication::OnWindowClosed(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool BagelApplication::OnWindowResize(WindowResizeEvent& e)
	{
		BG_PROFILE_FUNCTION()

		if (e.GetWidth() == 0.0f || e.GetHeight() == 0.0f) {
			_minimized = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		_minimized = false;

		return false;
	}
}
