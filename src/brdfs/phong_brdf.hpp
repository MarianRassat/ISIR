#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"
#include "glm/gtx/string_cast.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float p_s )
			: _ks( p_ks ), _s( p_s ) {};

		inline Vec3f evaluate( const Vec3f &p_incidence, const Vec3f &p_observation, const Vec3f &p_normal ) const { 
		
			Vec3f omega_r	 = glm::reflect( -p_incidence, p_normal );

			float cosalpha	 = glm::max(0.f, glm::dot( p_observation, omega_r ) );
			float costheta_i = glm::max(0.f, glm::dot( p_incidence, p_normal  ) );
			
			return _ks / costheta_i * glm::pow(cosalpha, _s);

		}

	  private:
		Vec3f _ks = WHITE;
		float _s = 0.f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
