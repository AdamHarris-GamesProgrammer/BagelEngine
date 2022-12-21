#include <Bagel.h>
#include <imgui/imgui.h>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example"),
		_cameraController(1280.0f / 720.0f),
		_aColor(0.8f,0.2f,0.2f,1.0f), _bColor(0.2f,0.2f,0.8f,1.0f)
	{
		_shaderLibrary.Load("Assets/Shaders/FlatColorShader.glsl");
		_shaderLibrary.Load("Assets/Shaders/TextureShader.glsl");

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

		Bagel::Ref<Bagel::VertexBuffer> pSquareVertexBuffer;
		pSquareVertexBuffer = Bagel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Bagel::BufferLayout squareLayout = {
			{Bagel::ShaderDataType::Float3, "a_Position"},
			{Bagel::ShaderDataType::Float2, "a_TextureCoordinate"}
		};
		pSquareVertexBuffer->SetLayout(squareLayout);
		_pSquareVAO->AddVertexBuffer(pSquareVertexBuffer);

		Bagel::Ref<Bagel::IndexBuffer> pSquareIndexBuffer;
		pSquareIndexBuffer = Bagel::IndexBuffer::Create(squareIndices, 6);
		_pSquareVAO->SetIndexBuffer(pSquareIndexBuffer);
	}

	void OnUpdate(Bagel::Timestep timestep) override {
		float dt = timestep;

		_cameraController.OnUpdate(timestep);

		Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Bagel::RenderCommand::Clear();

		Bagel::Renderer::BeginScene(_cameraController.GetCamera());

		glm::mat4 transform = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		const auto& flatColor = _shaderLibrary.Get("FlatColorShader");

		bool bIsAColor = false;
		for (float y = 0.0f; y < 2.0f; y += 0.1f) {
			for (float x = 0.0f; x < 2.0f; x += 0.1f) {

				glm::vec3 pos = glm::vec3(x, y, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * rotation * scale;

				const glm::vec4& colorToUpload = bIsAColor ? _aColor : _bColor;

				Bagel::Renderer::Submit(flatColor, _pSquareVAO, transform, colorToUpload);

				bIsAColor = !bIsAColor;
			}
		}

		const auto& textureShader = _shaderLibrary.Get("TextureShader");

		textureShader->Bind();
		textureShader->UploadUniformInt("u_Texture", 0);
		_pCrateTexture->Bind(0);
		Bagel::Renderer::Submit(textureShader, _pSquareVAO);

		_pBlendTexture->Bind(0);
		Bagel::Renderer::Submit(textureShader, _pSquareVAO);

		Bagel::Renderer::EndScene();
	}

	void OnEvent(Bagel::Event& event) override {
		Bagel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Bagel::KeyPressedEvent>(BG_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	
		_cameraController.OnEvent(event);

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
	Bagel::Ref<Bagel::Texture2D> _pCrateTexture;
	Bagel::Ref<Bagel::Texture2D> _pBlendTexture;

	Bagel::ShaderLibrary _shaderLibrary;

	glm::vec4 _aColor;
	glm::vec4 _bColor;

	Bagel::OrthographicCameraController _cameraController;
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