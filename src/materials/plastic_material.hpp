#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name,
						 const Vec3f &		 p_diffuse,
						 const Vec3f &		 p_ks,
						 const float		 p_s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfPhong( p_ks, p_s )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return   _brdfLambert.evaluate()
				   + _brdfPhong.evaluate( p_lightSample._direction, -p_ray.getDirection(), p_hitRecord._normal );
		}

		inline const Vec3f getFlatColor() const override { return _brdfLambert.getKd(); }

	  protected:
		LambertBRDF _brdfLambert;
		PhongBRDF   _brdfPhong  ;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__
