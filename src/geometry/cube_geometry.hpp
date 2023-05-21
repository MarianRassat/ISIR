#ifndef __RT_ISICG_CUBE_GEOMETRY__
#define __RT_ISICG_CUBE_GEOMETRY__

#include "base_geometry.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	class CubeGeometry : public BaseGeometry
	{
	  public:
		CubeGeometry()		  = delete;
		virtual ~CubeGeometry() = default;

		CubeGeometry( const Vec3f & p_center, const float p_size, float p_rot_rad_x, float p_rot_rad_y )
			: _center( p_center ), _size( p_size ), _rotX( p_rot_rad_x ), _rotY( p_rot_rad_y )
		{
			_up = glm::rotateX( _up, _rotX );
			_front = glm::rotateX( _up, glm::radians( 90.f ) );
			_up = glm::rotateY( _up, _rotY );
			_front = glm::rotateY( _front, _rotY );
			_right = glm::cross( _up, _front );

			_up = glm::normalize( _up );
			_front = glm::normalize( _front );
			_right = glm::normalize( _right );

			_cubeSpaceTransform = Mat4f( Vec4f( _right, 0. ), Vec4f( _up, 0. ), Vec4f( _front, 0. ), Vec4f( -_center, 1. ) );

			std::cout << glm::to_string( _up ) << std::endl;
			std::cout << glm::to_string( _right ) << std::endl;
			std::cout << glm::to_string( _front ) << std::endl;
			std::cout << glm::to_string( _cubeSpaceTransform ) << std::endl;

		}

		inline const Vec3f & getCenter() const { return _center; }
		inline const float	 getSize() const { return _size; }

		bool intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const;

		inline Vec3f computeNormal( const Vec3f & p_point ) const { 
			
			// TODO: calculer la normale
			Vec3f point = _cubeSpaceTransform * Vec4f( p_point, 1. );

			if ( glm::abs( point.x ) > glm::abs( point.y ) && glm::abs( point.x ) > glm::abs( point.z ) )
			{
				return glm::sign( point.x ) * _right;
			}
			if ( glm::abs( point.y ) > glm::abs( point.x ) && glm::abs( point.y ) > glm::abs( point.z ) )
			{
				return glm::sign( point.x ) * _up;
			}
			if ( glm::abs( point.z ) > glm::abs( point.y ) && glm::abs( point.z ) > glm::abs( point.x ) )
			{
				return glm::sign( point.x ) * _front;
			}

		}

	  private:
		Vec3f _center = VEC3F_ZERO;
		float _size	  = 1.f;

		float _rotX	  = 0.f;
		float _rotY	  = 0.f;

		Vec3f _up = Vec3f( 0, 1, 0 );
		Vec3f _right = Vec3f( 1, 0, 0 );
		Vec3f _front = Vec3f( 0, 0, 1 );

		Mat4f _cubeSpaceTransform = Mat4f( 1 );
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_CUBE_GEOMETRY__
