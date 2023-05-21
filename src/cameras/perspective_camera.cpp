#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		_v = glm::normalize( p_up );
		_w = -glm::normalize( p_lookAt - p_position );
		_u = glm::cross( _v, _w );
		_v = glm::cross( _w, _u );

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport() {

		float viewportHeight = glm::tan( glm::radians( _fovy / 2.f ) ) * _focalDistance * 2;
		float viewportWidth	 = viewportHeight * _aspectRatio;

		_viewportU = _u * viewportWidth;
		_viewportV = _v * viewportHeight;

		_viewportTopLeftCorner = _position + _viewportV / 2.f - _viewportU / 2.f - _w * _focalDistance;

	}

} // namespace RT_ISICG
