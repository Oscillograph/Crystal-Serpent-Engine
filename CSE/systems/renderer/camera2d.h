#ifndef CSE_CAMERA2D_H
#define CSE_CAMERA2D_H

#include <CSE/core.h>
#include <CSE/systems/platform.h>

namespace CSE
{
	class Camera2D
	{
	public:
		Camera2D();
		Camera2D(SDL_FPoint targetXY, float aspectRatio, float size);
		~Camera2D();
		
		float& GetAspectRatio();
		float& GetSize();
		
		void Lock();
		void UnLock();
		
		bool MoveTo(SDL_FPoint newPosition);
		bool MoveBy(SDL_FPoint newPosition);
		SDL_FPoint& GetPosition();
		
		void Retarget(SDL_FPoint& newTarget);
		SDL_FPoint& GetTarget();
		
		// rotate clockwise by angle in radians
		bool Rotate(float rotation);
		float& GetRotation();
		
	private:
		SDL_FPoint m_Target = {0.0f, 0.0f};
		float m_AspectRatio = 0.0f;
		float m_Size = 0.0f;
		SDL_FPoint m_Position = {0.0f, 0.0f};
		float m_Rotation = 0.0f; // in radians
		
		bool m_RotationAllowed = true;
		bool m_TranslationAllowed = true;
	};
}

#endif
