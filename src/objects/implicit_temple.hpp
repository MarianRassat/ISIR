#ifndef __RT_ISICG_IMPLICIT_TEMPLE__
#define __RT_ISICG_IMPLICIT_TEMPLE__

#include "base_object.hpp"
#include "implicit_surface.hpp"
#include <glm/gtx/rotate_vector.hpp>
namespace RT_ISICG
{
	class ImplicitTemple : public ImplicitSurface
	{
	  public:
		ImplicitTemple()		   = delete;
		virtual ~ImplicitTemple() = default;

		ImplicitTemple( const std::string & p_name, const Vec3f p_center )
			: ImplicitSurface( p_name ), _center( p_center )
		{
		}

	  private:
		// Signed Distance Function
		float _sdf( const Vec3f & p_point ) const override {
			Vec3f p = p_point - _center;
			float scale = 0.04f;
			return temple( p ).x * scale * 0.7f;
		}

		float ndot( Vec2f a, Vec2f b ) const { return a.x * b.x - a.y * b.y; }

		float sdRhombus( Vec2f p, Vec2f b, float r )
		const {
			Vec2f  q = abs( p );
			float h = glm::clamp( ( -2.0f * ndot( q, b ) + ndot( b, b ) ) / dot( b, b ), -1.0f, 1.0f );
			float  d = glm::length( q - 0.5f * b * Vec2f( 1.0f - h, 1.0f + h ) );
			d *= glm::sign( q.x * b.y + q.y * b.x - b.x * b.y );
			return d - r;
		}

		float utemple_sdBox( Vec3f p, Vec3f b ) const { return glm::length( glm::max( abs( p ) - b, 0.0f ) ); }

		float temple_sdBox( Vec3f p, Vec3f b )
		const {
			Vec3f d = abs( p ) - b;
			return glm::min( glm::max( d.x, glm::max( d.y, d.z ) ), 0.0f ) + glm::length( glm::max( d, 0.0f ) );
		}

		float temple_sdBox( float p, float b ) const { return abs( p ) - b; }

		Vec2f opRepLim( Vec2f p, float s, Vec2f lim ) const { return p - s * clamp( round( p / s ), -lim, lim ); }

		Vec2f opRepLim( Vec2f p, float s, Vec2f limmin, Vec2f limmax )
		const {
			return p - s * clamp( round( p / s ), -limmin, limmax );
		}

		float hash1( Vec2f p )
		const {
			p = 50.f * glm::fract( p * 0.3183099f );
			return glm::fract( p.x * p.y * ( p.x + p.y ) );
		}

