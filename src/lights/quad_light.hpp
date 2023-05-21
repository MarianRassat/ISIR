#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "defines.hpp"
#include "light_sample.hpp"
#include "base_light.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		QuadLight( const Vec3f &   p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v,
				   const Vec3f & p_color,
				   const float   p_power )
			: BaseLight( p_color, p_power ), _position( p_position ), _u( p_u ), _v( p_v )
		{
			_isSurface	  = true;
			Vec3f crossUV = glm::cross( _u, _v );
			_n			  = glm::normalize( crossUV );
			//float sinTheta = 1.f - glm::pow( glm::dot( _u, _v ), 2.f );
			_area = glm::length( crossUV );
		}
		virtual ~QuadLight() = default;

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  protected:
		Vec3f _position;
		Vec3f _u;
		Vec3f _v;
		Vec3f _n;
		float _area;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_QUAD_LIGHT__
