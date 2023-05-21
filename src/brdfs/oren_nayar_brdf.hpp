#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"
#include <glm/gtx/polar_coordinates.hpp>

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_sigma ) : _kd( p_kd * INV_PIf ), _sigma( p_sigma ), _sigma2( p_sigma*p_sigma ) {};

		inline Vec3f evaluate(Vec3f p_incidence, Vec3f p_observation, Vec3f p_normal) const {
			
			const float cosThetaO = glm::dot( p_normal, p_observation );
			const float cosThetaI = glm::dot( p_normal, p_incidence );

			const float thetaO = glm::acos( cosThetaO );
			const float thetaI = glm::acos( cosThetaI );

			const float alpha = glm::max( thetaO, thetaI );
			const float beta  = glm::min( thetaO, thetaI );
			const Vec3f woPhi = glm::normalize( p_observation - p_normal * cosThetaO );
			const Vec3f wiPhi = glm::normalize( p_incidence   - p_normal * cosThetaI );

			const float cosPhiIO = glm::dot( woPhi, wiPhi );
			const float gamma	 = glm::max( 0.f, cosPhiIO );

			float A = 1.f - 0.5f  * _sigma2 / ( _sigma2 + 0.33f );
			float B =		0.45f * _sigma2 / ( _sigma2 + 0.09f );

			return _kd * ( A + B * gamma * glm::sin( alpha ) * glm::tan( beta ) );
		}

		inline const Vec3f getKd() const { return _kd / INV_PIf; } // inversion, le constructeur applique INV_PIf

	  private:
		Vec3f _kd = WHITE;
		float _sigma = 0.f;
		float _sigma2 = 0.f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_OREN_NAYAR__
