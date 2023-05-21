#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"
#include "glm/gtx/string_cast.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		CookTorranceBRDF( const Vec3f & p_ks, const float p_r )
			: _ks( p_ks ), _r( p_r ), _r4( p_r * p_r * p_r * p_r ) {};

		inline Vec3f evaluate( const Vec3f &p_incidence, const Vec3f &p_observation, const Vec3f &p_normal ) const {

			Vec3f h = glm::normalize( p_incidence + p_observation );
			float hn = glm::dot( h, p_normal );
			float _r4m1 = _r4 - 1;
			float Ddiv	= ( hn * hn * _r4m1 + 1.f );
			float D		= _r4 / ( PIf * Ddiv * Ddiv );

			float nwo	= glm::dot( p_normal, p_observation );
			float nwi	= glm::dot( p_normal, p_incidence );
			float k		= ( _r + 1.f ) * ( _r + 1.f ) / 8.f;
			float G1nwo = nwo / ( nwo * ( 1.f - k ) + k );
			float G1nwi = nwi / ( nwi * ( 1.f - k ) + k );
			float G		= G1nwo * G1nwi;

			float mul = 1 - glm::dot( h, p_observation );
			Vec3f F	  = _ks + ( 1.f - _ks ) * mul * mul * mul * mul * mul;

			return ( D * F * G ) / ( 4 * nwo * nwi );

		}

	  private:
		Vec3f _ks = WHITE;
		float _m  = 0.f;
		float _r  = 0.f;
		float _r4 = 0.f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__
