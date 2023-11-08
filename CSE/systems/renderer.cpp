#include <CSE/systems/renderer.h>
#include <CSE/systems/application.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	Scene* Renderer::m_Scene = nullptr;
	Camera2D* Renderer::m_ActiveCamera = nullptr;
	
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
	
	void Renderer::SetActiveCamera(Camera2D* camera)
	{
		m_ActiveCamera = camera;
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
	
	void Renderer::DrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		GeneralDrawTexture(texture, destRect, srcRect, glm::vec2(1.0f), glm::vec4(1.0f));
	}
	
	void Renderer::DrawTiledTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		GeneralDrawTexture(texture, destRect, srcRect, tilingFactor, glm::vec4(1.0f));
	}
	
	void Renderer::GeneralDrawTexture(SDL_Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		SDL_Rect* place = new SDL_Rect;
		SDL_Rect* source = new SDL_Rect;
		
		int windowWidth;
		int windowHeight;
		// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
		SDL_GetWindowSize(m_Scene->GetLayer()->GetWindow()->GetNativeWindow(), &windowWidth, &windowHeight);
		
		SDL_FPoint dXY = {0.0f, 0.0f};
		if (m_ActiveCamera != nullptr)
		{
			dXY.x = m_ActiveCamera->GetPosition().x;
			dXY.y = m_ActiveCamera->GetPosition().y;
		}
		
		float scaleX = m_Scene->GetLayer()->GetWindow()->GetScale().x;
		float scaleY = m_Scene->GetLayer()->GetWindow()->GetScale().y;
		
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
			*place = 
			{ 
				(int)floorf(destRect->x * scaleX - dXY.x * windowWidth), 
				(int)floorf(destRect->y * scaleY - dXY.y * windowHeight), 
				(int)floorf(destRect->w * scaleX), 
				(int)floorf(destRect->h * scaleY) 
			};
		} else {
			// not making it NULL is important for the next step - tiling
			*place = 
			{ 
				(int)floorf(0 - dXY.x * windowWidth), 
				(int)floorf(0 - dXY.y * windowHeight), 
				windowWidth, 
				windowHeight 
			};
		}
		
		// draw only if it's on screen
		if ((((*place).x + (*place).w) > 0) && (((*place).y + (*place).h) > 0) &&
			((*place).x < windowWidth) && ((*place).y < windowHeight))
		{
			// tiling texture across the place rectangle
			// TODO: Adjust tiling to pixel size
			if ((tilingFactor.x != 0.0f) || (tilingFactor.y != 0.0f))
			{
				// now, get subPlaces and RenderCopy there
				int xNum, yNum; // how many whole tiles there are?
				int xMod, yMod; // how big is the partial tile left?
				int tileWidth = (*source).w;
				int tileHeight = (*source).h;
				
				// CSE_CORE_LOG("Region width: ", (*destRect).w, "; height: ", (*destRect).h);
				// CSE_CORE_LOG("Tile width: ", tileWidth, "; height: ", tileHeight);
				// CSE_CORE_LOG("Tiling factor: (", tilingFactor.x, "; ", tilingFactor.y, ")");
				if (tilingFactor.x > 0.0f)
				{
					xNum = (int)floorf((*destRect).w / (floor)(tileWidth * tilingFactor.x)); 
					xMod = (int)floorf((*destRect).w - (xNum * tileWidth * tilingFactor.x));
					if (xMod > 0)
						xNum++;
					// CSE_CORE_LOG("xNum: ", xNum, "; xMod: ", xMod, "");
				} else { // not only prevent division by zero, but also allow no-tiling at all
					xNum = 1;
					xMod = 0;
					tilingFactor.x = (*destRect).w / tileWidth; // prevent multiplying by zero
				}
				
				if (tilingFactor.y > 0.0f)
				{
					yNum = (int)floorf((*destRect).h / (floor)(tileHeight * tilingFactor.y)); // how many whole tiles there are?
					yMod = (int)floorf((*destRect).h - (yNum * tileHeight * tilingFactor.y)); // how big is the partial tile left?
					if (yMod > 0)
						yNum++;
					// CSE_CORE_LOG("yNum: ", yNum, "; yMod: ", yMod, "");
				} else { // not only prevent division by zero, but also allow no-tiling at all
					yNum = 1;
					yMod = 0;
					tilingFactor.y = (*destRect).h / tileHeight; // prevent multiplying by zero
				}
				
				SDL_Rect* newPlace = new SDL_Rect;
				SDL_Rect* newSource = new SDL_Rect;
				*newSource = *source;
				
				for (int x = 0; x < xNum; x++)
				{
					if ((x == (xNum - 1)) && (xMod > 0))
					{
						tileWidth = xMod / tilingFactor.x;
					} else {
						tileWidth = (*source).w;
					}
					
					for (int y = 0; y < yNum; y++)
					{
						if ((y == (yNum - 1)) && (yMod > 0))
						{
							tileHeight = yMod / tilingFactor.y;
						} else {
							tileHeight = (*source).h;
						}
						
						// CSE_LOG("scaleX: ", scaleX, "; scaleY: ", scaleY);
						*newPlace = 
						{
							(int)floorf(scaleX * ((*destRect).x + x * (*source).w * tilingFactor.x) - dXY.x * windowWidth), 
							(int)floorf(scaleY * ((*destRect).y + y * (*source).h * tilingFactor.y) - dXY.y * windowHeight), 
							(int)floorf(scaleX * tileWidth * tilingFactor.x), 
							(int)floorf(scaleY * tileHeight * tilingFactor.y) 
						};
						// CSE_LOG("Tile width: ", tileWidth, "; height: ", tileHeight);
						// CSE_LOG("New place (SDL_Rect): (", (*newPlace).x, "; ", (*newPlace).y, "; ", (*newPlace).w, "; ", (*newPlace).h, ")");
						
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
		}
		
		delete place;
		delete source;
	}
	
	void Renderer::DrawRect(SDL_FPoint center, SDL_FPoint size)
	{
		float scaleX = m_Scene->GetLayer()->GetWindow()->GetPrefs().width * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		float scaleY = m_Scene->GetLayer()->GetWindow()->GetPrefs().height * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		
		SDL_FPoint dXY = {0.0f, 0.0f};
		if (m_ActiveCamera != nullptr)
		{
			dXY = m_ActiveCamera->GetPosition();
		}
		
		SDL_Rect rect = 
		{
			(int)roundf(scaleX * (center.x - dXY.x - size.x/2)), 
			(int)roundf(scaleY * (center.y - dXY.y - size.y/2)), 
			(int)roundf(scaleX * size.x), 
			(int)roundf(scaleY * size.y)
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), 128, 255, 255, 255);
		SDL_RenderDrawRect(GetActiveRenderer(), &rect);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::Update()
	{
		SDL_RenderPresent(GetActiveRenderer());
	}
}
