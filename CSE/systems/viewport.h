#ifndef CSE_VIEWPORT_H
#define CSE_VIEWPORT_H

#include <CSE/core.h>

namespace CSE
{
	class Camera2D;
	
	
	class Viewport {
	public:
		Viewport();
		Viewport(Camera2D* camera);
		~Viewport();
		
		inline bool HasCamera() { return (m_Camera == nullptr ? false : true); } 
		inline void SetCamera(Camera2D* camera) { m_Camera = camera; }
		inline Camera2D* GetCamera() { return m_Camera; }
	private:
		Camera2D* m_Camera = nullptr;
	};
}

#endif