		Vec3f temple( Vec3f p )
		const {
			Vec3f op	 = p;
			Vec3f res = Vec3f( -1.0f, -1.0f, 0.5f );

			p.y += 2.0f;

			// bounding box
			float bbox = utemple_sdBox( p, Vec3f( 15.0f, 12.0f, 15.0f ) * 1.5f );
			if ( bbox > 5.0f ) return Vec3f( bbox + 1.0f, -1.0f, 0.5f );
			Vec3f q = p;
			Vec2f qxz = Vec2f( q.x, q.z );
			qxz		  = opRepLim( qxz, 4.0f, Vec2f( 4.0f, 2.0f ) );
			q.x		  = qxz.x;
			q.z		  = qxz.y;

			// columns
			Vec2f id = glm::floor( ( Vec2f(p.x, p.z) + 2.0f ) / 4.0f );

			float d = glm::length( qxz ) - 0.9f + 0.05f * p.y;
			d		= glm::max( d, p.y - 6.0f );
			d		= glm::max( d, -p.y - 5.0f );
			d -= 0.05f * pow( 0.5f + 0.5f * sin( glm::atan( q.x, q.z ) * 16.0f ), 2.0f );
			d -= 0.15f * pow( 0.5f + 0.5f * sin( q.y * 3.0f + 0.6f ), 0.12f ) - 0.15f;
			res.z = hash1( id + 11.0f * glm::floor( 0.25f + ( q.y * 3.0f + 0.6f ) / 6.2831f ) );
			d *= 0.85f;

			{
				Vec3f qq = Vec3f( q.x, abs( q.y - 0.3f ) - 5.5f, q.z );
				d		= glm::min( d,
						   temple_sdBox( qq, Vec3f( 1.4f, 0.2f, 1.4f ) + glm::sign( q.y - 0.3f ) * Vec3f( 0.1f, 0.05f, 0.1f ) )
							   - 0.1f ); // base
			}

			d = glm::max( d, -temple_sdBox( p, Vec3f( 14.0f, 10.0f, 6.0f ) ) ); // clip in

			// glm::floor
			float ra = 0.15f * hash1( id + Vec2f( 1.0f, 3.0f ) );
			q		 = p;
			qxz		 = Vec2f( q.x, q.z );
			q.x		 = opRepLim( qxz, 4.0f, Vec2f( 4.0f, 3.0f ) ).x;
			q.z		 = opRepLim( qxz, 4.0f, Vec2f( 4.0f, 3.0f ) ).y;
			float b	 = temple_sdBox( q - Vec3f( 0.0f, -6.0f + 0.1 - ra, 0.0f ), Vec3f( 2.0f, 0.5f, 2.0f ) - 0.15f - ra ) - 0.15f;
			b *= 0.5f;
			if ( b < d )
			{
				d	  = b;
				res.z = hash1( id );
			}
			
			p -= Vec3f( 2.f, 0.f, 2.f );
			id	 = glm::floor( ( Vec2f( p.x, p.z ) + 2.0f ) / 4.0f );
			ra	 = 0.15f * hash1( id + Vec2f( 1.0f, 3.0f ) + 23.1f );
			q	 = p;
			qxz = Vec2f( q.x, q.z );
			q.x		  = opRepLim( qxz, 4.0f, Vec2f( 5.0f, 4.0f ), Vec2f( 5.0f, 3.0f ) ).x;
			q.z		  = opRepLim( qxz, 4.0f, Vec2f( 5.0f, 4.0f ), Vec2f( 5.0f, 3.0f ) ).y;
			b = temple_sdBox( q - Vec3f( 0.0f, -7.0f - ra, 0.0f ), Vec3f( 2.0f, 0.6, 2.0f ) - 0.15f - ra ) - 0.15f;
			b *= 0.8f;
			if ( b < d )
			{
				d	  = b;
				res.z = hash1( id + 13.5f );
			}
			Vec2f( p.x, p.z ) += 2.0f;

			id				  = glm::floor( ( Vec2f( p.x, p.z ) + 2.0f ) / 4.0f );
			ra				  = 0.15f * hash1( id + Vec2f( 1.0f, 3.0f ) + 37.7f );
			q				  = p;
			Vec2f( q.x, q.z ) = opRepLim( Vec2f( q.x, q.z ), 4.0f, Vec2f( 5.0f, 4.0f ) );
			b = temple_sdBox( q - Vec3f( 0.0f, -8.0f - ra - 1.0f, 0.0f ), Vec3f( 2.0f, 0.6 + 1.0f, 2.0f ) - 0.15f - ra )
				- 0.15f;
			b *= 0.5f;
			if ( b < d )
			{
				d	  = b;
				res.z = hash1( id * 7.0f + 31.1f );
			}

			// roof
			q = Vec3f( glm::mod( p.x + 2.0f, 4.0f ) - 2.0f, p.y, glm::mod( p.z + 0.0f, 4.0f ) - 2.0f );
			b = temple_sdBox( q - Vec3f( 0.0f, 7.0f, 0.0f ), Vec3f( 1.95, 1.0f, 1.95 ) - 0.15f ) - 0.15f;
			b = glm::max( b, temple_sdBox( p - Vec3f( 0.0f, 7.0f, 0.0f ), Vec3f( 18.0f, 1.0f, 10.0f ) ) );
			if ( b < d )
			{
				d	  = b;
				res.z = hash1( glm::floor( ( Vec2f( p.x, p.z ) + Vec2f( 2.0f, 0.0f ) ) / 4.0f ) + 31.1f );
			}

			q = Vec3f( glm::mod( p.x + 0.5f, 1.0f ) - 0.5f, p.y, glm::mod( p.z + 0.5f, 1.0f ) - 0.5f );
			b = temple_sdBox( q - Vec3f( 0.0f, 8.0f, 0.0f ), Vec3f( 0.45f, 0.5f, 0.45f ) - 0.02f ) - 0.02f;
			b = glm::max( b, temple_sdBox( p - Vec3f( 0.0f, 8.0f, 0.0f ), Vec3f( 19.0f, 0.2, 11.0f ) ) );
			// q = p+Vec3f(0.0f,0.0f,-0.5f); Vec2f( q.x, q.z ) = opRepLim( Vec2f( q.x, q.z ), 1.0f, Vec2f(19.0f,10.0f)
			// );
			// b = temple_sdBox( q-Vec3f(0.0f,8.0f,0.0f), Vec3f(0.45,0.2,0.45)-0.0f2 )-0.0f2;
			if ( b < d )
			{
				d	  = b;
				res.z = hash1( glm::floor( ( Vec2f( p.x, p.z ) + 0.5f ) / 1.0f ) + 7.8f );
			}

			b = sdRhombus( Vec2f(p.y, p.z) - Vec2f( 8.2f, 0.0f ), Vec2f( 3.0f, 11.0f ), 0.05f );
			q = Vec3f( glm::mod( p.x + 1.0f, 2.0f ) - 1.0f, p.y, glm::mod( p.z + 1.0f, 2.0f ) - 1.0f );
			b = glm::max( b,
					 -temple_sdBox( Vec3f( abs( p.x ) - 20.0f, p.y, q.z ) - Vec3f( 0.0f, 8.0f, 0.0f ), Vec3f( 2.0f, 5.0f, 0.1f ) )
						 - 0.02f );

			b		= glm::max( b, -p.y + 8.2f );
			b		= glm::max( b, utemple_sdBox( p - Vec3f( 0.0f, 8.0f, 0.0f ), Vec3f( 19.0f, 12.0f, 11.0f ) ) );
			float c = sdRhombus( Vec2f( p.y, p.z ) - Vec2f( 8.3f, 0.0f ), Vec2f( 2.25, 8.5f ), 0.05f );
			c		= glm::max( c, temple_sdBox( abs( p.x ) - 19.0f, 2.0f ) );
			b		= glm::max( b, -c );

			d = glm::min( d, b );

			d = glm::max( d, -temple_sdBox( p - Vec3f( 0.0f, 9.5f, 0.0f ), Vec3f( 15.0f, 4.0f, 9.0f ) ) );

			d -= 0.02f * glm::smoothstep( 0.5f, 1.0f, fbm3_high( Vec3f( p.z, p.x, p.y ), 0.4f, 2.96f ) );
			d -= 0.01f * glm::smoothstep( 0.4f, 0.8f, fbm3_high( op * 3.0f, 0.4f, 2.96f ) );
			// d += 0.0f05;

			res = Vec3f( d, 1.0f, res.z );

			return res;
		}

