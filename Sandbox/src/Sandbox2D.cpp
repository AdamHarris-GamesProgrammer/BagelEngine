#include "Sandbox2D.h"

#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>


template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func)
		: _name(name), _stopped(false), _func(func)
	{
		_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!_stopped) Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		_stopped = true;

		auto duration = (end - start) * 0.001f;

		//BG_INFO("{0}: Durtation {1}(ms)", _name, duration);

		_func({ _name, duration });
	}



private:
	const char* _name;
	std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
	bool _stopped;
	Fn _func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult){_profilers.push_back(profileResult);});

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
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		_cameraController.OnUpdate(timestep);
	}

	{
		PROFILE_SCOPE("Render Prep");
		Bagel::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Bagel::RenderCommand::Clear();
		Bagel::Renderer2D::BeginScene(_cameraController.GetCamera());
	}

	{
		PROFILE_SCOPE("Render Draw Calls");
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
	ImGui::Begin("Sandbox Testing");
	ImGui::ColorEdit4("square color", glm::value_ptr(_color));
	ImGui::DragFloat("square rot", &_testRot, 1.0f, 0.0f, 360.0f);
	ImGui::End();

	ImGui::Begin("Profiling");

	for (const auto& result : _profilers) {
		char label[50];
		strcpy(label, " %.3fms\t");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
		ImGui::Separator();
	}

	_profilers.clear();

	ImGui::End();
}
