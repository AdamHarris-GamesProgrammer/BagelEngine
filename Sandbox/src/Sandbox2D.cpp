#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(const std::string& name)
	: Layer(name), _cameraController(1280.0f / 720.f), _color(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)) {}

void Sandbox2D::OnAttach()
{
	_pCrateTexture = Bagel::Texture2D::Create("Assets/Textures/CrateTexture.jpg");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Bagel::Timestep timestep)
{
	BG_PROFILE_FUNCTION();

	{
		BG_PROFILE_SCOPE("CameraController::OnUpdate");
		_cameraController.OnUpdate(timestep);
	}

	{
		BG_PROFILE_SCOPE("Render Prep");
		Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Bagel::RenderCommand::Clear();
		Bagel::Renderer2D::BeginScene(_cameraController.GetCamera());
	}

	{
		BG_PROFILE_SCOPE("Render Draw Calls");
		Bagel::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(0.3f));
		Bagel::Renderer2D::DrawQuad(glm::vec2(-0.7f, 0.7f), glm::vec2(0.3f), _testRot, _color);
		Bagel::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.3f), glm::vec2(10.0f), 90.0f, _pCrateTexture, glm::vec4(0.3f, 0.2f, 0.3f, 1.0f));
	}

	Bagel::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Bagel::Event& event)
{
	_cameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	BG_PROFILE_FUNCTION();

	ImGui::Begin("Sandbox Testing");
	ImGui::ColorEdit4("square color", glm::value_ptr(_color));
	ImGui::DragFloat("square rot", &_testRot, 1.0f, 0.0f, 360.0f);
	ImGui::End();

	ImGui::Begin("Profiling");

	//for (const auto& result : _profilers) {
	//	char label[50];
	//	strcpy(label, " %.3fms\t");
	//	strcat(label, result.name);
	//	ImGui::Text(label, result.time);
	//	ImGui::Separator();
	//}

	ImGui::End();
}
