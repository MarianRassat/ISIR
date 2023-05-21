#include "ray_cast_integrator.hpp"

namespace RT_ISICG
{
	Vec3f RayCastIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			float cosTheta = glm::dot( hitRecord._normal, -p_ray.getDirection() );
			return hitRecord._object->getMaterial()->getFlatColor()
				   * glm::max( cosTheta, 0.f );

		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
