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
		float dt = ts;

		if (Input::IsKeyPress(BG_KEY_A)) {
			_cameraPosition.x -= _cameraMoveSpeed * dt;
		}
		else if (Input::IsKeyPress(BG_KEY_D)) {
			_cameraPosition.x += _cameraMoveSpeed * dt;
		}

		if (Input::IsKeyPress(BG_KEY_W)) {
			_cameraPosition.y += _cameraMoveSpeed * dt;
		}
		else if (Bagel::Input::IsKeyPress(BG_KEY_S)) {
			_cameraPosition.y -= _cameraMoveSpeed * dt;
		}
		_camera.SetPosition(_cameraPosition);

		if (_rotation) {
			if (Input::IsKeyPress(BG_KEY_Q)) {
				_cameraRotation += _cameraRotationSpeed * dt;
			}
			else if (Input::IsKeyPress(BG_KEY_E)) {
				_cameraRotation -= _cameraRotationSpeed * dt;
			}
			_camera.SetRotation(_cameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(BG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizedEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		_zoomLevel -= e.GetY() * 0.25f;
		_zoomLevel = std::max(_zoomLevel, 0.25f);

		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);

		_cameraMoveSpeed = _zoomLevel;

		return false;
	}

	bool OrthographicCameraController::OnWindowResizedEvent(WindowResizeEvent& e) {
		_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		_camera.SetProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
		return false;
	}
}