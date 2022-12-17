#include <Bagel.h>
#include <imgui/imgui.h>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example"),
		_orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0.0f),
		_aColor(0.8f,0.2f,0.2f,1.0f), _bColor(0.2f,0.2f,0.8f,1.0f)
	{
		_pFlatColorShader = Bagel::Shader::Create("Assets/Shaders/FlatColorShader.glsl");
		_pTexturedShader = Bagel::Shader::Create("Assets/Shaders/TextureShader.glsl");

		_pCrateTexture = Bagel::Texture2D::Create("Assets/Textures/CrateTexture.jpg");
		_pBlendTexture = Bagel::Texture2D::Create("Assets/Textures/BlendTest.png");

		//Vertex Pos (X, Y, Z). Texture coordinate (U, V)
		float squareVertices[4 * 5] = {
			0.2f, 0.0f, 0.0f, 0.0f, 0.0f,	//Bottom Left
			0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	//Bottom Right
			0.5f, 0.3f, 0.0f, 1.0f, 1.0f,	//Top Right
			0.2f, 0.3f, 0.0f, 0.0f, 1.0f	//Top Left
		};

		uint32_t squareIndices[6] = {
			0,1,2,
			3,0,2
		};

		_pTexturedSquareVAO = Bagel::VertexArray::Create();
		_pSquareVAO = Bagel::VertexArray::Create();

		std::shared_ptr<Bagel::VertexBuffer> pSquareVertexBuffer;
		pSquareVertexBuffer = Bagel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Bagel::BufferLayout squareLayout = {
			{Bagel::ShaderDataType::Float3, "a_Position"},
			{Bagel::ShaderDataType::Float2, "a_TextureCoordinate"}
		};
		pSquareVertexBuffer->SetLayout(squareLayout);
		_pSquareVAO->AddVertexBuffer(pSquareVertexBuffer);

		std::shared_ptr<Bagel::IndexBuffer> pSquareIndexBuffer;
		pSquareIndexBuffer = Bagel::IndexBuffer::Create(squareIndices, 6);
		_pSquareVAO->SetIndexBuffer(pSquareIndexBuffer);
	}

	void OnUpdate(Bagel::Timestep timestep) override {
		float dt = timestep;

		if (Bagel::Input::IsKeyPress(BG_KEY_A)) {
			_cameraPosition.x -= _cameraMoveSpeed * dt;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_D)) {
			_cameraPosition.x += _cameraMoveSpeed * dt;
		}

		if (Bagel::Input::IsKeyPress(BG_KEY_W)) {
			_cameraPosition.y += _cameraMoveSpeed * dt;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_S)) {
			_cameraPosition.y -= _cameraMoveSpeed * dt;
		}

		if (Bagel::Input::IsKeyPress(BG_KEY_Q)) {
			_cameraRotation += _cameraRotationSpeed * dt;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_E)) {
			_cameraRotation -= _cameraRotationSpeed * dt;
		}

		_orthographicCamera.SetPosition(_cameraPosition);
		_orthographicCamera.SetRotation(_cameraRotation);

		Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Bagel::RenderCommand::Clear();

		Bagel::Renderer::BeginScene(_orthographicCamera);

		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		bool bIsAColor = false;
		for (float y = 0.0f; y < 2.0f; y += 0.1f) {
			for (float x = 0.0f; x < 2.0f; x += 0.1f) {

				glm::vec3 pos = glm::vec3(x, y, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * rotation * scale;

				const glm::vec4& colorToUpload = bIsAColor ? _aColor : _bColor;

				Bagel::Renderer::Submit(_pFlatColorShader, _pSquareVAO, transform, colorToUpload);

				bIsAColor = !bIsAColor;
			}
		}

		_pTexturedShader->Bind();
		_pTexturedShader->UploadUniformInt("u_Texture", 0);
		_pCrateTexture->Bind(0);
		Bagel::Renderer::Submit(_pTexturedShader, _pSquareVAO);

		_pBlendTexture->Bind(0);
		Bagel::Renderer::Submit(_pTexturedShader, _pSquareVAO);

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
		ImGui::Begin("Color Settings");

		ImGui::ColorEdit3("Color A", glm::value_ptr(_aColor));
		ImGui::ColorEdit3("Color B", glm::value_ptr(_bColor));

		ImGui::End();
	}

private:
	Bagel::Ref<Bagel::VertexArray> _pTexturedSquareVAO;
	Bagel::Ref<Bagel::VertexArray> _pSquareVAO;
	Bagel::Ref<Bagel::Shader> _pFlatColorShader, _pTexturedShader;
	Bagel::Ref<Bagel::Texture2D> _pCrateTexture;
	Bagel::Ref<Bagel::Texture2D> _pBlendTexture;

	Bagel::OrthographicCamera _orthographicCamera;
	glm::vec3 _cameraPosition;
	float _cameraRotation = 0.0f;

	float _cameraMoveSpeed = 0.25f;
	float _cameraRotationSpeed = 10.0f;

	glm::vec4 _aColor;
	glm::vec4 _bColor;
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