#pragma once
#include <glm/glm.hpp>

struct ray_t
{
	ray_t() = default;
	ray_t(const glm::vec3& origin, const glm::vec3& direction) :
		m_origin{origin},
		m_direction{direction}
	{}

	glm::vec3 At(float distance) const { return m_origin + (m_direction * distance); }


	glm::vec3 m_origin{ 0 };
	glm::vec3 m_direction{ 0 };
};