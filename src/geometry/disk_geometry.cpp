#include "disk_geometry.hpp"

namespace RT_ISICG
{
	bool DiskGeometry::intersect( const Ray & p_ray, float & p_t ) const
	{
		float divisor = glm::dot( p_ray.getDirection(), _normal );

		if ( divisor <= 0 )
		{
			p_t = -1;
			return false;
		}

		p_t = glm::dot( _point - p_ray.getOrigin(), _normal ) / divisor;
	
		return glm::distance(p_ray.pointAtT(p_t), _point) <= _radius;
	}

} // namespace RT_ISICG
