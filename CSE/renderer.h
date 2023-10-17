#ifndef CSE_RENDERER_H
#define CSE_RENDERER_H

#include <CSE/core.h>
#include <CSE/platform.h>

// bricks of the renderer system
#include <CSE/systems/renderer/texture.h>

namespace CSE
{
	class Renderer
	{
	public:
		static void SetActiveRenderer(SDL_Renderer* renderer);
		
		static void SetBackgroundColor(const glm::u8vec4& color);
		static void ClearScreen();
		
		inline static SDL_Renderer* GetActiveRenderer() { return m_Renderer; }
		
		
		// low-level methods (draw pixels, primitives, operate with data)
		static uint32_t GetPixel(SDL_Surface *surface, int x, int y);
		static void DrawTexture(SDL_Texture* texture, SDL_FRect* stretchRect, SDL_Rect* srcRect, float scaleX, float scaleY);
		
		// mid-level methods (advanced renderer commands)
		static void Update();
		
		// high-level methods (complex graphics operations)
		
	private:
		static SDL_Renderer* m_Renderer;
	};
}

#endif
