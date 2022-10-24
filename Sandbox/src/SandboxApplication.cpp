#include <Bagel.h>

class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		//BG_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Bagel::Event& event) override {
		//BG_TRACE("{0}", event);
	}
};

class SandboxApplication : public Bagel::BagelApplication {
public:
	SandboxApplication() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Bagel::ImGuiLayer());
	}

	~SandboxApplication() {

	}

};

Bagel::BagelApplication* Bagel::CreateApplication() {
	return new SandboxApplication();
}