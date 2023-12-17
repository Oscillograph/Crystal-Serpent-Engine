#include <CSE/systems/viewport.h>

namespace CSE
{
	Viewport::Viewport()
	{
	}
	
	Viewport::Viewport(Camera2D* camera, glm::vec4 place)
		: m_Camera(camera), m_Place(place)
	{
	}
	
	Viewport::~Viewport()
	{
		m_Camera = nullptr;
	}
}
