#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(const std::string& name)
	: Layer(name), _cameraController(1280.0f / 720.f), _color(glm::vec4(1.0f,1.0f,1.0f,1.0f)) {}

void Sandbox2D::OnAttach()
{
	_shader = Bagel::Shader::Create("Assets/Shaders/TextureShader.glsl");
	_pCrateTexture = Bagel::Texture2D::Create("Assets/Textures/CrateTexture.jpg");

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

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Bagel::Timestep timestep)
{
	_cameraController.OnUpdate(timestep);

	Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Bagel::RenderCommand::Clear();

	Bagel::Renderer::BeginScene(_cameraController.GetCamera());

	_shader->Bind();
	_shader->UploadUniformInt("u_Texture", 0);
	_shader->UploadUniformFloat4("u_Color", _color);

	_pCrateTexture->Bind(0);

	Bagel::Renderer::Submit(_shader, _pSquareVAO);

	Bagel::Renderer::EndScene();
}

void Sandbox2D::OnEvent(Bagel::Event& event)
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Color");
	ImGui::ColorEdit4("square color", glm::value_ptr(_color));
	ImGui::End();
}
