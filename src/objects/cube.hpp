#ifndef __RT_ISICG_CUBE__
#define __RT_ISICG_CUBE__

#include "base_object.hpp"
#include "geometry/cube_geometry.hpp"

namespace RT_ISICG
{
	class Cube : public BaseObject
	{
	  public:
		Cube()		  = delete;
		virtual ~Cube() = default;

		Cube( const std::string & p_name,
			  const Vec3f &		  p_center,
			  const float		  p_size,
			  const float		  p_rot_rad_x,
			  const float		  p_rot_rad_y )
			: BaseObject( p_name ), _geometry( p_center, p_size, p_rot_rad_x, p_rot_rad_y )
		{
		}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

				// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		CubeGeometry _geometry;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_CUBE__
