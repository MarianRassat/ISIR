#ifndef __RT_ISICG_SPHERE_PHYSICAL_LIGHT__
#define __RT_ISICG_SPHERE_PHYSICAL_LIGHT__

#include "light_sample.hpp"
#include "objects/sphere.hpp"
#include "lights/base_physical_light.hpp"
#include "materials/emissive_material.hpp"
#include "scene.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace RT_ISICG
{
	class SpherePhysicalLight : public virtual BasePhysicalLight
	{
	  public:
		SpherePhysicalLight( const Vec3f & p_color, Scene* p_scene, const Vec3f p_center, const float p_radius, const float p_power = 1.f )
			: BaseLight( p_color, p_power ), BasePhysicalLight( p_color, p_scene, p_power ), _center( p_center ), _radius( p_radius )
		{
			_isSurface	= true;
			
			int lightID = rand();

			_object	  = new Sphere( "Sphere" + std::to_string( lightID ), p_center, p_radius );
			_material = new EmissiveMaterial( "LightSphere" + std::to_string( lightID ), p_color, p_power );
			p_scene->addObject( _object );
			p_scene->addMaterial( _material );
			p_scene->attachMaterialToObject( _material->getName(), _object->getName() );

		}
		virtual ~SpherePhysicalLight() = default;

		inline const Vec3f & getFlatColor() const { return _color*_power; }

		LightSample			 sample( const Vec3f & p_point ) const {
			
			Vec3f pointToLight	  = _center - p_point;
			Vec3f pointToLightDir = glm::normalize( pointToLight );
			float distance		  = glm::length( pointToLight );
			if ( distance < _radius ) // p inside the sphere
			{ 
				LightSample sample = LightSample( pointToLightDir, distance, _color / ( distance * distance ), 1.f );
				return sample;
			}

			Vec3f wcX = glm::rotateX( pointToLightDir, glm::radians( 90.f ) );
			Vec3f wcY = glm::normalize( glm::cross( wcX, pointToLightDir ) );
			wcX = glm::normalize( glm::cross( wcY, pointToLightDir ) );

			float u			   = (float)rand() / RAND_MAX;
			float v			   = (float)rand() / RAND_MAX;

			float cosThetaMax  = glm::sqrt( glm::max( 0.f, 
				1.f - (_radius / distance)*(_radius / distance)
			) );

			float pdf = 1.f / ( 2.f * glm::pi<float>() * ( 1.f - cosThetaMax ) );

			float cosTheta = (1.f - u) + u*cosThetaMax;
			float sinTheta = glm::sqrt( glm::max( 0.f, 1.f - cosTheta * cosTheta ) );
			float phi	   = v * 2.f * glm::pi<float>();

			float ds = distance * cosTheta - glm::sqrt( glm::max( 0.f, _radius * _radius - distance * distance * sinTheta * sinTheta ) );
			float cosAlpha = ( distance * distance + _radius * _radius - ds * ds ) / ( 2.f * distance * _radius );
			float sinAlpha = glm::sqrt( glm::max( 0.f, 1.f - cosAlpha * cosAlpha ) );

			Vec3f nObj = sinAlpha * std::cos( phi ) * ( -wcX ) + sinAlpha * std::sin( phi ) * ( -wcY ) + cosAlpha * ( -pointToLightDir );
			Vec3f ptOnSphere = nObj * _radius + pointToLight; //  + p_point for global pos
			
			LightSample sample = LightSample( glm::normalize( ptOnSphere ), ds, _color * _power / pdf, pdf );

			return sample;

		}

	  protected:
		Vec3f	   _center;
		float	   _radius;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE_PHYSICAL_LIGHT__
