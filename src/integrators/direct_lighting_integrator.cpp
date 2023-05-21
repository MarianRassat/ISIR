#include "direct_lighting_integrator.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;

		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{


			Vec3f Li = Vec3f( 0 );

			for ( auto & light : p_scene.getLights() )
			{

				int nbSamples = light->isSurface() ? _nbLightSamples : 1;
				
				// add each light's contribution if no intersection is found before the light
				for ( int i = 0; i < nbSamples; i++ )
				{
					
					LightSample lightHit = light->sample( hitRecord._point ); // get light properties wrt the hit point

					Ray litRay = Ray( hitRecord._point, lightHit._direction );
					litRay.offset( hitRecord._normal );

					if ( !p_scene.intersectAny( litRay, 0.f, lightHit._distance ) )
					{

						float cosTheta = glm::max( 0.f, glm::dot( hitRecord._normal, lightHit._direction ) );

						Li += lightHit._radiance * hitRecord._object->getMaterial()->shade( p_ray, hitRecord, lightHit ) * cosTheta
							  / (float)nbSamples;
						
					}

				}
				
			}
			
			return Li;

		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
