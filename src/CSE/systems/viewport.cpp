#include <CSE/systems/viewport.h>
#include <CSE/systems/renderer/camera2d.h>

namespace CSE
{
	Viewport::Viewport()
	{
	}
	
	Viewport::Viewport(Camera2D* camera, glm::vec4 place)
		: m_Camera(camera), m_Place(place)
	{
		if (camera == nullptr)
		{
			m_UseOwnCamera = true;
			camera = new Camera2D();
		}
	}
	
	Viewport::~Viewport()
	{
		if (m_UseOwnCamera)
		{
			CSE_CORE_ASSERT(m_Camera, "Viewport destructor: m_Camera was a null pointer.");
			delete m_Camera;
		}
		m_Camera = nullptr;
	}
}
