#include <CSE/systems/renderer.h>
#include <CSE/systems/application.h>
#include <CSE/systems/scene.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	Scene* Renderer::m_Scene = nullptr;
	Camera2D* Renderer::m_ActiveCamera = nullptr;
	
	glm::uvec4 Renderer::m_CurrentScreen = glm::u8vec4(1.0f);
	glm::vec2 Renderer::m_PixelSize = glm::vec2(1.0f);
	glm::uvec4 Renderer::m_BackgroundColor = glm::uvec4(1.0f);
	glm::vec2 Renderer::m_FrameSize = glm::vec2(1.0f);
	glm::vec2 Renderer::m_FrameScale = glm::vec2(1.0f);
	glm::vec2 Renderer::m_CameraPosition = glm::vec2(1.0f);
	
	void Renderer::SetActiveRenderer(SDL_Renderer* renderer)
	{
		m_Renderer = renderer;
	}
	
	void Renderer::SetActiveScene(Scene* scene)
	{
		m_Scene = scene;
		
		int newWidth, newHeight;
		SDL_GetWindowSize(m_Scene->GetLayer()->GetWindow()->GetNativeWindow(), &newWidth, &newHeight);
		m_PixelSize.x = (float)newWidth / m_Scene->GetLayer()->GetWindow()->GetPrefs().width;
		m_PixelSize.y = (float)newHeight / m_Scene->GetLayer()->GetWindow()->GetPrefs().height;
	}
	
	void Renderer::SetActiveCamera(Camera2D* camera)
	{
		m_ActiveCamera = camera;
		m_CameraPosition = camera->GetPositionNormalized();
		// m_CameraPosition.x = m_CameraPosition.x / m_Scene->GetLayer()->GetWindow()->GetPrefs().width;
		// m_CameraPosition.y = m_CameraPosition.y / m_Scene->GetLayer()->GetWindow()->GetPrefs().height;
	}
	
	void Renderer::SetActiveScreen(const glm::uvec4& screen)
	{ 
		m_CurrentScreen = screen; 
	}
	
	void Renderer::SetActiveScreenDefault()
	{ 
		glm::vec2 windowScale = {
			m_Scene->GetLayer()->GetWindow()->GetScale().x,
			m_Scene->GetLayer()->GetWindow()->GetScale().y
		};
		
		m_CurrentScreen = {
			0,
			0,
			m_Scene->GetLayer()->GetWindow()->GetPrefs().width,
			m_Scene->GetLayer()->GetWindow()->GetPrefs().height,
		}; 
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
	
	uint32_t Renderer::GetPixel(Texture* surface, int x, int y)
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
		int bpp = surface->GetSurface()->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		uint8_t *p = (uint8_t *)surface->GetSurface()->pixels + y * surface->GetSurface()->pitch + x * bpp;
		
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
	
	void Renderer::DrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect)
	{
		GeneralDrawTexture(texture, destRect, srcRect, glm::vec2(1.0f), glm::vec4(1.0f));
	}
	
	void Renderer::DrawTiledTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, const glm::vec2& tilingFactor)
	{
		GeneralDrawTexture(texture, destRect, srcRect, tilingFactor, glm::vec4(1.0f));
	}
	
	void Renderer::GeneralDrawTexture(Texture* texture, SDL_FRect* destRect, SDL_Rect* srcRect, glm::vec2 tilingFactor, const glm::vec4& tintColor)
	{
		SDL_Rect* place = new SDL_Rect;
		SDL_Rect* source = new SDL_Rect;
		
		// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
		glm::vec2 windowScale = {
			m_Scene->GetLayer()->GetWindow()->GetScale().x,
			m_Scene->GetLayer()->GetWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			m_Scene->GetLayer()->GetWindow()->GetPrefs().width,
			m_Scene->GetLayer()->GetWindow()->GetPrefs().height,
		};
		
		// correct the source rectangle
		if (srcRect != NULL)
		{
			*source = { 
				srcRect->x, 
				srcRect->y, 
				srcRect->w, 
				srcRect->h 
			};
		} else {
			source = NULL;
		}
		
		// correct the destination rectangle
		if (destRect != NULL)
		{
			// CSE_CORE_LOG("Viewport (x; y): ", m_CurrentScreen.x, "; ", m_CurrentScreen.y);
			*place = 
			{ 
				(int)floorf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * destRect->x)), 
				(int)floorf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * destRect->y)), 
				(int)floorf(windowScale.x * m_CurrentScreen.z * destRect->w), 
				(int)floorf(windowScale.y * m_CurrentScreen.w * destRect->h) 
			};
		} else {
			// not making it NULL is important for the next step - tiling
			*place = 
			{ 
				0, 
				0, 
				(int)floorf(windowScale.x * m_CurrentScreen.z), 
				(int)floorf(windowScale.x * m_CurrentScreen.w) 
			};
		}
		
		// TODO: draw only if it's on screen
		// if ((((*place).x + (*place).w) > m_CurrentScreen.x) && (((*place).y + (*place).h) > m_CurrentScreen.y) &&
		//	((*place).x < m_CurrentScreen.z) && ((*place).y < m_CurrentScreen.w))
		if (true)
		{
			// tiling texture across the place rectangle
			// TODO: Adjust tiling to pixel size
			// if ((tilingFactor.x != 0.0f) || (tilingFactor.y != 0.0f))
			if (false)
			{
				// now, get subPlaces and RenderCopy there
				int xNum, yNum; // how many whole tiles there are?
				int xMod, yMod; // how big is the partial tile left?
				float tileWidth = (*source).w / m_CurrentScreen.z; // (float)texture->GetWidth();
				float tileHeight = (*source).h / m_CurrentScreen.w; // (float)texture->GetHeight();
				
				// CSE_CORE_LOG("Region width: ", (*destRect).w, "; height: ", (*destRect).h);
				// CSE_CORE_LOG("Tile width: ", tileWidth, "; height: ", tileHeight);
				// CSE_CORE_LOG("Tiling factor: (", tilingFactor.x, "; ", tilingFactor.y, ")");
				if (tilingFactor.x > 0.0f)
				{
					xNum = (int)floorf((*destRect).w / (float)(tileWidth * tilingFactor.x)); 
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
					yNum = (int)floorf((*destRect).h / (float)(tileHeight * tilingFactor.y)); // how many whole tiles there are?
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
							// (int)floorf(scaleX * (*destRect).x + m_CurrentScreen.z * (x * (*source).w * tilingFactor.x)), 
							// (int)floorf(scaleY * (*destRect).y + m_CurrentScreen.w * (y * (*source).h * tilingFactor.y)),
							(int)floorf(windowScale.x * (*destRect).x + m_CurrentScreen.z * (x * tileWidth * tilingFactor.x)), 
							(int)floorf(windowScale.y * (*destRect).y + m_CurrentScreen.w * (y * tileHeight * tilingFactor.y)),
							(int)floorf(windowScale.x * tileWidth * tilingFactor.x), 
							(int)floorf(windowScale.y * tileHeight * tilingFactor.y) 
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
						
						SDL_RenderCopyEx(
							GetActiveRenderer(), 
							texture->GetTexture(), 
							newSource, 
							newPlace,
							0,
							NULL,
							SDL_RendererFlip::SDL_FLIP_NONE
							);
					}
				}
				
				delete newPlace;
				delete newSource;
			} else {
				SDL_RenderCopyEx(
					GetActiveRenderer(), 
					texture->GetTexture(), 
					source, 
					place,
					0,
					NULL,
					SDL_RendererFlip::SDL_FLIP_NONE
					);
			}
		}
		
		delete place;
		delete source;
	}
	
	void Renderer::DrawRect(SDL_FPoint center, SDL_FPoint size, SDL_Color color)
	{
		// float scaleX = m_CurrentScreen.z * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		// float scaleY = m_CurrentScreen.w * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		glm::vec2 windowScale = {
			m_Scene->GetLayer()->GetWindow()->GetScale().x,
			m_Scene->GetLayer()->GetWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			m_Scene->GetLayer()->GetWindow()->GetPrefs().width,
			m_Scene->GetLayer()->GetWindow()->GetPrefs().height
		};
		
		SDL_FRect rect = 
		{
			windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * (center.x - size.x/2)), 
			windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * (center.y - size.y/2)), 
			windowScale.x * m_CurrentScreen.z * size.x, 
			windowScale.y * m_CurrentScreen.w * size.y
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawRectF(GetActiveRenderer(), &rect);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::DrawRect(SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3, SDL_FPoint p4, SDL_Color color)
	{
		// float scaleX = m_CurrentScreen.z * m_Scene->GetLayer()->GetWindow()->GetScale().x;
		// float scaleY = m_CurrentScreen.w * m_Scene->GetLayer()->GetWindow()->GetScale().y;
		glm::vec2 windowScale = {
			m_Scene->GetLayer()->GetWindow()->GetScale().x,
			m_Scene->GetLayer()->GetWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			m_Scene->GetLayer()->GetWindow()->GetPrefs().width,
			m_Scene->GetLayer()->GetWindow()->GetPrefs().height
		};
		
		SDL_Point points[5] = {
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p1.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p1.y)),
			}, 
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p2.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p2.y)),
			},  
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p3.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p3.y)),
			}, 
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p4.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p4.y)),
			},
			{
				(int)roundf(windowScale.x * (m_CurrentScreen.x + m_CurrentScreen.z * p1.x)),
				(int)roundf(windowScale.y * (m_CurrentScreen.y + m_CurrentScreen.w * p1.y)),
			}
		};
		
		SDL_SetRenderDrawColor(GetActiveRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawLines(GetActiveRenderer(), points, 5);
		SetBackgroundColor(m_BackgroundColor);
	}
	
	void Renderer::NewFrame()
	{
		ClearScreen();
		
		if (GetActiveCamera() != nullptr)
		{
			m_CameraPosition = GetActiveCamera()->GetPosition();
		}
	}
	
	void Renderer::ShowFrame()
	{
		SDL_RenderPresent(GetActiveRenderer());
	}
}
