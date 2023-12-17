#include <CSE/systems/viewport.h>

namespace CSE
{
	Viewport::Viewport()
	: m_Camera(nullptr)
	{
	}
	
	Viewport::Viewport(Camera2D* camera)
	: m_Camera(camera)
	{
	}
	
	Viewport::~Viewport()
	{
		m_Camera = nullptr;
	}
}
