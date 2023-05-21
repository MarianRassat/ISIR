#ifndef __RT_ISICG_DISK_PHYSICAL_LIGHT__
#define __RT_ISICG_DISK_PHYSICAL_LIGHT__

#include "light_sample.hpp"
#include "objects/disk.hpp"
#include "lights/base_physical_light.hpp"
#include "materials/emissive_material.hpp"
#include "scene.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace RT_ISICG
{
	class DiskPhysicalLight : public virtual BasePhysicalLight
	{
	  public:
		DiskPhysicalLight( const Vec3f & p_color,
						   Scene *		 p_scene,
						   const Vec3f	 p_center,
						   const Vec3f	 p_normal,
						   const float	 p_radius,
						   const float	 p_power = 1.f )
			: BaseLight( p_color, p_power ), BasePhysicalLight( p_color, p_scene, p_power ), _center( p_center ), _normal( p_normal ),
			  _radius( p_radius ), _area( glm::pi<float>() * p_radius * p_radius )
		{
			_isSurface	= true;
			
			int lightID = rand();

			_object	  = new Disk( "Disk" + std::to_string( lightID ), p_center, p_normal, p_radius );
			_material = new EmissiveMaterial( "LightDisk" + std::to_string( lightID ), p_color, p_power );
			p_scene->addObject( _object );
			p_scene->addMaterial( _material );
			p_scene->attachMaterialToObject( _material->getName(), _object->getName() );

			
			_u = glm::rotateX( _normal, glm::radians( 90.f ) );
			_v = glm::normalize( glm::cross( _u, _normal ) );
			_u = glm::normalize( glm::cross( _v, _normal ) ) * p_radius;
			_v *= p_radius;

		}
		virtual ~DiskPhysicalLight() = default;

		inline const Vec3f & getFlatColor() const { return _color*_power; }

		LightSample			 sample( const Vec3f & p_point ) const {

			float u = (float)rand() / RAND_MAX * 2.f - 1.f;
			float v = (float)rand() / RAND_MAX * 2.f - 1.f;

			if ( u == 0 && v == 0 ) { 
				Vec3f pointToLight = _center - p_point;
				Vec3f dir = glm::normalize( pointToLight );
				float distance = glm::length( pointToLight );
				float pdf = distance * distance / _area / glm::dot( _normal, dir );

				return LightSample( dir, distance, _color * _power / pdf, pdf );
			}

			float offset = 0.f;
			if ( glm::abs( u ) > glm::abs( v ) )
			{
				std::swap( u, v );
				offset = PI_2f;
			}
			float theta = offset + PI_4f * ( u / v );
			
			u = v * glm::cos( theta );
			v = v * glm::sin( theta );

			Vec3f point		   = u * _u + v * _v + _center;
			Vec3f pointToLight = point - p_point;
			Vec3f dir		   = glm::normalize( pointToLight );
			float distance	   = glm::length( pointToLight );
			float pdf		   = distance * distance / _area / glm::dot( _normal, dir );

			return LightSample( dir, distance, _color * _power / pdf, pdf );
		}



	  protected:
		Vec3f _center;
		Vec3f _normal;
		Vec3f _u;
		Vec3f _v;
		float _radius;
		float _area;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DISK_PHYSICAL_LIGHT__
