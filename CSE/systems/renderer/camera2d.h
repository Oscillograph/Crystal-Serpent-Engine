#ifndef CSE_CAMERA2D_H
#define CSE_CAMERA2D_H

#include <CSE/core.h>

namespace CSE
{
	class Camera2D
	{
	public:
		Camera2D();
		Camera2D(glm::vec2 targetXY, float aspectRatio, float size);
		~Camera2D();
		
		float& GetAspectRatio();
		float& GetSize();
		
		void Lock();
		void UnLock();
		
		bool MoveTo(glm::vec2 newPosition);
		bool MoveBy(glm::vec2 newPosition);
		glm::vec2& GetPosition();
		
		void Retarget(glm::vec2 newTarget);
		glm::vec2& GetTarget();
		
		// rotate clockwise by angle in radians
		bool Rotate(float rotation);
		float& GetRotation();
		
	private:
		glm::vec2 m_Target = {0.0f, 0.0f};
		float m_AspectRatio = 0.0f;
		float m_Size = 0.0f;
		glm::vec2 m_Position = {0.0f, 0.0f};
		float m_Rotation = 0.0f; // in radians
		
		bool m_RotationAllowed = true;
		bool m_TranslationAllowed = true;
	};
}

#endif
