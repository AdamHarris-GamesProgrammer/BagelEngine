#include "bgpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Bagel {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _viewMat(glm::mat4(1.0f)), _projMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		_position(0.0f, 0.0f, 0.0f)
	{
		_viewProjMat = _projMat * _viewMat;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		BG_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		_viewMat = glm::inverse(transform);
		_viewProjMat = _projMat * _viewMat;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		BG_PROFILE_FUNCTION();
		_projMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		_viewProjMat = _projMat * _viewMat;
	}
}