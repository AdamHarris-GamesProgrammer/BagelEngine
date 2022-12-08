#pragma once

#include <glm/glm.hpp>

namespace Bagel {
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return _position; }
		void SetPosition(const glm::vec3& position) { _position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return _rotation; }
		void SetRotation(float rotation) { _rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& ViewProj() const { return _viewProjMat; }
		const glm::mat4& Proj() const { return _projMat; }
		const glm::mat4& View() const { return _viewMat; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 _position = glm::vec3(0.0f);
		float _rotation = 0.0f;

		glm::mat4 _viewMat;
		glm::mat4 _projMat;
		glm::mat4 _viewProjMat;
	};
}
