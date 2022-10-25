#include <Bagel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Bagel::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		//BG_INFO("ExampleLayer::OnUpdate");

	}

	void OnEvent(Bagel::Event& event) override {
		//BG_TRACE("{0}", event);

		if (event.GetEventType() == Bagel::EventType::KeyPressed) {
			Bagel::KeyPressedEvent& e = (Bagel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == BG_KEY_TAB) BG_TRACE("Tab key is pressed!");

			BG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	virtual void OnImGuiRender() override {
		//ImGui::Begin("Hello World");
		//ImGui::Text("Testing");
		//ImGui::End(); 
	}
};

class SandboxApplication : public Bagel::BagelApplication {
public:
	SandboxApplication() {
		PushLayer(new ExampleLayer());
	}

	~SandboxApplication() {

	}

};

Bagel::BagelApplication* Bagel::CreateApplication() {
	return new SandboxApplication();
}