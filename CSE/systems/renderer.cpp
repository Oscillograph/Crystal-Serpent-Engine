#include <CSE/systems/renderer.h>
#include <CSE/systems/application.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	
	void Renderer::SetActiveRenderer(SDL_Renderer* renderer)
	{
		m_Renderer = renderer;
	}
	
	void Renderer::SetBackgroundColor(const glm::u8vec4& color)
	{
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.x, color.y, color.z, color.w);
	}
	
	void Renderer::ClearScreen()
	{
		SDL_RenderClear(GetActiveRenderer());
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
	
	void Renderer::DrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY)
	{
		GeneralDrawTexture(texture, destRect, srcRect, scaleX, scaleY, glm::vec2(1.0f), glm::vec4(1.0f));
	}
	
	void Renderer::DrawTiledTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY, const glm::vec2& tilingFactor)
	{
		GeneralDrawTexture(texture, destRect, srcRect, scaleX, scaleY, tilingFactor, glm::vec4(1.0f));
	}
	
	void Renderer::GeneralDrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
	{
		SDL_Rect* place = new SDL_Rect;
		SDL_Rect* source = new SDL_Rect;
		
		// correct the source rectangle
		if (srcRect != NULL)
		{
			*source = { srcRect->x, srcRect->y, srcRect->w, srcRect->h };
		} else {
			source = NULL;
		}
		
		// correct the destination rectangle
		if (destRect != NULL)
		{
			*place = { (int)round(destRect->x * scaleX), (int)round(destRect->y * scaleY), (int)round(destRect->w * scaleX), (int)round(destRect->h * scaleY) };
		} else {
			place = NULL;
		}
		
		// tiling texture across the place rectangle
		if ((tilingFactor.x != 1.0f) && (tilingFactor.y != 1.0f))
		{
			// if the region is the whole window, we need that window to get the correct *place
			int windowWidth;
			int windowHeight;
			// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
			SDL_GetWindowSize(SDL_RenderGetWindow(GetActiveRenderer()), &windowWidth, &windowHeight);
			
			// correct the destination rectangle
			*place = { 0, 0, windowWidth, windowHeight };
			
			// now, get subPlaces and RenderCopy there
			int xNum = (int)round((*place).w / ((*source).w * scaleX * tilingFactor.x));
			int xMod = (int)round((*place).w / scaleX - xNum * (*source).w); // get the non-scaled remainder in X coordinate
			if (xMod > 0)
				xNum++;
			
			int yNum = (int)round((*place).h / ((*source).h * scaleY * tilingFactor.y));
			int yMod = (int)round((*place).h / scaleY - yNum * (*source).h); // get the non-scaled remainder in Y coordinate
			if (yMod > 0)
				yNum++;
			
			SDL_Rect* newPlace = new SDL_Rect;
			SDL_Rect* newSource = new SDL_Rect;
			*newSource = *source;
			
			int tileWidth = 0;
			int tileHeight = 0;
			
			for (int x = 0; x < xNum; x++)
			{
				if ((x == (xNum - 1)) && (xMod > 0))
				{
					tileWidth = xMod;
				} else {
					tileWidth = (*source).w;
				}
				
				for (int y = 0; y < yNum; y++)
				{
					if ((y == (yNum - 1)) && (yMod > 0))
					{
						tileHeight = yMod;
					} else {
						tileHeight = (*source).h;
					}
					
					*newPlace = { x * source->w * scaleX * tilingFactor.x, y * source->h * scaleY * tilingFactor.x, tileWidth * scaleX * tilingFactor.x, tileHeight * scaleY * tilingFactor.y };
					*newSource = { source->x, source->y, tileWidth, tileHeight };
					
					SDL_RenderCopy(GetActiveRenderer(), texture, newSource, newPlace);
				}
			}
			
			delete newPlace;
			delete newSource;
		} else {
			SDL_RenderCopy(GetActiveRenderer(), texture, source, place);
		}
		
		delete place;
		delete source;
	}
	
	void Renderer::Update()
	{
		SDL_RenderPresent(GetActiveRenderer());
	}
}
