#include <CSE/renderer.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	
	void Renderer::SetActiveRenderer(SDL_Renderer* renderer)
	{
		m_Renderer = renderer;
	}
	
	void Renderer::SetBackgroundColor(const glm::u8vec4& color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.w);
	}
	
	void Renderer::ClearScreen()
	{
		SDL_RenderClear(m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}
}
