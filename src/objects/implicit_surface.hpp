#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;

		// Evaluate normal by computing gradient at 'p_point'
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{

			float fx = _sdf( p_point + Vec3f( _h, 0, 0 ) ) - _sdf( p_point - Vec3f( _h, 0, 0 ) );
			float fy = _sdf( p_point + Vec3f( 0, _h, 0 ) ) - _sdf( p_point - Vec3f( 0, _h, 0 ) );
			float fz = _sdf( p_point + Vec3f( 0, 0, _h ) ) - _sdf( p_point - Vec3f( 0, 0, _h ) );
			
			fx /= 2.f*_h;
			fy /= 2.f*_h;
			fz /= 2.f*_h;

			return glm::normalize( Vec3f( fx, fy, fz ) );

		}

	  private:
		const float _minDistance = 1e-4f;
		const float _h = 1e-3f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
