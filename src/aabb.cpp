#include <glm/gtx/string_cast.hpp>
#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f offset = Vec3f( 0.01f, 0.01f, 0.01f );

		Vec3f tMin	= ( _min - offset - p_ray.getOrigin() ) / p_ray.getDirection();
		Vec3f tMax	= ( _max + offset - p_ray.getOrigin() ) / p_ray.getDirection();
		
		Vec3f t1	= glm::min( tMin, tMax );
		Vec3f t2	= glm::max( tMin, tMax );

		float tNear = glm::max( glm::max( t1.x, t1.y ), t1.z );
		float tFar	= glm::min( glm::min( t2.x, t2.y ), t2.z );

		return tNear < tFar && tFar >= p_tMin && tNear <= p_tMax;

	}
} // namespace RT_ISICG
