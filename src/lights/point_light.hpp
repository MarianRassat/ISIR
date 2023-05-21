#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "defines.hpp"
#include "light_sample.hpp"
#include "base_light.hpp"

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
	  public:
		PointLight( Vec3f p_position, const Vec3f & p_color, float p_power = 1.f )
			: BaseLight( p_color, p_power ), _position(p_position)
		{
		}
		virtual ~PointLight() = default;

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  protected:
		Vec3f _position;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__
