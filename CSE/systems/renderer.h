#ifndef CSE_RENDERER_H
#define CSE_RENDERER_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>

// bricks of the renderer system
#include <CSE/systems/renderer/texture.h>

namespace CSE
{
	// TODO: Draw graphic primitives
	// TODO: Draw text messages using raster fonts
	// TODO: Draw text messages using TTF (ex.: FreeType)
	// TODO: Optimization of draw calls using batch rendering
	
	class Renderer
	{
	public:
		static void SetActiveRenderer(SDL_Renderer* renderer);
		
		static void SetBackgroundColor(const glm::u8vec4& color);
		static void ClearScreen();
		
		inline static SDL_Renderer* GetActiveRenderer() { return m_Renderer; }
		
		
		// low-level methods (draw pixels, primitives, operate with data)
		static uint32_t GetPixel(SDL_Surface *surface, int x, int y);
		static void DrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY);
		static void DrawTiledTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY, bool tiling);
		static void GeneralDrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY, bool tiling, const glm::vec4& tintColor);
		
		// mid-level methods (advanced renderer commands)
		static void Update();
		
		// high-level methods (complex graphics operations)
		
	private:
		static SDL_Renderer* m_Renderer;
		
	};
}

#endif
