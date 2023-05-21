#include "whitted_integrator.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		return _Li( p_scene, p_ray, p_tMin, p_tMax, false, 0 );
	}

	Vec3f WhittedIntegrator::_Li( const Scene & p_scene,
								  const Ray &	p_ray,
								  const float	p_tMin,
								  const float	p_tMax,
								  const bool	p_inTransparent,
								  const int		p_nbBounces ) const
	{
		if ( p_nbBounces >= _nbBounces ) { return BLACK; }

		HitRecord hitRecord;

		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isReflective() )
			{
				// Matériau miroir
				Ray reflectedRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflectedRay.offset( hitRecord._normal );

				return _Li( p_scene, reflectedRay, p_tMin, p_tMax, p_inTransparent, p_nbBounces + 1 );
			}

			if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				// Matériau transparent
				Ray reflectedRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflectedRay.offset( hitRecord._normal );
				Vec3f reflectedLi = _Li( p_scene, reflectedRay, p_tMin, p_tMax, p_inTransparent, p_nbBounces + 1 );

				float ior_i = p_inTransparent ? hitRecord._object->getMaterial()->getIOR() : 1.f;
				float ior_t = p_inTransparent ? 1.f : hitRecord._object->getMaterial()->getIOR();

				float costheta_i = glm::dot( -p_ray.getDirection(), hitRecord._normal );
				float sintheta_t = ior_i / ior_t * glm::sqrt( 1.f - costheta_i * costheta_i );

				if ( sintheta_t >= 1.f ) { return reflectedLi; }

				float costheta_t = glm::sqrt( 1.f - sintheta_t * sintheta_t );
				float rs = ( ior_t * costheta_i - ior_i * costheta_t ) / ( ior_t * costheta_i + ior_i * costheta_t );
				float rp = ( ior_i * costheta_i - ior_t * costheta_t ) / ( ior_i * costheta_i + ior_t * costheta_t );
				float r	 = ( rs * rs + rp * rp ) / 2.f;
				Vec3f tangent = glm::cross( p_ray.getDirection(), hitRecord._normal );

				Vec3f refractedDir = glm::rotate( -hitRecord._normal, glm::acos( costheta_t ), tangent );
				Ray	  refractedRay = Ray( hitRecord._point, refractedDir );
				refractedRay.offset( -hitRecord._normal );

				Vec3f refractedLi = _Li( p_scene, refractedRay, p_tMin, p_tMax, !p_inTransparent, p_nbBounces + 1 );
				return r * reflectedLi + ( 1.f - r ) * refractedLi;
			}

			// Matériau ni miroir ni transparent
			Vec3f Li = Vec3f( 0 );

			for ( auto & light : p_scene.getLights() )
			{
				int nbSamples = light->isSurface() ? _nbLightSamples : 1;

				// add each light's contribution if no intersection is found before the light
				for ( int iSample = 0; iSample < nbSamples; iSample++ )
				{
					LightSample lightHit = light->sample( hitRecord._point ); // get light properties wrt the hit point

					Ray litRay = Ray( hitRecord._point, lightHit._direction );
					litRay.offset( hitRecord._normal );

					if ( !p_scene.intersectAny( litRay, 0.f, lightHit._distance - 0.001 ) )
					{
						float cosTheta = glm::max( 0.f, glm::dot( hitRecord._normal, lightHit._direction ) );
						Vec3f shade	   = hitRecord._object->getMaterial()->shade( p_ray, hitRecord, lightHit );
						Li += lightHit._radiance * glm::max( Vec3f( 0 ), shade ) * cosTheta / (float)nbSamples;
					}
				}
			}

			return Li;
		}
		else { return _backgroundColor; }
	}
} // namespace RT_ISICG
