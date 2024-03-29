#ifndef CSE_VIEWPORT_H
#define CSE_VIEWPORT_H

#include <CSE/core.h>

namespace CSE
{
	class Camera2D;
	class Layer;
	class Scene;
	
	
	// A viewport is attached to a layer to give a look into a scene through a camera
	class Viewport {
	public:
		Viewport();
		Viewport(Camera2D* camera, glm::vec4 place);
		~Viewport();
		
		inline bool HasCamera() { return (m_Camera == nullptr ? false : true); } 
		inline void SetCamera(Camera2D* camera) { m_Camera = camera; }
		inline Camera2D* GetCamera() { return m_Camera; }
		
		inline void SetLayer(Layer* layer) { m_Layer = layer; }
		inline Layer* GetLayer() { return m_Layer; }
		
		inline void SetPlace(glm::vec4 place) { m_Place = place; }
		inline glm::vec4 GetPlace() { return m_Place; }
	
		// TODO: allow an entity to have a viewport
		inline void SetScene(Scene* scene) { m_Scene = scene; }
		inline Scene* GetScene() { return m_Scene; }
		// ----------------------------------------
		
	protected:
		Layer* m_Layer = nullptr; // where it is attached to
		Scene* m_Scene = nullptr; // what it is set up to show
		Camera2D* m_Camera = nullptr; // how it is gonna show
		bool m_UseOwnCamera = false; // turn true if the viewport is initialized with its own camera
		
		glm::vec4 m_Place = glm::vec4(0.0f); // where the viewport is located and what size it is
		glm::vec2 m_ScaleToPixel = glm::vec2(0.0f); // how many meters are in one pixel
		glm::vec2 m_ScaleNormalized = glm::vec2(0.0f); // how many meters in one OpenGL coordinate
	};
}

#endif
