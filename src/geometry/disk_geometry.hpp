#ifndef __RT_ISICG_DISK_GEOMETRY__
#define __RT_ISICG_DISK_GEOMETRY__

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class DiskGeometry : public BaseGeometry
	{
	  public:
		DiskGeometry()		  = delete;
		virtual ~DiskGeometry() = default;

		DiskGeometry( const Vec3f & p_point, const Vec3f & p_normal, const float p_radius )
			: _point( p_point ), _normal( p_normal ), _radius( p_radius )
		{
		}

		inline const Vec3f & getPoint() const { return _point; }

		bool intersect( const Ray & p_ray, float & p_t ) const;

		inline Vec3f computeNormal( const Vec3f & p_point ) const { return _normal; }

	  private:
		Vec3f _point = VEC3F_ZERO;
		Vec3f _normal = VEC3F_ZERO;
		float _radius = 0.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DISK_GEOMETRY__
