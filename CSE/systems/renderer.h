#ifndef CSE_RENDERER_H
#define CSE_RENDERER_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>

// bricks of the renderer system
#include <CSE/systems/renderer/texture.h>
#include <CSE/systems/renderer/camera2d.h>

// TODO: Consider developing Render class into a per-scene object instead of a static global

namespace CSE
{
	class Scene;
	
	// TODO: Draw graphic primitives
	// TODO: Draw text messages using raster fonts
	// TODO: Draw text messages using TTF (ex.: FreeType)
	// TODO: Optimization of draw calls using batch rendering
	
	class Renderer
	{
	public:
		static void SetActiveRenderer(SDL_Renderer* renderer);
		static void SetActiveScene(Scene* scene);
		static void SetActiveCamera(Camera2D* camera);
		
		static void SetBackgroundColor(const glm::u8vec4& color);
		static void ClearScreen();
		
		inline static SDL_Renderer* GetActiveRenderer() { return m_Renderer; }
		inline static Camera2D* GetActiveCamera() { return m_ActiveCamera; };
		
		
		// low-level methods (draw pixels, primitives, operate with data)
		static uint32_t GetPixel(SDL_Surface *surface, int x, int y);
		static void DrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect);
		static void DrawTiledTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor);
		static void GeneralDrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor);
		
		static void DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color = {128, 255, 255, 255});
		static void DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color = {128, 255, 255, 255});
		
		// mid-level methods (advanced renderer commands)
		static void NewFrame(); 
		static void ShowFrame(); 
		
		// high-level methods (complex graphics operations)
		
	private:
		static SDL_Renderer* m_Renderer;
		static Scene* m_Scene;
		static Camera2D* m_ActiveCamera;
		
		static glm::vec2 m_PixelSize;
		static glm::uvec4 m_BackgroundColor;
		
		static glm::vec2 m_FrameSize;
		static glm::vec2 m_FrameScale;
		static glm::vec2 m_CameraPosition;
	};
}

#endif
