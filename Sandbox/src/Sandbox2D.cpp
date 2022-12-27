#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(const std::string& name)
	: Layer(name), _cameraController(1280.0f / 720.f), _color(glm::vec4(1.0f,1.0f,1.0f,1.0f)) {}

void Sandbox2D::OnAttach()
{
	_pCrateTexture = Bagel::Texture2D::Create("Assets/Textures/CrateTexture.jpg");
	}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Bagel::Timestep timestep)
{
	_cameraController.OnUpdate(timestep);

	Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	Bagel::RenderCommand::Clear();

	Bagel::Renderer2D::BeginScene(_cameraController.GetCamera());

	_pCrateTexture->Bind(0);

	Bagel::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(0.3f));
	Bagel::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.5f), glm::vec2(0.5f), 90.0f, glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

	Bagel::Renderer2D::EndScene();
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
