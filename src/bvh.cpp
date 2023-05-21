#include <algorithm>
#include <glm/gtx/string_cast.hpp>
#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		_buildRec( _root, 0, (unsigned int) _triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		p_hitRecord._distance = std::min( p_tMax, p_hitRecord._distance );
		return _intersectRec( _root, p_ray, p_tMin, p_hitRecord._distance, p_hitRecord );
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId  = p_lastTriangleId;

		// calcul aabb
		for (unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++) {
			p_node->_aabb.extend( (*_triangles)[i].getAABB() );
		}

		// critere d'arret
		if (p_depth < _maxDepth && (p_lastTriangleId - p_firstTriangleId) > _maxTrianglesPerLeaf) {

			// get axe de partition
			int largestAxis = (int)p_node->_aabb.largestAxis();

			// get milieu de l'axe
			float midPartitionLength = p_node->_aabb.centroid()[ largestAxis ];
			
			
			__int64 idPartition = std::partition(
				( *_triangles ).begin() + p_firstTriangleId,
				( *_triangles ).begin() + p_lastTriangleId,

				[ & ]( const TriangleMeshGeometry & a ) { 
					return a.getAABB().centroid()[ largestAxis ] < midPartitionLength;
				} )
				
				- ( *_triangles ).begin();


			// construire left et right
			if (idPartition > p_firstTriangleId) {
				p_node->_left = new BVHNode();
				_buildRec( p_node->_left, p_firstTriangleId, (int)idPartition, p_depth + 1 );
			}

			if ( idPartition < p_lastTriangleId ) {
				p_node->_right = new BVHNode();
				_buildRec( p_node->_right, (int)idPartition, p_lastTriangleId, p_depth + 1 );
			}


		}

	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{

		if ( ! p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; } 
		
		// critere d'arret
		if ( p_node->isLeaf() )
		{
			int   iMin = -1;
			float tMin = std::numeric_limits<float>::infinity();
			float uMin;
			float vMin;

			for (unsigned int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++) {

				float tTemp = std::numeric_limits<float>::infinity();
				float uTemp;
				float vTemp;

				// if there is an intersection
				if ( ( *_triangles )[ i ].intersect( p_ray, tTemp, uTemp, vTemp ) )
				{ 

					// if it's closer than the current tMin and still valid, replace it
					if ( tTemp < tMin && tTemp >= p_tMin && tTemp <= p_tMax ) {

						iMin = i;
						tMin = tTemp;
						uMin = uTemp;
						vMin = vTemp;

					}

				}
			}

			if ( iMin != -1 )
			{

				p_hitRecord._normal	  = ( *_triangles )[ iMin ].getSmoothNormal( uMin, vMin );
				p_hitRecord._point	  = p_ray.pointAtT( tMin );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = tMin;
				
				return true;

			}

			return false;
				
		} 

		bool left = false;
		bool right = false;

		if (p_node->_left != nullptr) {
			left = _intersectRec( p_node->_left, p_ray, p_tMin, p_hitRecord._distance, p_hitRecord );
		}
		if ( p_node->_right != nullptr ) {
			right = _intersectRec( p_node->_right, p_ray, p_tMin, p_hitRecord._distance, p_hitRecord );
		}

		return left || right;

	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{

		if ( ! p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }

		if ( p_node->isLeaf() )
		{
			// check triangles
			for ( unsigned int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			{
				float tTemp;
				float uTemp;
				float vTemp;

				// if there is an intersection
				if ( ( *_triangles )[ i ].intersect( p_ray, tTemp, uTemp, vTemp ) )
				{
					if ( tTemp >= p_tMin && tTemp <= p_tMax ) return true;
				}
			}

			return false;

		}

		bool left  = false;
		bool right = false;

		if ( p_node->_left != nullptr )
		{
			left = _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
		}

		if ( p_node->_right != nullptr )
		{
			right = _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
		}

		return left || right;

	}
} // namespace RT_ISICG
