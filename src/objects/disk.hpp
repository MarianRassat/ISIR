#ifndef __RT_ISICG_DISK__
#define __RT_ISICG_DISK__

#include "base_object.hpp"
#include "geometry/disk_geometry.hpp"

namespace RT_ISICG
{
	class Disk : public BaseObject
	{
	  public:
		Disk()		  = delete;
		virtual ~Disk() = default;

		Disk( const std::string & p_name, const Vec3f & p_point, const Vec3f & p_normal, const float p_radius )
			: BaseObject( p_name ), _geometry( p_point, p_normal, p_radius )
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
		DiskGeometry _geometry;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DISK__
