#include "bgpch.h"
#include "OrthographicCameraController.h"

#include "Bagel/Core/KeyCodes.h"
#include "Bagel/Core/Input.h"

namespace Bagel {
	OrthographicCameraController::OrthographicCameraController(float aspectratio, bool rotation)
		: _aspectRatio(aspectratio), _rotation(rotation), _camera(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		BG_PROFILE_FUNCTION();

		float dt = ts;

		if (Input::IsKeyPress(BG_KEY_A)) {
			_cameraPosition.x -= cos(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
			_cameraPosition.y -= sin(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
		}
		else if (Input::IsKeyPress(BG_KEY_D)) {
			_cameraPosition.x += cos(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
			_cameraPosition.y += sin(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
		}

		if (Input::IsKeyPress(BG_KEY_W)) {
			_cameraPosition.x += -sin(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
			_cameraPosition.y += cos(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt; 
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_S)) {
			_cameraPosition.x -= -sin(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
			_cameraPosition.y -= cos(glm::radians(_cameraRotation)) * _cameraMoveSpeed * dt;
		}
		_camera.SetPosition(_cameraPosition);

		if (_rotation) {
			if (Input::IsKeyPress(BG_KEY_Q)) {
				_cameraRotation += _cameraRotationSpeed * dt;
			}
			else if (Input::IsKeyPress(BG_KEY_E)) {
				_cameraRotation -= _cameraRotationSpeed * dt;
			}

			if (_cameraRotation > 180.0f) {
				_cameraRotation -= 360.0f;
			}
			else if (_cameraRotation <= -180.0f) {
				_cameraRotation += 360.0f;
			}

			_camera.SetRotation(_cameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		BG_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(BG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizedEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		BG_PROFILE_FUNCTION();

		_zoomLevel -= e.GetY() * 0.25f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);

		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		_cameraMoveSpeed = _zoomLevel;

		return false;
	}

	bool OrthographicCameraController::OnWindowResizedEvent(WindowResizeEvent& e) {
		BG_PROFILE_FUNCTION();

		_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}
}