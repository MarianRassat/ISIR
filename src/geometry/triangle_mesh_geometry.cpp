#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

//#define TEST_CULL

#define EPSILON 0.00001f


namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
		_aabb		= AABB();
		_aabb.extend( _refMesh->_vertices[ p_v0 ] );
		_aabb.extend( _refMesh->_vertices[ p_v1 ] );
		_aabb.extend( _refMesh->_vertices[ p_v2 ] );
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 pVec = glm::cross( d, edge2 );

		float det = glm::dot( edge1, pVec );

		float epsilon = 0.00001f;
		if ( det > -epsilon && det < epsilon ) { return false; }
		float invDet = 1.0f / det;

		glm::vec3 tVec = o - v0;

		p_u = glm::dot( tVec, pVec ) * invDet;
		if ( p_u < 0.0f || p_u > 1.0f ) return false;

		glm::vec3 qVec = glm::cross( tVec, edge1 );

		p_v = glm::dot( d, qVec ) * invDet;
		if ( p_v < 0.0f || p_u + p_v > 1.0f ) return false;

		p_t = glm::dot( edge2, qVec ) * invDet;
		
		return true;
	}

	const Vec3f & TriangleMeshGeometry::getSmoothNormal( float u, float v ) const
	{
		return glm::normalize( ( 1.f - u - v ) * _refMesh->_normals[ _v0 ] + u * _refMesh->_normals[ _v1 ]
			   + v * _refMesh->_normals[ _v2 ] );
	}
} // namespace RT_ISICG
