#include <glm/gtx/string_cast.hpp>
#include "quad_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	LightSample QuadLight::sample(const Vec3f& p_point) const 
	{	
		Vec3f pointOnQuad = _position + _u * getRandomFloat() + _v * getRandomFloat();

		Vec3f pointToLight = pointOnQuad - p_point; 
		Vec3f dir		   = glm::normalize( pointToLight );
		float distance	   = glm::length( pointToLight );
		
		float pdf = distance * distance / _area / glm::dot( _n, dir );

		return LightSample( dir, distance, _color * _power / pdf, pdf );
	
	}

}