#pragma once

#include "Bagel/Renderer/OrthographicCamera.h"
#include "Bagel/Core/TimeStep.h"

#include "Bagel/Events/ApplicationEvent.h"
#include "Bagel/Events/MouseEvent.h"

namespace Bagel {

	class OrthographicCameraController {
	public:
		
		OrthographicCameraController(float aspectratio, bool rotation = false);

		void OnUpdate(Timestep ts);

		void OnEvent(Event& e);

		const OrthographicCamera& GetCamera() const {
			return _camera;
		}
		
		void SetZoomLevel(float level) {
			_zoomLevel = level;
		}

		float GetZoomLevel() const {
			return _zoomLevel;
		}

	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);


	private:
		float _aspectRatio;
		float _zoomLevel = 1.0f;

		glm::vec3 _cameraPosition = glm::vec3(0.0f);
		float _cameraRotation = 0.0f;

		float _cameraMoveSpeed = 1.5f;
		float _cameraRotationSpeed = 1.0f;

		bool _rotation;


		OrthographicCamera _camera;
	};
}
