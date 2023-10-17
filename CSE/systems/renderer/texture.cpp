#include <CSE/systems/renderer/texture.h>

namespace CSE
{
	Texture::Texture(const std::string& path, SDL_Renderer* r)
	{
		m_Texture = NULL; //The final texture
		m_Surface = NULL;
		m_Renderer = r;
		
		Load(path);
	}
	
	Texture::Texture(const std::string& path, SDL_Renderer* r, const ColorKey& colorKey)
	{
		m_Texture = NULL; //The final texture
		m_Surface = NULL;
		m_Renderer = r;
		
		m_Surface = IMG_Load(path.c_str()); //Load image at specified path
		SDL_SetColorKey(m_Surface, SDL_TRUE, SDL_MapRGB(m_Surface->format, colorKey.r, colorKey.g, colorKey.b));
		m_Texture = SDL_CreateTextureFromSurface(r, m_Surface);
	}
	
	Texture::Texture(	SDL_Renderer* renderer, int w, int h, uint32_t flags, int depth,
						uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask)
	{
		m_Renderer = renderer;
		m_Surface = SDL_CreateRGBSurface(flags, w, h, depth, Rmask, Gmask, Bmask, Amask);
		m_Texture = SDL_CreateTexture(
			m_Renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING,
			// SDL_TEXTUREACCESS_TARGET, 
			w,
			h
			);
	}
	
	Texture::~Texture()
	{
		Free();
	}
	
	void Texture::Load(const std::string& path)
	{
		FreeSurface();
		m_Surface = IMG_Load(path.c_str()); //Load image at specified path  
		
		if(m_Surface == NULL)
		{
			CSE_CORE_LOG("Unable to load image ", path.c_str(), "! SDL_image Error: ", IMG_GetError());
		} else {
			FreeTexture();
			
			//Create texture from surface pixels
			m_Texture = SDL_CreateTextureFromSurface(m_Renderer, m_Surface);
			
			if(m_Texture == NULL)
			{
				CSE_CORE_LOG("Unable to create texture from ", path.c_str(), "! SDL Error: ", SDL_GetError());
			} 
		} 
	}
	
	SDL_Texture* Texture::ConvertToSurface(SDL_Surface* srcSurface)
	{
		Free();
		m_Surface = srcSurface;
		m_Texture = SDL_CreateTextureFromSurface(m_Renderer, srcSurface);
		return m_Texture;
	}
	
	void Texture::Update()
	{
		if (m_Surface != NULL)
		{
			if (m_Texture != NULL)
			{
				SDL_UpdateTexture(m_Texture, NULL, m_Surface->pixels, m_Surface->pitch);
			}
		}
	}
	
	int Texture::GetWidth()
	{
		if (m_Surface != NULL)
		{
			return m_Surface->w;
		}
		return 0;
	}
	
	int Texture::GetHeight()
	{
		if (m_Surface != NULL)
		{
			return m_Surface->h;
		}
		return 0;
	}
	
	// shamelessly took from Lazy Foo's tutorials
	uint32_t* Texture::GetPixels32()
	{  
		if (m_Surface != NULL)
		{ 
			return static_cast<uint32_t*>(m_Surface->pixels);
		}
		return 0;
	}
	
	int Texture::GetPitch32()
	{ 
		if (m_Surface != NULL)
		{ 
			return m_Surface->pitch/4;
		}
		return 0;
	}
	
	void Texture::FreeTexture()
	{
		if (m_Texture != NULL)
		{
			SDL_DestroyTexture(m_Texture);
			m_Texture = NULL;
		}
	}
	
	void Texture::FreeSurface()
	{
		if (m_Surface != NULL)
		{
			SDL_FreeSurface(m_Surface);
			m_Surface = NULL;
		}
	}
	
	void Texture::Free(){
		FreeTexture();
		FreeSurface();
	}
	
	void Texture::Clear()
	{
		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
		SDL_RenderClear(m_Renderer);
	}
}
