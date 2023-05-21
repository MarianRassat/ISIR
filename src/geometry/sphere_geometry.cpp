#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{	
		Vec3f cToO	= p_ray.getOrigin() - _center;
		float t_cProjD = glm::dot( p_ray.getDirection(), cToO );
		float t2_cProjDToSphere = glm::pow( t_cProjD, 2.f )
								- glm::dot( cToO, cToO )
								+ _radius * _radius;

		if ( t2_cProjDToSphere < 0 )
		{
			p_t1 = -1;
			p_t2 = -1;
			return false;
		}

		p_t1 = - t_cProjD - glm::sqrt( t2_cProjDToSphere );
		p_t2 = - t_cProjD + glm::sqrt( t2_cProjDToSphere );

		return true;
	}

} // namespace RT_ISICG
