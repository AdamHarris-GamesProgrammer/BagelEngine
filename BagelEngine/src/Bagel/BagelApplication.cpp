#include "bgpch.h"
#include "BagelApplication.h"

#include<glad/glad.h>

#include "Input.h"

#include "Bagel/Renderer/Shader.h"
#include "Bagel/Renderer/Buffer.h"
#include "Bagel/Renderer/VertexArray.h"

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

		_pVAO.reset(VertexArray::Create());

		//Anti clock wise by default //Fullscreen Rect
		float vertices[4 * 7] = {
			-1.0f, -1.0f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, //BL
			1.0f, -1.0f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, //BR
			1.0f, 1.0f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f, //TR
			-1.0f, 1.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f //TL
		};

		std::shared_ptr<VertexBuffer> pVertexBuffer;
		pVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};
			pVertexBuffer->SetLayout(layout);
		}

		_pVAO->AddVertexBuffer(pVertexBuffer);

		unsigned int indices[6] = {
			0, 1, 2,
			0, 2, 3
		};

		std::shared_ptr<IndexBuffer> pIndexBuffer;
		pIndexBuffer.reset(IndexBuffer::Create(indices, 6));
		_pVAO->SetIndexBuffer(pIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5,1.0);
				color = v_Color;
			}
		)";

		_pShader.reset(Shader::Create(vertexSrc, fragmentSrc));
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		while (_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_pShader->Bind();
			_pVAO->Bind();

			glDrawElements(GL_TRIANGLES, _pVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
