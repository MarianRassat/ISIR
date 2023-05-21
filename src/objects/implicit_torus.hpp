#ifndef __RT_ISICG_IMPLICIT_TORUS__
#define __RT_ISICG_IMPLICIT_TORUS__

#include "base_object.hpp"
#include "implicit_surface.hpp"
#include <glm/gtx/rotate_vector.hpp>
namespace RT_ISICG
{
	class ImplicitTorus : public ImplicitSurface
	{
	  public:
		ImplicitTorus()		   = delete;
		virtual ~ImplicitTorus() = default;

		ImplicitTorus( const std::string & p_name, const Vec3f p_center, const float p_thickness, const float p_radius )
			: ImplicitSurface( p_name ), _center( p_center ), _thickness( p_thickness ), _radius( p_radius ) 
		{
		}

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const override {
			Vec3f p = p_point - _center;
			p		  = glm::rotate( p, PI_2f, Vec3f( 1, 0, 0 ) );
			Vec2f tmp = Vec2f( p.x, p.z );
			Vec2f q	  = Vec2f( glm::length( tmp ) - _radius, p.y );
			return glm::length( q ) - _thickness;
		}


	  private:
		Vec3f _center;
		float _radius;
		float _thickness;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_TORUS__
