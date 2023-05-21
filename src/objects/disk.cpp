#include "disk.hpp"

namespace RT_ISICG
{
	bool Disk::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float t;
		if ( _geometry.intersect( p_ray, t ) )
		{
			if ( t > p_tMax ) { return false; } // first intersection too far
			if ( t < p_tMin ) { return false; } // not in range

			// Intersection found, fill p_hitRecord.
			p_hitRecord._point	= p_ray.pointAtT( t );
			p_hitRecord._normal = _geometry.computeNormal( p_hitRecord._point );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = t;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool Disk::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t;
		if ( _geometry.intersect( p_ray, t ) )
		{
			if ( t > p_tMax ) { return false; } // first intersection too far
			if ( t < p_tMin ) { return false; } // not in range
			return true;
		}
		return false;
	}

} // namespace RT_ISICG
