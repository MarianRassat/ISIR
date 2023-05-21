#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample(const Vec3f& p_point) const 
	{	
		Vec3f pointToLight = _position - p_point; 
		Vec3f dir		   = glm::normalize( pointToLight ); 
		
		float distance = glm::length( pointToLight );
		
		return LightSample( dir, distance, _color * _power / distance / distance, 1.f );
	}

}