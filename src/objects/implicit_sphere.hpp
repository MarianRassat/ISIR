#ifndef __RT_ISICG_IMPLICIT_SPHERE__
#define __RT_ISICG_IMPLICIT_SPHERE__

#include "base_object.hpp"
#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitSphere : public ImplicitSurface
	{
	  public:
		ImplicitSphere()		   = delete;
		virtual ~ImplicitSphere() = default;

		ImplicitSphere( const std::string & p_name, const Vec3f p_center, const float p_radius )
			: ImplicitSurface( p_name ), _center( p_center ), _radius( p_radius ) 
		{
		}

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const override { 
			return glm::length( p_point - _center ) - _radius;
		}


	  private:
		  Vec3f _center;
		  float _radius;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__
