#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/components.h>

namespace CSE
{
	Camera2D::Camera2D() 
	{
	};
	
	/*
	Camera2D::Camera2D(glm::vec4 targetXYZW, float aspectRatio, float size)
		: m_TargetArea(targetXYZW), m_AspectRatio(aspectRatio), m_Size(size) 
	{
		m_TargetAreaBorder = m_TargetArea;
		m_TargetAreaBorder.x -= 10;
		m_TargetAreaBorder.y -= 10;
		m_TargetAreaBorder.z += 10;
		m_TargetAreaBorder.w += 10;
		
		m_Position = {
			(float)(2*newTargetArea.x + newTargetArea.z)/2,
			(float)(2*newTargetArea.y + newTargetArea.w)/2
		};
	};
	*/
	
	Camera2D::~Camera2D() 
	{
	};
	
	/*
	float& Camera2D::GetAspectRatio()
	{
		return m_AspectRatio;
	};
	
	float& Camera2D::GetSize()
	{
		return m_Size;
	};
	*/
	
	void Camera2D::Lock()
	{
		m_RotationAllowed = false;
		m_TranslationAllowed = false;
	};
	
	void Camera2D::UnLock()
	{
		m_RotationAllowed = true;
		m_TranslationAllowed = true;
	};
	
	bool Camera2D::MoveTo(glm::vec2 newPosition)
	{
		if (m_TranslationAllowed)
		{
			Retarget({
				newPosition.x - (float)m_TargetArea.w/2,
				newPosition.y - (float)m_TargetArea.z/2,
				m_TargetArea.w,
				m_TargetArea.z
			});
			return true;
		}
		return false;
	};
	
	bool Camera2D::MoveBy(glm::vec2 newPosition)
	{
		if (m_TranslationAllowed)
		{
			Retarget({
				m_TargetArea.x + newPosition.x,
				m_TargetArea.y + newPosition.y,
				m_TargetArea.w,
				m_TargetArea.z
			});
			return true;
		}
		return false;
	};
	
	glm::vec2 Camera2D::GetPosition()
	{
		return m_Position;
	};
	
	glm::vec2 Camera2D::GetPositionNormalized()
	{
		glm::vec2 positionNormalized = m_Position;
		positionNormalized.x = positionNormalized.x / m_TargetArea.z;
		positionNormalized.y = positionNormalized.y / m_TargetArea.w;
		
		return positionNormalized;
	}
	
	void Camera2D::Retarget(glm::vec4 newTargetArea)
	{
		m_TargetArea = newTargetArea;
		
		m_TargetAreaBorder = m_TargetArea;
		m_TargetAreaBorder.x -= 10;
		m_TargetAreaBorder.y -= 10;
		m_TargetAreaBorder.z += 10;
		m_TargetAreaBorder.w += 10;
		
		m_Position = {
			(float)(2*newTargetArea.x + newTargetArea.z)/2,
			(float)(2*newTargetArea.y + newTargetArea.w)/2
		};
	};
	
	glm::vec4 Camera2D::GetTargetArea()
	{
		return m_TargetArea;
	};
	
	glm::vec4 Camera2D::GetTargetAreaNormalized()
	{
		glm::vec4 targetAreaNormalized = m_TargetArea;
		targetAreaNormalized.x = targetAreaNormalized.x / m_WorldConstraints.x;
		targetAreaNormalized.y = targetAreaNormalized.y / m_WorldConstraints.y;
		targetAreaNormalized.z = targetAreaNormalized.z / m_WorldConstraints.x;
		targetAreaNormalized.w = targetAreaNormalized.w / m_WorldConstraints.y;
		
		return targetAreaNormalized;
	}
	
	void Camera2D::Zoom(glm::vec2 newZoom)
	{
		m_Zoom = newZoom;
	};
	
	glm::vec2 Camera2D::GetZoom()
	{
		return m_Zoom;
	};
	
	// rotate clockwise by angle in radians
	bool Camera2D::Rotate(float rotation)
	{
		if (m_RotationAllowed)
		{
			m_Rotation = rotation;
			return true;
		}
		return false;
	};
	
	float& Camera2D::GetRotation()
	{
		return m_Rotation;
	};
	
	bool Camera2D::Sees(PhysicsComponent* physicsComponent)
	{
		if ((physicsComponent->position.x >= m_TargetAreaBorder.x) &&
			(physicsComponent->position.y >= m_TargetAreaBorder.y) &&
			(physicsComponent->position.x <= (m_TargetAreaBorder.x + m_TargetAreaBorder.z)) &&
			(physicsComponent->position.y <= (m_TargetAreaBorder.y + m_TargetAreaBorder.w)))
		{
			return true;
		}
		return false;
	};
}
