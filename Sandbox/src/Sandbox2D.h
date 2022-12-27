#pragma once

#include <Bagel.h>

//#include <Bagel/Renderer/Shader.h>
//#include "Bagel/Core/Layer.h"

class Sandbox2D : public Bagel::Layer {
public:
	Sandbox2D(const std::string& name);
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Bagel::Timestep timestep) override;
	virtual void OnEvent(Bagel::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	//Temp
	Bagel::Ref<Bagel::Texture2D> _pCrateTexture;

	glm::vec4 _color;

	Bagel::OrthographicCameraController _cameraController;
};