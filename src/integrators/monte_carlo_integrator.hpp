#ifndef __RT_ISICG_MONTE_CARLO_INTEGRATOR__
#define __RT_ISICG_MONTE_CARLO_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class MonteCarloIntegrator : public BaseIntegrator
	{
	  public:
		MonteCarloIntegrator() : BaseIntegrator() {}
		virtual ~MonteCarloIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::MONTE_CARLO; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene,
				  const Ray &	p_ray,
				  const float	p_tMin,
				  const float	p_tMax ) const override;
		Vec3f _Li( const Scene & p_scene,
				   const Ray &	p_ray,
				   const float	 p_tMin,
				   const float	p_tMax,
				   const bool	 p_inTransparent,
				   const int	 p_nbBounces ) const;
			
	  private:
		int _nbLightSamples = 1;
		int _nbBRDFSamples = 2;
		int _nbBounces = 10;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MONTE_CARLO_INTEGRATOR__
