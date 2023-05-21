#include "cube_geometry.hpp"


namespace RT_ISICG
{
	bool CubeGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{	
		
		p_t1 = -1;
		p_t2 = -1;
		for ( int face = 0; face < 3; face++ ) {
			// 0: front 1: up 2: right
			for (int direction = -1; direction <= 1; direction += 2) {
				Vec3f normal = face == 0 ? _front : ( face == 1 ? _up : _right );
				normal *= (float)direction;
				Vec3f point	 = normal * _size + _center;
				// do a plane-ray intersection check
				float divisor = glm::dot( p_ray.getDirection(), normal );

				if ( divisor == 0 ) {
					break;
				}

				// we have an intersection
				float point_t = glm::dot( point - p_ray.getOrigin(), normal ) / divisor;

				// check if in cube bounds
				Vec3f pointCubeSpace = _cubeSpaceTransform * Vec4f( point_t * p_ray.getDirection() + p_ray.getOrigin(), 1. );
				bool withinThresholdX = pointCubeSpace.x <= _size / 1 && pointCubeSpace.x >= -_size / 1;
				bool withinThresholdY = pointCubeSpace.y <= _size / 1 && pointCubeSpace.y >= -_size / 1;
				bool withinThresholdZ = pointCubeSpace.z <= _size / 1 && pointCubeSpace.z >= -_size / 1;
				if ( face == 0 ) // z should be _size
				{
					if ( withinThresholdX && withinThresholdY )
					{
						p_t2 = p_t1 == -1 ? p_t2 : point_t;
						p_t1 = p_t1 == -1 ? point_t : p_t1;
					}
				}
				else if ( face == 1 ) // y
				{
					if ( withinThresholdX && withinThresholdZ )
					{
						p_t2 = p_t1 == -1 ? p_t2 : point_t;
						p_t1 = p_t1 == -1 ? point_t : p_t1;
					}
				}
				else // x
				{
					if ( withinThresholdZ && withinThresholdY )
					{
						p_t2 = p_t1 == -1 ? p_t2 : point_t;
						p_t1 = p_t1 == -1 ? point_t : p_t1;
					}
				}

			}
		}

		return p_t1 != -1;
	}

} // namespace RT_ISICG
