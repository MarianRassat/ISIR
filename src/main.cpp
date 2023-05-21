#include "cameras/perspective_camera.hpp"
#include "defines.hpp"
#include "renderer.hpp"

namespace RT_ISICG
{
	int main( int argc, char ** argv )
	{
		const int imgWidth	= 1920;
		//const int imgWidth	= 300 * 4;
		const int imgHeight = 1080;
		//const int imgHeight = 200 * 4;

		const int maxSamples = 2048*16;

		// Create a texture to render the scene.
		Texture img = Texture( imgWidth, imgHeight );

		Vec3f * accumulated_img = new Vec3f[ imgWidth * imgHeight ];
		memset( accumulated_img, 0.f, imgWidth * imgHeight * sizeof(Vec3f) );
		float	totalRenderingTime = 0.f;
		float	renderingTime = 0.f;

		// Create and init scene.
		Scene scene;
		scene.init();

		// Create a perspective camera.
		PerspectiveCamera camera(
			Vec3f( 2, 0, -3 ),
			Vec3f( 0, 0, 1 ),
			Vec3f( 0, 1, 0 ), 75.f, float( imgWidth ) / imgHeight );

		// Create and setup the renderer.
		Renderer renderer;
		renderer.setIntegrator( IntegratorType::MONTE_CARLO );
		renderer.setBackgroundColor( GREY );
		renderer.setNbPixelSamples( 1 );

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;
		for (int i = 1; i < maxSamples + 1; i++) {
			std::cout << "Rendering sample " << i << std::endl;

			renderingTime = renderer.renderImage( scene, &camera, img, accumulated_img );
			totalRenderingTime += renderingTime;

			for ( int x = 0; x < imgWidth; x++ )
			{
				for ( int y = 0; y < imgHeight; y++ ) {
					img.setPixel( x, y, glm::clamp(accumulated_img[ x + y * imgWidth ] / (float)i, 0.f, 1.f) );
				}
			}

			std::cout << accumulated_img[ 0 ].x << std::endl;

			// Save rendered image.
			const std::string imgName = "image.jpg";
			img.saveJPG( RESULTS_PATH + imgName );

			std::cout << "-> Done in " << renderingTime << "s" << std::endl;

		}





		delete[] accumulated_img;

		return EXIT_SUCCESS;
	}

} // namespace RT_ISICG

int main( int argc, char ** argv )
{
	try
	{
		return RT_ISICG::main( argc, argv );
	}
	catch ( const std::exception & e )
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
	}
}
