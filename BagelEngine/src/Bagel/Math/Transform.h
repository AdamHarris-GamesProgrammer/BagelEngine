#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct Transform {
	Transform(glm::vec3 pos = glm::vec3(1.0f), glm::vec2 size = glm::vec2(1.0f), float degrees = 0.0f) {
		const static glm::mat4 baseMat = glm::mat4(1.0f);
		transformMat = glm::translate(baseMat, pos) * glm::rotate(baseMat, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(baseMat, { size.x, size.y, 0.0f });
	}

	glm::mat4 transformMat;
};
