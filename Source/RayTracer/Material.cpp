#include "Material.h"
#include "MathUtils.h"
#include "Random.h"

bool Lambertian::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& color, ray_t& scattered) const
{
    glm::vec3 target = raycastHit.point + raycastHit.normal + randomInUnitSphere();
    glm::vec3 direction = normalize(target - raycastHit.point);
    //glm::vec3 direction = normalize(raycastHit.point - target);

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
bool Dielectric::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, glm::vec3& color, ray_t& scattered) const
{
    glm::vec3 reflected = reflect(normalize(ray.m_direction), raycastHit.normal);
    glm::vec3 refracted;

    glm::vec3 outNormal;
    float ni_over_nt;
    float cosine;

    // ray hits inside of surface ( ray -> | -> normal )
    // ray is going out of object (ray direction is facing in the same direction of the surface normal)
    if (dot(ray.m_direction, raycastHit.normal) > 0)
    {
        outNormal = -raycastHit.normal;
        ni_over_nt = m_index;
        cosine = m_index * dot(ray.m_direction, raycastHit.normal) / ray.m_direction.length();
    }
    else
    {
        // ray hits outside of surface ( ray -> | <- normal )
        // ray is going into object (ray direction is facing in the opposite direction of the surface norma)
        outNormal = raycastHit.normal;
        ni_over_nt = 1.0f / m_index;
        cosine = -dot(ray.m_direction, raycastHit.normal) / ray.m_direction.length();
    }

    // set the probability of the ray being reflected or refracted
    float reflectProbability = 1.0f;
    if (refract(ray.m_direction, outNormal, ni_over_nt, refracted))
    {
        // use schlick and cosine (angle) to determine if ray is reflected or refracted
        reflectProbability = schlick(cosine, m_index);
    }

    // create reflected or refracted ray
    scattered = (random01() < reflectProbability) ? ray_t{ raycastHit.point, reflected } : ray_t{ raycastHit.point, refracted };
    color = m_albedo;

    return true;
}

