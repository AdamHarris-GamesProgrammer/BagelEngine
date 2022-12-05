#include "bgpch.h"
#include "BagelApplication.h"

#include<glad/glad.h>

#include "Input.h"

#include "Bagel/Renderer/Shader.h"
#include "Bagel/Renderer/Buffer.h"

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

		/*Abstraction Tasks
			- Vertex Array
			- Shader creation and usage
		*/

		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		//Anti clock wise by default //Fullscreen Rect
		float vertices[3 * 4] = {
			-1.0f, -1.0f, 0.0f, //BL
			1.0f, -1.0f, 0.0f, //BR
			1.0f, 1.0f, 0.0f, //TR
			-1.0f, 1.0f, 0.0f //TL
		};
		//float vertices[3 * 4] = { //Rhombus Demo
		//	-0.5f, 0.0f, 0.0f, //Left
		//	0.0f, -0.5f, 0.0f, //Bottom
		//	0.5f, 0.0f, 0.0f, //Right
		//	0.0f, 0.5f, 0.0f //Top
		//};
		_pVBO.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		unsigned int indices[6] = {
			0, 1, 2,
			0, 2, 3
		};
		_pIBO.reset(IndexBuffer::Create(indices, 6));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5,1.0);
			}
		)";

		_pShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
	}

	BagelApplication::~BagelApplication()
	{
		delete _pImGuiLayer;
		_pImGuiLayer = nullptr;
	}

	void BagelApplication::Run()
	{
		while (_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_pShader->Bind();
			glBindVertexArray(_VAO);

			glDrawElements(GL_TRIANGLES, _pIBO->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : _layerStack) {
				layer->OnUpdate();
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

}
