#ifndef CSE_RENDERER_H
#define CSE_RENDERER_H

#include <CSE/core.h>
#include <CSE/platform.h>

namespace CSE
{
	class Renderer
	{
	public:
		static void SetActiveRenderer(SDL_Renderer* renderer);
		
		static void SetBackgroundColor(const glm::u8vec4& color);
		static void ClearScreen();
		
		inline static SDL_Renderer* GetActiveRenderer() { return m_Renderer; }
		
	private:
		static SDL_Renderer* m_Renderer;
	};
}

#endif
