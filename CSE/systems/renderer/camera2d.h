#ifndef CSE_CAMERA2D_H
#define CSE_CAMERA2D_H

#include <CSE/core.h>

namespace CSE
{
	struct PhysicsComponent;
	
	class Camera2D
	{
	public:
		Camera2D();
		// Camera2D(glm::vec4 targetXYZW, float aspectRatio, float size);
		~Camera2D();
		
		// float& GetAspectRatio();
		// float& GetSize();
		
		void Lock();
		void UnLock();
		
		bool MoveTo(glm::vec2 newPosition);
		bool MoveBy(glm::vec2 newPosition);
		glm::vec2& GetPosition();
		
		void Retarget(glm::vec4 newTargetArea);
		glm::vec4& GetTargetArea();
		
		void Zoom(glm::vec2 newZoom);
		glm::vec2& GetZoom();
		
		// rotate clockwise by angle in radians
		bool Rotate(float rotation);
		float& GetRotation();
		
		// Scene utilities
		bool Sees(PhysicsComponent* physicsComponent);
		
	private:
		glm::vec4 m_TargetArea = {0.0f, 0.0f, 0.0f, 0.0f}; // physical coordinates
		glm::vec4 m_TargetAreaBorder = {0.0f, 0.0f, 0.0f, 0.0f}; // physical coordinates
		glm::vec2 m_Position = {0.0f, 0.0f}; // the middle of m_TargetArea
		glm::vec2 m_Zoom = {0.0f, 0.0f}; // zoom in both directions without changing target area
		float m_Rotation = 0.0f; // in radians
		
		// TODO: Upgrade to 3D somehow
		// glm::vec2 m_Target = {0.0f, 0.0f};
		// float m_AspectRatio = 0.0f;
		// float m_Size = 0.0f;
		
		// TODO: Control size of a frame through camera interface 
		// glm::vec2 m_FrameSize = {100.0f, 100.0f}; // in meters
		
		bool m_ZoomAllowed = true;
		bool m_RotationAllowed = true;
		bool m_TranslationAllowed = true;
	};
}

#endif
