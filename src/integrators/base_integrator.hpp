#ifndef __RT_ISICG_BASE_INTEGRATOR__
#define __RT_ISICG_BASE_INTEGRATOR__

#include "scene.hpp"

namespace RT_ISICG
{
	enum class IntegratorType : int
	{
		RAY_CAST		= 0,
		DIRECT_LIGHTING = 1,
		WHITTED			= 2,
		MONTE_CARLO		= 3,
		COUNT // keep it last
	};

	class BaseIntegrator
	{
	  public:
		BaseIntegrator()		  = default;
		virtual ~BaseIntegrator() = default;

		virtual const IntegratorType getType() const = 0;

		// Return incoming luminance.
		virtual Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const = 0;

		const Vec3f & getBackgroundColor() const { return _backgroundColor; }
		void		  setBackgroundColor( const Vec3f & p_color ) { _backgroundColor = p_color; }

		const Vec3f & randomDirection( const Vec3f & p_normal ) const { 
			Vec3f randomRay = glm::normalize( Vec3f( (float)std::rand() / (float)RAND_MAX - 0.5f,
													 (float)std::rand() / (float)RAND_MAX - 0.5f,
													 (float)std::rand() / (float)RAND_MAX - 0.5f ) );

			return glm::dot( randomRay, p_normal ) > 0.f ? randomRay : -randomRay;	
		}

	  protected:
		Vec3f _backgroundColor = BLACK;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_INTEGRATOR__
