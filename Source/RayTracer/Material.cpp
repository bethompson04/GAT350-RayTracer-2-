#include "Material.h"
#include "MathUtils.h"
#include "Random.h"

bool Lambertian::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& color, ray_t& scattered) const
{
    glm::vec3 target = raycastHit.point + raycastHit.normal + random01();
    glm::vec3 direction = normalize(target - raycastHit.point);

    scattered = ray_t(raycastHit.point, direction);
    color = m_albedo;

    return true;
}

bool Metal::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, glm::vec3& color, ray_t& scattered) const
{
    glm::vec3 reflected = reflect(normalize(ray.m_direction), raycastHit.normal);

    // set scattered ray from reflected ray + random point in sphere (fuzz = randomness)
    // a mirrir has 0 fuzz, diffused metal has higher fuzz
    scattered = ray_t(raycastHit.point, reflected + random01() * m_fuzz);
    color = m_albedo;

    // Make sure that reflected ray is going away from surface normal (dot product > 0, angle between vectors < 90 degrees)
    return glm::dot(scattered.m_direction, raycastHit.normal) > 0;
}
