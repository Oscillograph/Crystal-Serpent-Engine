#include <CSE/systems/renderer.h>
#include <CSE/systems/application.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/layer.h>
#include <CSE/systems/window.h>
#include <CSE/systems/renderer/camera2d.h>

namespace CSE
{
	SDL_Renderer* Renderer::m_Renderer = nullptr;
	Scene* Renderer::m_ActiveScene = nullptr;
	Layer* Renderer::m_ActiveLayer = nullptr;
	Window* Renderer::m_ActiveWindow = nullptr;
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
		m_ActiveScene = scene;
		
		int newWidth, newHeight;
		SDL_GetWindowSize(GetActiveWindow()->GetNativeWindow(), &newWidth, &newHeight);
		m_PixelSize.x = (float)newWidth / GetActiveWindow()->GetPrefs().width;
		m_PixelSize.y = (float)newHeight / GetActiveWindow()->GetPrefs().height;
	}
	
	void Renderer::SetActiveLayer(Layer* layer)
	{
		m_ActiveLayer = layer;
	}
	
	void Renderer::SetActiveWindow(Window* window)
	{
		m_ActiveWindow = window;
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
		
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		SDL_Rect viewport;
		viewport = {
			windowScale.x * m_CurrentScreen.x,
			windowScale.y * m_CurrentScreen.y,
			windowScale.x * m_CurrentScreen.z,
			windowScale.y * m_CurrentScreen.w
		};
		// SDL_RenderSetViewport(GetActiveRenderer(), &viewport);
		SDL_RenderSetClipRect(GetActiveRenderer(), &viewport);
	}
	
	void Renderer::SetActiveScreenDefault()
	{ 
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		m_CurrentScreen = {
			0,
			0,
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height,
		};
		
		SDL_Rect viewport;
		viewport = {
			windowScale.x * m_CurrentScreen.x,
			windowScale.y * m_CurrentScreen.y,
			windowScale.x * m_CurrentScreen.z,
			windowScale.y * m_CurrentScreen.w
		};
		// SDL_RenderSetViewport(GetActiveRenderer(), &viewport);
		SDL_RenderSetClipRect(GetActiveRenderer(), &viewport);
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
		SDL_FRect* place = new SDL_FRect;
		SDL_Rect* source = new SDL_Rect;
		
		// TODO: find out why Application::Get()->GetWindows() is not allowed to be accessed from here
		glm::vec2 windowScale = {
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height,
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
				windowScale.x * m_CurrentScreen.x, 
				windowScale.y * m_CurrentScreen.y, 
				windowScale.x * m_CurrentScreen.z, 
				windowScale.y * m_CurrentScreen.w 
			};
		}
		
		// TODO: draw only if it's on screen
		// if(true)
		if ((((*place).x + (*place).w) > (0)) &&
			(((*place).y + (*place).h) > (0)) &&
			((*place).x < (windowScale.x * windowSize.x)) &&
			((*place).y < (windowScale.y * windowSize.y)))
		{
			// tiling texture across the place rectangle
			// TODO: Adjust tiling to pixel size
			if ((tilingFactor.x > 0.0f) || (tilingFactor.y > 0.0f))
			{
				// now, get subPlaces and RenderCopy there
				int xNum, yNum; // how many whole tiles there are?
				xNum = 1;
				yNum = 1;
				int xMod, yMod; // how big is the partial tile left?
				xMod = 0;
				yMod = 0;
				glm::uvec2 wholeTileSize = {(*source).w, (*source).h};
				glm::uvec2 currentTileSize = wholeTileSize;
				
				glm::vec2 regionSize = {
					windowSize.x * (*destRect).w, 
					windowSize.y * (*destRect).h
				};
				
				if (tilingFactor.x > 0.0f) // first of all, we need to know how many times to multiply the image
				{
					// how many whole tiles there are?
					xNum = (int)floorf((float)regionSize.x / (wholeTileSize.x * tilingFactor.x));
					// and we need to know how big the remainder is
					xMod = (int)floorf(regionSize.x - (xNum * wholeTileSize.x * tilingFactor.x));
					xMod = (int)(floorf)((float)xMod / tilingFactor.x);
					if (xMod > 0)
						xNum++;
				} else { // not only prevent division by zero, but also allow no-tiling at all
					xNum = 1;
					xMod = 0;
				}
				
				if (tilingFactor.y > 0.0f) // first of all, we need to know how many times to multiply the image
				{
					// how many whole tiles there are?
					yNum = (int)floorf((float)regionSize.y / (wholeTileSize.y * tilingFactor.y)); 
					// and we need to know how big the remainder is
					yMod = (int)floorf(regionSize.y - (yNum * wholeTileSize.y * tilingFactor.y));
					yMod = (int)(floorf)((float)yMod / tilingFactor.y);
					if (yMod > 0)
						yNum++;
				} else { // not only prevent division by zero, but also allow no-tiling at all
					yNum = 1;
					yMod = 0;
				}
				
				// CSE_LOG("xNum: ", xNum, "; yNum: ", yNum, "; xMod: ", xMod, "; yMod: ", yMod);
				
				SDL_FRect* newPlace = new SDL_FRect;
				SDL_Rect* newSource = new SDL_Rect;
				*newSource = *source;
				
				glm::uvec4 currentTilePlace = {0, 0, 0, 0};
				
				for (int x = 0; x < xNum; x++)
				{
					if ((x == (xNum - 1)) && (xMod > 0))
					{
						currentTileSize.x = xMod; //wholeTileSize.x - xMod;
					} else {
						currentTileSize.x = wholeTileSize.x;
					}
					
					for (int y = 0; y < yNum; y++)
					{
						if ((y == (yNum - 1)) && (yMod > 0))
						{
							currentTileSize.y = yMod; //wholeTileSize.y - yMod;
						} else {
							currentTileSize.y = wholeTileSize.y;
						}
						
						*newSource = 
						{ 
							(*source).x, 
							(*source).y, 
							currentTileSize.x, 
							currentTileSize.y 
						};
						
						if (tilingFactor.x > 0.0f)
						{
							currentTilePlace.x = (windowSize.x * (*destRect).x + (x * wholeTileSize.x * tilingFactor.x)) * m_CurrentScreen.z / windowSize.x;
							currentTilePlace.z = currentTileSize.x * tilingFactor.x * m_CurrentScreen.z / windowSize.x;
						} else {
							currentTilePlace.x = (m_CurrentScreen.z * (*destRect).x);
							currentTilePlace.z = m_CurrentScreen.z * (*destRect).w;
						}
						
						if (tilingFactor.y > 0.0f)
						{
							currentTilePlace.y = (windowSize.y * (*destRect).y + (y * wholeTileSize.y * tilingFactor.y)) * m_CurrentScreen.z / windowSize.x;
							currentTilePlace.w = currentTileSize.y * tilingFactor.y * m_CurrentScreen.w / windowSize.y;
						} else {
							currentTilePlace.y = (m_CurrentScreen.w * (*destRect).y);
							currentTilePlace.w = m_CurrentScreen.w * (*destRect).h;
						}
						
						*newPlace = 
						{
							windowScale.x * (m_CurrentScreen.x + currentTilePlace.x), 
							windowScale.y * (m_CurrentScreen.y + currentTilePlace.y),
							windowScale.x * currentTilePlace.z, 
							windowScale.y * currentTilePlace.w 
						};
						
						// CSE_LOG("Whole tile size: ( ", wholeTileSize.x, "; ", wholeTileSize.y, ") >> x = ", x, "; y = ", y);
						// CSE_LOG("Current tile size: ", currentTileSize.x, "; height: ", currentTileSize.y);
						// CSE_LOG("New place (SDL_Rect): (", (*newPlace).x, "; ", (*newPlace).y, "; ", (*newPlace).w, "; ", (*newPlace).h, ")");
						
						SDL_RenderCopyExF(
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
				SDL_RenderCopyExF(
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
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height
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
			GetActiveWindow()->GetScale().x,
			GetActiveWindow()->GetScale().y
		};
		
		glm::vec2 windowSize = {
			GetActiveWindow()->GetPrefs().width,
			GetActiveWindow()->GetPrefs().height
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