		float fbm3_high( Vec3f p, float a, float f )
		const {
			float ret = 0.f;
			float amp = 1.f;
			float frq = 1.f;
			for ( int i = 0; i < 5; i++ )
			{
				float n = pow( noise_3( p * frq ), 2.f );
				ret += n * amp;
				frq *= f;
				amp *= a * ( pow( n, 0.2f ) );
			}
			return ret;
		}

		float noise_3( Vec3f p )
		const {
			Vec3f i = floor( p );
			Vec3f f = fract( p );
			Vec3f u = f * f * ( 3.0f - 2.0f * f );

			Vec2f ii = Vec2f(i.x, i.y) + i.z * Vec2f( 5.0f );
			float a	 = hash12( ii + Vec2f( 0.0f, 0.0f ) );
			float b	 = hash12( ii + Vec2f( 1.0f, 0.0f ) );
			float c	 = hash12( ii + Vec2f( 0.0f, 1.0f ) );
			float d	 = hash12( ii + Vec2f( 1.0f, 1.0f ) );
			float v1 = glm::mix( glm::mix( a, b, u.x ), glm::mix( c, d, u.x ), u.y );

			ii += Vec2f( 5.0f );
			a		 = hash12( ii + Vec2f( 0.0f, 0.0f ) );
			b		 = hash12( ii + Vec2f( 1.0f, 0.0f ) );
			c		 = hash12( ii + Vec2f( 0.0f, 1.0f ) );
			d		 = hash12( ii + Vec2f( 1.0f, 1.0f ) );
			float v2 = glm::mix( glm::mix( a, b, u.x ), glm::mix( c, d, u.x ), u.y );

			return glm::max( glm::mix( v1, v2, u.z ), 0.0f );
		}

		float hash12( Vec2f p )
		const {
			float h = dot( p, Vec2f( 127.1f, 311.7f ) );
			return glm::fract( sin( h ) * 437.545f );
		}
	  private:
		Vec3f _center;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_TEMPLE__
