#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/matte_material.hpp"
#include "materials/mirror_material.hpp"
#include "materials/transparent_material.hpp"
#include "materials/metallic_material.hpp"
#include "materials/emissive_material.hpp"
#include "objects/sphere.hpp"
#include "objects/implicit_sphere.hpp"
#include "objects/implicit_torus.hpp"
#include "objects/implicit_temple.hpp"
#include "objects/cube.hpp"
#include "objects/plane.hpp"
#include "objects/disk.hpp"
#include "objects/triangle_mesh.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"
#include "lights/sphere_physical_light.hpp"
#include "lights/disk_physical_light.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	
	void Scene::init()
	{
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new MirrorMaterial( "Mirror" ) );
		_addMaterial( new TransparentMaterial( "Glass", 1.3f ) );

		_addMaterial(
			new MetallicMaterial( "YellowMetal", Vec3f( 1.f, 0.85f, 0.2f ), Vec3f( 1.f, 0.85f, 0.2f ), 0.3f, 0.8f ) );
		_addMaterial( new MetallicMaterial( "WhiteMetal", WHITE, WHITE, 0.5f, 0.2f ) );
		_addMaterial(
			new MetallicMaterial( "RedMetal", RED * 0.3f + WHITE * 0.7f, RED * 0.3f + WHITE * 0.7f, 0.1f, 0.8f ) );
		_addMaterial( new MetallicMaterial( "GreenSmooth", Vec3f( 0.2f, 0.85f, 0.2f ), WHITE, 0.7f, 0.1f ) );

		_addMaterial( new PlasticMaterial( "PasticGray", GREY * 0.7f, GREY * 0.3f, 64.f ) );
		_addMaterial( new PlasticMaterial( "PasticWhite", WHITE*0.97f * 0.7f, WHITE*0.97f * 0.3f, 64.f ) );

		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( "YellowMatte", YELLOW, 0.6f ) );
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ) );

		_addMaterial( new EmissiveMaterial( "WhiteLight", WHITE, 30.f ) );

		// ================================================================
		// Add objects .
		// ================================================================
		// Spheres .
		loadFileTriangleMesh( "Bunny", "data/Bunny.obj", Vec3f( -1.5, -1, 1.5f ), 1.f );
		//_addObject( new Sphere( "Sphere1", Vec3f( 3, 0, 3 ), 1.5f ) );
		_addObject( new ImplicitTorus( "Torus1", Vec3f( 1, 0, 3 ), 0.4f, 2.f ) );
		_addObject( new Sphere( "Sphere2", Vec3f( 1, 0, 3.5 ), 1.5f ) );
		_addObject( new Disk( "DiskMirror", Vec3f( 2, 2, 6.5 ), glm::normalize( Vec3f( 0, 0, 1 ) ), 3.5f ) );
		_addObject( new Disk( "DiskMirror2", Vec3f( -4.5, 2, 0 ), glm::normalize( Vec3f( -1, 0, 0 ) ), 3.5f ) );
		//_addObject( new Sphere( "Sphere3", Vec3f( -1.5, -1, 1.5f ), 1.5f ) );
		//_addObject( new Sphere( "Sphere3", Vec3f( 1, -1, 1 ), 1.f ) );
		//_addObject( new Sphere( "SphereLight", Vec3f( -2, 0, 0 ), 1.f ) );
		//_addObject( new Sphere( "SphereLight2", Vec3f( -2, 4, 0 ), 1.f ) );
		//_addObject( new Sphere( "SphereLight3", Vec3f( 2, 3, -4 ), 1.f ) );
		_attachMaterialToObject( "YellowMetal", "Torus1" );
		//_attachMaterialToObject( "PasticWhite", "Sphere1" );
		_attachMaterialToObject( "PasticWhite", "Sphere2" );
		_attachMaterialToObject( "Glass", "Bunny_defaultobject" );
		_attachMaterialToObject( "Mirror", "DiskMirror" );
		_attachMaterialToObject( "Mirror", "DiskMirror2" );

		
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "RedMetal", "PlaneGround" );
		
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 7.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "BlueMatte", "PlaneFront" );
		
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "MagentaMatte", "PlaneRight" );

		_addObject( new Plane( "PlaneTop", Vec3f( 0.f, 8.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenMatte", "PlaneTop" );

		_addObject( new Plane( "PlaneLeft", Vec3f( 7.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "YellowMatte", "PlaneLeft" );

		
		// ================================================================
		// Add lights .
		// ================================================================
		//_addLight( new PointLight( Vec3f( 15.f, 20.f, 0.f ), WHITE, 4500.f ) );
		//_addLight( new PointLight( Vec3f( 0, 0, -2 ), WHITE, 60.f ) );
		//_addLight( new QuadLight( Vec3f( 1.f, 5.f,-2.f ), Vec3f(-2.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 2.f ), WHITE, 40.f ) );
		//_addLight( new SpherePhysicalLight( WHITE, this, Vec3f( -2, 4, 0 ), 0.5f, 20.f ) );
		//_addLight( new SpherePhysicalLight( WHITE, this, Vec3f( 5, 2, 2 ), 0.3f, 300.f ) );
		//_addLight( new DiskPhysicalLight( WHITE, this, Vec3f( -3, 2, 0 ), glm::normalize( Vec3f( -1, 1, -1 ) ), 1.5f, 20.f ) );
		_addLight( new DiskPhysicalLight(
			WHITE * 0.9f + YELLOW * 0.2f, this, Vec3f( 0, 7.5f, 0 ), glm::normalize( Vec3f( 0, 1, 0 ) ), 5.f, 7.f ) );
		_addLight( new DiskPhysicalLight(
			WHITE * 0.9f + YELLOW * 0.2f, this, Vec3f( 0, 7.51f, 0 ), glm::normalize( Vec3f( 0, -1, 0 ) ), 5.f, 7.f ) );
	}
	
	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path, const Vec3f p_position, const float p_scale )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x * p_scale + p_position.x,
									mesh->mVertices[ v ].y * p_scale + p_position.y,
									mesh->mVertices[ v ].z * p_scale + p_position.z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			triMesh->generateBVH();
			
			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				_addMaterial( new ColorMaterial( std::string( mtlName.C_Str() ), kd ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::_addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			std::cout << "Material \'" << name << "\' added." << std::endl;
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
