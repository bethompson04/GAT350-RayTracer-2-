#pragma once
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <stdlib.h>

inline void seedRandom(unsigned int seed)
{
	srand(seed);
}

inline float random01()
{
	return rand() / (float)RAND_MAX;
}

inline float random(float min, float max)
{
	if (min > max) std::swap(min, max);
	float range = max - min;
	return random01() * (range + min);
}

inline glm::vec3 randomVec3(const glm::vec3& min, const glm::vec3& max)
{
	return glm::vec3(random(min.x, max.x), random(min.y, max.y), random(min.z, max.z));
}

inline glm::vec3 randomInUnitSphere()
{
	glm::vec3 v;
	// generate random vectors between -1 - +1 return vector if length is less than 1
	do
	{
		v = randomVec3({ -1, -1, -1 }, { 1, 1, 1 });
	} while (glm::length2(v) >= 1.0f);

	return v;
}