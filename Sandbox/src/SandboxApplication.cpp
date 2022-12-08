#include <Bagel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example"),
		_orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0.0f)
	{
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec4 v_Color;

			void main() {
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

			uniform mat4 u_ViewProjection;

			void main() {
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main() {
				color = vec4(0.2, 0.2,0.8,1.0);
			}
		)";

		_pShader.reset(Bagel::Shader::Create(vertexSrc, fragmentSrc));
		_pBlueShader.reset(Bagel::Shader::Create(blueVertexSrc, blueFragmentSrc));

		//Anti clock wise by default 
		float triangleVertices[3 * 7] = {
			-0.5f, 0.0f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,	//Left
			-0.2f, 0.0f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f,	//Right
			-0.35f, 0.3f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f	//Top
		};

		unsigned int triangleIndices[3] = {
			0, 1, 2
		};

		float squareVertices[4 * 3] = {
			0.2f, 0.0f, 0.0f,	//Bottom Left
			0.5f, 0.0f, 0.0f,	//Bottom Right
			0.5f, 0.3f, 0.0f,	//Top Right
			0.2f, 0.3f, 0.0f	//Top Left
		};

		uint32_t squareIndices[6] = {
			0,1,2,
			3,0,2
		};

		_pTriangleVAO.reset(Bagel::VertexArray::Create());

		std::shared_ptr<Bagel::VertexBuffer> pVertexBuffer;
		pVertexBuffer.reset(Bagel::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
		Bagel::BufferLayout layout = {
			{ Bagel::ShaderDataType::Float3, "a_Position" },
			{ Bagel::ShaderDataType::Float4, "a_Color" }
		};
		pVertexBuffer->SetLayout(layout);
		_pTriangleVAO->AddVertexBuffer(pVertexBuffer);

		std::shared_ptr<Bagel::IndexBuffer> pIndexBuffer;
		pIndexBuffer.reset(Bagel::IndexBuffer::Create(triangleIndices, 3));
		_pTriangleVAO->SetIndexBuffer(pIndexBuffer);


		_pSquareVAO.reset(Bagel::VertexArray::Create());

		std::shared_ptr<Bagel::VertexBuffer> pSquareVertexBuffer;
		pSquareVertexBuffer.reset(Bagel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Bagel::BufferLayout squareLayout = {
			{Bagel::ShaderDataType::Float3, "a_Position"}
		};
		pSquareVertexBuffer->SetLayout(squareLayout);
		_pSquareVAO->AddVertexBuffer(pSquareVertexBuffer);

		std::shared_ptr<Bagel::IndexBuffer> pSquareIndexBuffer;
		pSquareIndexBuffer.reset(Bagel::IndexBuffer::Create(squareIndices, 6));
		_pSquareVAO->SetIndexBuffer(pSquareIndexBuffer);
	}

	void OnUpdate() override {
		if (Bagel::Input::IsKeyPress(BG_KEY_A)) {
			_cameraPosition.x -= _cameraMoveSpeed;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_D)) {
			_cameraPosition.x += _cameraMoveSpeed;
		}

		if (Bagel::Input::IsKeyPress(BG_KEY_W)) {
			_cameraPosition.y += _cameraMoveSpeed;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_S)) {
			_cameraPosition.y -= _cameraMoveSpeed;
		}

		if (Bagel::Input::IsKeyPress(BG_KEY_Q)) {
			_cameraRotation += _cameraRotationSpeed;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_E)) {
			_cameraRotation -= _cameraRotationSpeed;
		}

		_orthographicCamera.SetPosition(_cameraPosition);
		_orthographicCamera.SetRotation(_cameraRotation);

		Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Bagel::RenderCommand::Clear();

		Bagel::Renderer::BeginScene(_orthographicCamera);

		Bagel::Renderer::Submit(_pShader, _pTriangleVAO);
		Bagel::Renderer::Submit(_pBlueShader, _pSquareVAO);

		Bagel::Renderer::EndScene();
	}

	void OnEvent(Bagel::Event& event) override {
		Bagel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Bagel::KeyPressedEvent>(BG_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
		
	}

	bool OnKeyPressedEvent(Bagel::KeyPressedEvent& event) {

		return false;
	}

	virtual void OnImGuiRender() override {
	}

private:
	std::shared_ptr<Bagel::VertexArray> _pTriangleVAO;
	std::shared_ptr<Bagel::VertexArray> _pSquareVAO;

	std::shared_ptr<Bagel::Shader> _pShader;
	std::shared_ptr<Bagel::Shader> _pBlueShader;

	Bagel::OrthographicCamera _orthographicCamera;
	glm::vec3 _cameraPosition;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 0.1f;
	float _cameraRotationSpeed = 0.05f;
};

class SandboxApplication : public Bagel::BagelApplication {
public:
	SandboxApplication() : BagelApplication() {
		PushLayer(new ExampleLayer());
	}

	~SandboxApplication() {

	}

};

Bagel::BagelApplication* Bagel::CreateApplication() {
	return new SandboxApplication();
}