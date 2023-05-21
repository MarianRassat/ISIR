#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "integrators/monte_carlo_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"
#include <thread>

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new WhittedIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::DIRECT_LIGHTING: _integrator = new DirectLightingIntegrator(); break;

		case IntegratorType::WHITTED: _integrator = new WhittedIntegrator(); break;

		case IntegratorType::MONTE_CARLO: _integrator = new MonteCarloIntegrator(); break;

		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene &		p_scene,
								 const BaseCamera * p_camera,
								 Texture &			p_texture,
								 Vec3f *			p_accumulated )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();
		
#pragma omp parallel for collapse(2)
		for ( int j = 0; j < height; j++ ) {
			for ( int i = 0; i < width; i++ )
			{
				_renderPixel(p_scene, p_camera, p_texture, p_accumulated, i, j );
			}

			progressBar.next();

		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}

	void Renderer::_renderPixel( const Scene &			  p_scene,
								 const BaseCamera *		  p_camera,
								 Texture &				  p_texture,
								 Vec3f *				  p_accumulated,
								 int					  p_i,
								 int					  p_j )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		float	  x		 = (float)p_i / ( width-1 );
		float	  y		 = (float)p_j / ( height-1 );
		
		Vec3f sum = Vec3f( 0 );
		/* // Stratified
	 	for ( float k = 0; k < _nbPixelSamples; k++ )
		{
			for ( float l = 0; l < _nbPixelSamples; l++ ) {

				Ray ray = p_camera->generateRay( x + k / width / _nbPixelSamples, y + l / height / _nbPixelSamples );

				sum += _integrator->Li( p_scene, ray, 0, 6000 );
				
			}
		}
		*/

	 	for ( float k = 0; k < _nbPixelSamples; k++ )
		{
			for ( float l = 0; l < _nbPixelSamples; l++ )
			{
				Ray ray = p_camera->generateRay( x + getRandomFloat() / width, y + getRandomFloat() / height );

				sum += _integrator->Li( p_scene, ray, 0, 6000 );
			}
		}

		sum = sum / (float)( _nbPixelSamples * _nbPixelSamples );

		p_accumulated[ p_i + width * p_j ] += sum;

	}
} // namespace RT_ISICG
