#ifndef __RT_ISICG_EMISSIVE_MATERIAL__
#define __RT_ISICG_EMISSIVE_MATERIAL__

#include "base_material.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
	class EmissiveMaterial : public BaseMaterial
	{
	  public:
		EmissiveMaterial( const std::string & p_name, const Vec3f & p_color, const float p_intensity )
			: BaseMaterial( p_name ), _color( p_color ), _intensity( p_intensity )
		{
		}

		virtual ~EmissiveMaterial() = default;

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			return _color*_intensity;
		}
		inline const Vec3f getFlatColor() const override { return _color; }

		inline const bool isEmissive() const override { return true; }

	  protected:
		Vec3f _color = Vec3f( 1.f, 1.f, 1.f );
		float _intensity = 1.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_EMISSIVE_MATERIAL__
