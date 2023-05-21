#ifndef __RT_ISICG_METALLIC_MATERIAL__
#define __RT_ISICG_METALLIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/cook_torrance_brdf.hpp"
#include "brdfs/oren_nayar_brdf.hpp"

namespace RT_ISICG
{
	class MetallicMaterial : public BaseMaterial
	{
	  public:
		MetallicMaterial( const std::string & p_name,
						 const Vec3f &		 p_kd,
						 const Vec3f &		 p_ks,
						 const float		 p_r,
						 const float		 p_metalness )
			: BaseMaterial( p_name ), _metalness(p_metalness), _brdfOrenNayar( p_kd, p_r ), _brdfCookTorrance( p_ks, p_r )
		{
		}

		virtual ~MetallicMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1.f - _metalness ) * _brdfOrenNayar.evaluate( p_lightSample._direction, -p_ray.getDirection(), p_hitRecord._normal )
				   + _metalness * _brdfCookTorrance.evaluate( p_lightSample._direction, -p_ray.getDirection(), p_hitRecord._normal );
		}

		inline const Vec3f getFlatColor() const override { return _brdfOrenNayar.getKd(); }

	  protected:
		float			 _metalness;
		OrenNayarBRDF	 _brdfOrenNayar;
		CookTorranceBRDF _brdfCookTorrance;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_METALLIC_MATERIAL__
