#include <CSE/systems/renderer.h>
#include <CSE/systems/application.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	Scene* Renderer::m_Scene = nullptr;
	glm::vec2 Renderer::m_PixelSize = glm::vec2(1.0f);
	glm::uvec4 Renderer::m_BackgroundColor = glm::uvec4(1.0f);
	
	void Renderer::SetActiveRenderer(SDL_Renderer* renderer)
	{
		m_Renderer = renderer;
	}
	
	void Renderer::SetActiveScene(Scene* scene)
	{
		m_Scene = scene;
		
		int newWidth, newHeight;
		SDL_GetWindowSize(m_Scene->GetLayer()->GetWindow()->GetNativeWindow(), &newWidth, &newHeight);
		m_PixelSize.x = newWidth / m_Scene->GetLayer()->GetWindow()->GetPrefs().width;
		m_PixelSize.y = newHeight / m_Scene->GetLayer()->GetWindow()->GetPrefs().height;
	}
	
	void Renderer::SetBackgroundColor(const glm::u8vec4& color)
	{
		m_BackgroundColor = color;
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
	
	void Renderer::GeneralDrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, float scaleX, float scaleY, glm::vec2 tilingFactor, const glm::vec4& tintColor)
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
			// if the region is the whole window, we need that window to get the correct *place
			int windowWidth;
			int windowHeight;
			// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
			SDL_GetWindowSize(SDL_RenderGetWindow(GetActiveRenderer()), &windowWidth, &windowHeight);
			
			*place = { 0, 0, windowWidth, windowHeight };
			// not making it NULL is important for the next step - tiling
		}
		
		// TODO: Adjust tiling to pixel size
		// tiling texture across the place rectangle
		if ((tilingFactor.x != 0.0f) || (tilingFactor.y != 0.0f))
		{
			// now, get subPlaces and RenderCopy there
			int xNum, yNum; // how many whole tiles there are?
			int xMod, yMod; // how big is the partial tile left?
			
			if (tilingFactor.x > 0.0f)
			{
				xNum = (int)round((*place).w / ((*source).w * scaleX * tilingFactor.x)); 
				xMod = (int)round((xNum + 1) * (*source).w - (*place).w / scaleX); 
				if (xMod > 0)
					xNum++;
			} else { // not only prevent division by zero, but also allow no-tiling at all
				xNum = 1;
				xMod = 0;
				tilingFactor.x = (*place).w / scaleX; // prevent multiplying by zero
			}
			
			if (tilingFactor.y > 0.0f)
			{
				yNum = (int)round((*place).h / ((*source).h * scaleY * tilingFactor.y)); // how many whole tiles there are?
				yMod = (int)round((yNum + 1) * (*source).h - (*place).h / scaleY); // how big is the partial tile left?
				if (yMod > 0)
					yNum++;
			} else { // not only prevent division by zero, but also allow no-tiling at all
				yNum = 1;
				yMod = 0;
				tilingFactor.y = (*place).h / scaleY; // prevent multiplying by zero
			}
			
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
					
					*newPlace = 
					{
						(*place).x + x * (*source).w * scaleX * tilingFactor.x, 
						(*place).y + y * (*source).h * scaleY * tilingFactor.y, 
						tileWidth * scaleX * tilingFactor.x, 
						tileHeight * scaleY * tilingFactor.y 
					};
					
					*newSource = 
					{ 
						(*source).x, 
						(*source).y, 
						tileWidth, 
						tileHeight 
					};
					
					SDL_RenderCopy(
						GetActiveRenderer(), 
						texture, 
						newSource, 
						newPlace
						);
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
	
	void Renderer::DrawRect(SDL_FPoint center, SDL_FPoint size)
	{
		float scaleX = m_Scene->GetLayer()->GetWindow()->GetPrefs().width * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		float scaleY = m_Scene->GetLayer()->GetWindow()->GetPrefs().height * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		
		SDL_Rect rect = 
		{
			(int)round(scaleX * (center.x - size.x/2)), 
			(int)round(scaleY * (center.y - size.y/2)), 
			(int)round(scaleX * size.x), 
			(int)round(scaleY * size.y)
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(GetActiveRenderer(), &rect);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::Update()
	{
		SDL_RenderPresent(GetActiveRenderer());
	}
}
