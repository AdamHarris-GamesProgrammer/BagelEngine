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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			void main() {
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec4 v_Color;

			void main() {
				color = v_Color;
			}
		)";

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			void main() {
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main() {
				color = vec4(0.2, 0.2,0.8,1.0);
			}
		)";

		_pShader.reset(Shader::Create(vertexSrc, fragmentSrc));
		_pBlueShader.reset(Shader::Create(blueVertexSrc, blueFragmentSrc));

		//Anti clock wise by default 
		float triangleVertices[3 * 7] = {
			-0.5f, 0.0f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f, //Left
			-0.2f, 0.0f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f, //Right
			-0.35f, 0.3f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f //Top
		};

		unsigned int triangleIndices[3] = {
			0, 1, 2
		};

		float squareVertices[4 * 3] = {
			0.2f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,
			0.5f, 0.3f, 0.0f,
			0.2f, 0.3f, 0.0f
		};

		uint32_t squareIndices[6] = {
			0,1,2,
			3,0,2
		};

		_pTriangleVAO.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> pVertexBuffer;
		pVertexBuffer.reset(VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		pVertexBuffer->SetLayout(layout);
		_pTriangleVAO->AddVertexBuffer(pVertexBuffer);

		std::shared_ptr<IndexBuffer> pIndexBuffer;
		pIndexBuffer.reset(IndexBuffer::Create(triangleIndices, 3));
		_pTriangleVAO->SetIndexBuffer(pIndexBuffer);


		_pSquareVAO.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> pSquareVertexBuffer;
		pSquareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		pSquareVertexBuffer->SetLayout(squareLayout);
		_pSquareVAO->AddVertexBuffer(pSquareVertexBuffer);


		std::shared_ptr<IndexBuffer> pSquareIndexBuffer;
		pSquareIndexBuffer.reset(IndexBuffer::Create(squareIndices, 6));
		_pSquareVAO->SetIndexBuffer(pSquareIndexBuffer);
	}

	BagelApplication::~BagelApplication()
	{
	}

	void BagelApplication::Run()
	{
		while (_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_pTriangleVAO->Bind();
			_pShader->Bind();
			glDrawElements(GL_TRIANGLES, _pTriangleVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			_pSquareVAO->Bind();
			_pBlueShader->Bind();
			glDrawElements(GL_TRIANGLES, _pSquareVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


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
