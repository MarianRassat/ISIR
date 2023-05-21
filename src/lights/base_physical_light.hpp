#ifndef __RT_ISICG_BASE_PHYSICAL_LIGHT__
#define __RT_ISICG_BASE_PHYSICAL_LIGHT__

#include "light_sample.hpp"
#include "objects/base_object.hpp"
#include "scene.hpp"

namespace RT_ISICG
{
	class BasePhysicalLight : public virtual BaseLight
	{
	  public:
		BasePhysicalLight( const Vec3f & p_color, const Scene* p_scene, const float p_power = 1.f )
			: BaseLight( p_color , p_power)
		{
		}
		virtual ~BasePhysicalLight()
		{
			delete _object;
			delete _material;
		};

		inline const Vec3f & getFlatColor() const { return _color; }
		virtual LightSample	 sample( const Vec3f & p_point ) const = 0;
		BaseObject *		 getObject() const { return _object; };
		BaseMaterial *		 getMaterial() const { return _material; };

	  protected:
		BaseObject * _object;
		BaseMaterial * _material;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_PHYSICAL_LIGHT__
