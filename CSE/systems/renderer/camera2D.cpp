#include <CSE/systems/renderer/camera2d.h>

namespace CSE
{
	Camera2D::Camera2D() 
	{
	};
	
	Camera2D::Camera2D(glm::vec2 targetXY, float aspectRatio, float size)
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
	
	bool Camera2D::MoveTo(glm::vec2 newPosition)
	{
		if (m_TranslationAllowed)
		{
			m_Position = newPosition;
			return true;
		}
		return false;
	}
	
	bool Camera2D::MoveBy(glm::vec2 newPosition)
	{
		if (m_TranslationAllowed)
		{
			m_Position.x += newPosition.x;
			m_Position.y += newPosition.y;
			return true;
		}
		return false;
	}
	
	glm::vec2& Camera2D::GetPosition()
	{
		return m_Position;
	}
	
	void Camera2D::Retarget(glm::vec4 newTargetArea)
	{
		m_TargetArea = newTargetArea;
		m_Position = {
			(float)(2*newTargetArea.x + newTargetArea.z)/2,
			(float)(2*newTargetArea.y + newTargetArea.w)/2
		};
	}
	
	glm::vec4& Camera2D::GetTarget()
	{
		return m_Target;
	}
	
	void Camera2D::Zoom(glm::vec2 newZoom)
	{
		m_Zoom = newZoom;
	}
	
	glm::vec2& Camera2D::GetZoom()
	{
		return m_Zoom;
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
