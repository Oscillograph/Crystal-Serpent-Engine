#include <CSE/systems/renderer/camera2d.h>

namespace CSE
{
	Camera2D::Camera2D() 
	{
	};
	
	Camera2D::Camera2D(SDL_FPoint targetXY, float aspectRatio, float size)
		: m_Target(targetXY), m_AspectRatio(aspectRatio), m_Size(size) 
	{
	};
	
	Camera2D::~Camera2D() 
	{
	};
	
	float& Camera2D::GetAspectRatio()
	{
		return m_AspectRatio;
	}
	
	float& Camera2D::GetSize()
	{
		return m_Size;
	}
	
	void Camera2D::Lock()
	{
		m_RotationAllowed = false;
		m_TranslationAllowed = false;
	}
	
	void Camera2D::UnLock()
	{
		m_RotationAllowed = true;
		m_TranslationAllowed = true;
	}
	
	bool Camera2D::MoveTo(SDL_FPoint newPosition)
	{
		if (m_TranslationAllowed)
		{
			m_Position = newPosition;
			return true;
		}
		return false;
	}
	
	bool Camera2D::MoveBy(SDL_FPoint newPosition)
	{
		if (m_TranslationAllowed)
		{
			m_Position.x += newPosition.x;
			m_Position.y += newPosition.y;
			return true;
		}
		return false;
	}
	
	SDL_FPoint& Camera2D::GetPosition()
	{
		return m_Position;
	}
	
	void Camera2D::Retarget(SDL_FPoint& newTarget)
	{
		m_Target = newTarget;
	}
	
	SDL_FPoint& Camera2D::GetTarget()
	{
		return m_Target;
	}
	
	// rotate clockwise by angle in radians
	bool Camera2D::Rotate(float rotation)
	{
		if (m_RotationAllowed)
		{
			m_Rotation = rotation;
			return true;
		}
		return false;
	}
	
	float& Camera2D::GetRotation()
	{
		return m_Rotation;
	}
}
