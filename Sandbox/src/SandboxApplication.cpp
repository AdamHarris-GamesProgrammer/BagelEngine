#include <Bagel.h>
#include <Bagel/Core/EntryPoint.h>

#include <imgui/imgui.h>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate(Bagel::Timestep timestep) override {

	}

	void OnEvent(Bagel::Event& event) override {

	}

	bool OnKeyPressedEvent(Bagel::KeyPressedEvent& event) {
		return false;
	}

	virtual void OnImGuiRender() override {

	}

private:
};

class SandboxApplication : public Bagel::BagelApplication {
public:
	SandboxApplication() : BagelApplication() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D("Sandbox2D"));
	}

	~SandboxApplication() {

	}
};

Bagel::BagelApplication* Bagel::CreateApplication() {
	return new SandboxApplication();
}