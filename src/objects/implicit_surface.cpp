#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float curr_t = p_tMin;
		Vec3f point	 = p_ray.pointAtT( curr_t );
		
		float dist = _sdf( point );
		int	  n	   = 0;
		while ( dist > _minDistance && curr_t < p_tMax && n < 200)
		{
			n++;
			curr_t += dist;
			point	   = p_ray.pointAtT( curr_t );
			dist = _sdf( point );
		}
		

		if ( curr_t < p_tMax && curr_t > p_tMin )
		{ 
		
			p_hitRecord._distance = curr_t;
			p_hitRecord._point	  = point;
			p_hitRecord._normal	  = _evaluateNormal( point );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._object = this;
			return true;
		}
		
		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float curr_t = p_tMin;
		Vec3f point	 = p_ray.pointAtT( curr_t );
		float dist	 = _sdf( point );
		int	  n		 = 0;

		while ( dist > _minDistance && dist < p_tMax && n < 200)
		{
			n++;
			curr_t += dist;
			point = p_ray.pointAtT( curr_t );
			dist  = _sdf( point );
		}
		if ( dist < p_tMax && dist > p_tMin )
		{
			return true;
		}
		return false;
	}
} // namespace RT_ISICG
