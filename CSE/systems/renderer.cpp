#include <CSE/systems/renderer.h>

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
	}
	
	uint32_t Renderer::GetPixel(SDL_Surface *surface, int x, int y)
	{
		/*
		  Got this function from here:
		  https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
		 */
		/* - - - Usage example: - - -
		  SDL_Color rgb;
		  Uint32 data = getPixel(gSurface, 200, 200);
		  SDL_GetRGB(data, gSurface->format, &rgb.r, &rgb.g, &rgb.b);
		  - - - - - - - - - - - - - - - */
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;
		
		switch (bpp)
		{
			case 1:
				return *p;
				break;
			case 2:
				return *(uint16_t *)p;
				break;
			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					return p[0] << 16 | p[1] << 8 | p[2];
				} else {
					return p[0] | p[1] << 8 | p[2] << 16;
				}
				break;
			case 4:
				return *(uint32_t *)p;
				break;
			default:
				return 0;       /* shouldn't happen, but avoids warnings */
		}
	}
	
	void Renderer::DrawTexture(SDL_Texture* texture, SDL_FRect* stretchRect, SDL_Rect* srcRect, float scaleX, float scaleY)
	{
		SDL_Rect place = 
		{ 
			(int)round(stretchRect->x * scaleX),
			(int)round(stretchRect->y * scaleY),
			(int)round(stretchRect->w * scaleX),
			(int)round(stretchRect->h * scaleY)
		};
		
		SDL_Rect source;
		
		if (srcRect != NULL)
		{
			source = 
			{ 
				srcRect->x,
				srcRect->y,
				srcRect->w,
				srcRect->h
			};
		} else {
			source = 
			{
				(int)round(stretchRect->x),
				(int)round(stretchRect->y),
				(int)round(stretchRect->w),
				(int)round(stretchRect->h)
			};
		}
		
		SDL_RenderCopy(m_Renderer, texture, &source, &place);
	}
	
	void Renderer::Update()
	{
		SDL_RenderPresent(m_Renderer);
	}
}
