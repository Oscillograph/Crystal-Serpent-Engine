#include <CSE/systems/layer.h>
#include <CSE/systems/renderer.h>
#include <CSE/systems/viewport.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
#include <CSE/systems/physics/physicsAPI.h>
#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/window.h>
#include <CSE/systems/application.h>

namespace CSE
{
	// ================= LayerStack =================
	LayerStack::LayerStack()
	{
		
	}
	
	LayerStack::~LayerStack()
	{
		/*
		for (Ref<Layer> layer : m_Layers)
		{
			delete layer;
		}
		*/
	}
	
	bool LayerStack::Attach(Ref<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		if (layer->Attach())
			return true;
		return false;
	}
	
	bool LayerStack::Detach(Ref<Layer> layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
		
		if (layer->Detach())
			return true;
		return false;
	}
	
	// ================= Layer =================
	Layer::Layer(const std::string& name)
	{
		m_Name = name;
		m_Enabled = true;
	}
	
	Layer::~Layer()
	{
		m_Window = nullptr;
		
		if (m_Viewport != nullptr)
			delete m_Viewport;
		m_Viewport = nullptr;
	}
	
	bool Layer::OnAttach()
	{
		return true;
	}
	
	bool Layer::Attach()
	{
		if (OnAttach())
		{
			return true;
		}
		return false;
	}
	
	bool Layer::OnDisplay()
	{
		return true;
	}
	
	bool Layer::Display()
	{
		OnDisplay();
		
		Animate(CSE::Platform::GetTimeMs());
		Draw();
		
		return true;
	}
	
	bool Layer::OnEvent(SDL_Event* event)
	{
		CSE_CORE_LOG("Layer ", m_Name, " event: ", event->type);
		return false;
	}
	
	bool Layer::OnUpdate(TimeType time)
	{
		return true;
	}
	
	bool Layer::Update(TimeType time)
	{
		if (m_Scene != nullptr)
		{
			m_Scene->Update(time); // user-defined update scene function
		}
		return true;
	}
	
	bool Layer::LoadScene(Scene* scene)
	{
		CSE_CORE_LOG("Layer: Load scene...");
		m_Scene = scene;
		m_Scene->SetLayer(this);
		if (!m_Scene->IsInitialized())
		{
			CSE_CORE_LOG("- initialize a scene");
			m_Scene->Init();
		}
		CSE_CORE_LOG("- scene load method call");
		m_Scene->Load();
		
		// CSE_CORE_LOG("Layer ", m_Name, " attached.");
		CSE_CORE_LOG("- layer-level viewport setup");
		if (m_Scene->GetPhysicsSystem() != PhysicsSystem::None)
		{
			if (m_Viewport == nullptr)
			{
				m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {0, 0, GetWindow()->GetPrefs().width, GetWindow()->GetPrefs().height});
				m_Viewport->SetScene(m_Scene);
			} else {
				m_Viewport->SetScene(m_Scene);
			}
		} else {
			if (m_Viewport == nullptr)
			{
				m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {GetWindow()->GetPrefs().width / 4, GetWindow()->GetPrefs().height / 4, GetWindow()->GetPrefs().width / 2, GetWindow()->GetPrefs().height / 2});
				// m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {0, 0, GetWindow()->GetPrefs().width, GetWindow()->GetPrefs().height});
				// m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {80, 60, 160, 120});
				m_Viewport->SetScene(m_Scene);
			} else {
				m_Viewport->SetScene(m_Scene);
			}
		}
		
		return true;
	}
	
	bool Layer::UnloadScene(Scene* scene)
	{
		CSE_CORE_LOG("Layer: Unload a scene...");
		scene->SetLayer(nullptr);
		m_Scene = nullptr;
		CSE_CORE_LOG("- scene unload method call");
		scene->Unload();
		
		CSE_CORE_LOG("- delete a layer-level viewport");
		if (m_Viewport != nullptr)
			delete m_Viewport;
		m_Viewport = nullptr;
		
		return true;
	}
	
	bool Layer::Detach()
	{
		if (OnDetach())
		{
			// CSE_CORE_LOG("Layer ", m_Name, " detached.");
			UnloadScene(m_Scene);
			
			return true;
		}
		
		return false;
	}
	
	bool Layer::OnDetach()
	{
		return true;
	}
	
	void Layer::Begin()
	{
	}
	
	void Layer::OnGuiRender(float time)
	{
	}
	
	void Layer::End()
	{
	}
	
	void Layer::Animate(TimeType sceneTime)
	{
		if (m_Viewport != nullptr)
		{
			m_Viewport->GetScene()->GetRegistry().view<AnimationComponent>().each([=](auto entity, auto& animationComponent){
				auto* frameset = animationComponent.frames[animationComponent.currentAnimation];
				
				if (!animationComponent.paused)
				{
					uint64_t timeNow = sceneTime;
					if ((timeNow - animationComponent.timeBefore) >= frameset->timeBetweenFrames)
					{
						animationComponent.timeBefore = timeNow;
						animationComponent.currentFrame++;
						if (animationComponent.currentFrame == animationComponent.framesTotal){
							if (frameset->loop)
							{
								animationComponent.currentFrame = 0;
							} else {
								animationComponent.currentFrame--;
								animationComponent.paused = true;
							}
						}
					}
				}
			});
		}
	}
	
	void Layer::Draw()
	{
		if (m_Viewport != nullptr)
		{
			Renderer::SetActiveScene(m_Viewport->GetScene());
			Renderer::SetActiveCamera(m_Viewport->GetCamera());
			
			glm::vec2 cameraPositionNormalized = m_Viewport->GetCamera()->GetPositionNormalized();
			
			glm::vec4 viewportPlace = 
			{ 
				m_Viewport->GetPlace().x,
				m_Viewport->GetPlace().y,
				m_Viewport->GetPlace().z, 
				m_Viewport->GetPlace().w 
			};
			
			glm::vec4 viewportPlaceNormalized = 
			{ 
				(float)(m_Viewport->GetPlace().x) / GetWindow()->GetPrefs().width,
				(float)(m_Viewport->GetPlace().y) / GetWindow()->GetPrefs().height,
				(float)(m_Viewport->GetPlace().z) / GetWindow()->GetPrefs().width, 
				(float)(m_Viewport->GetPlace().w) / GetWindow()->GetPrefs().height 
			};
			
			Renderer::SetActiveScreen(viewportPlace);
			
			for (auto entity : GetScene()->GetRegistry().view<SpriteComponent>())
			{
				bool mayDrawPhysicalEntity = false;
				Entity e = Entity(entity, GetScene());
				
				// Draw a physical entity only if the camera "sees" it
				if (e.HasComponent<PhysicsComponent>())
				{
					PhysicsComponent& physicsComponent = e.GetComponent<PhysicsComponent>();
					if (Renderer::GetActiveCamera()->Sees(&physicsComponent))
					{
						mayDrawPhysicalEntity = true;
					}
				}
				
				if (!(e.HasComponent<PhysicsComponent>() && !mayDrawPhysicalEntity))
				{
					TransformComponent& transform = e.GetComponent<TransformComponent>();
					SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
					
					SDL_FRect place; // where to draw
					SDL_Rect frame; // what to draw from a spritesheet
					
					// if a sprite is animated, choose the correct frame
					if (e.HasComponent<AnimationComponent>())
					{
						AnimationComponent& animationComponent = e.GetComponent<AnimationComponent>();
						AnimationFrames* frameset = animationComponent.frames[animationComponent.currentAnimation];
						
						frame = 
						{
							frameset->begin.x + (frameset->width * animationComponent.currentFrame),
							frameset->begin.y,
							std::abs(frameset->width),
							std::abs(frameset->height)
						};
					} else {
						frame = 
						{
							0, 
							0, 
							(spriteComponent.clip.x <= spriteComponent.texture->GetWidth() ? spriteComponent.clip.x : spriteComponent.texture->GetWidth()), 
							(spriteComponent.clip.y <= spriteComponent.texture->GetHeight() ? spriteComponent.clip.y : spriteComponent.texture->GetHeight())
						};
					}
					
					place = 
					{
						(transform.positionNormalized.x - cameraPositionNormalized.x), 
						(transform.positionNormalized.y - cameraPositionNormalized.y),
						transform.sizeNormalized.x,
						transform.sizeNormalized.y,
					};
					
					// CSE_CORE_LOG("Entity ", e.GetComponent<CSE::NameComponent>().value);
					Renderer::DrawTiledTexture(
						spriteComponent.texture,
						&place,
						&frame,
						spriteComponent.tilingFactor
						);
					
					if (Application::IsRenderWireframes())
					{
						if (e.HasComponent<PhysicsComponent>() && mayDrawPhysicalEntity)
						{
							// draw hitboxes
							PhysicsComponent& physicsComponent = e.GetComponent<PhysicsComponent>();
							for (int i = 0; i < physicsComponent.hitBoxes.size(); i++)
							{
								switch (physicsComponent.hitBoxes[i].hitBoxType)
								{
								case PhysicsDefines::HitBoxType::Circle:
									{
										SDL_FPoint center = physicsComponent.hitBoxes[i].points[0];
										Renderer::DrawRect(
											{transform.positionNormalized.x - cameraPositionNormalized.x + center.x + transform.sizeNormalized.x/2, transform.positionNormalized.y - cameraPositionNormalized.y + center.y + transform.sizeNormalized.y/2}, 
											{transform.sizeNormalized.x, transform.sizeNormalized.y},
											{128, 255, 255, 255}
											);
									}
									break;
								case PhysicsDefines::HitBoxType::Rectangle:
									{
										Renderer::DrawRect(
											{transform.positionNormalized.x - cameraPositionNormalized.x + physicsComponent.hitBoxes[i].points[0].x, transform.positionNormalized.y - cameraPositionNormalized.y + physicsComponent.hitBoxes[i].points[0].y},
											{transform.positionNormalized.x - cameraPositionNormalized.x + physicsComponent.hitBoxes[i].points[1].x, transform.positionNormalized.y - cameraPositionNormalized.y + physicsComponent.hitBoxes[i].points[1].y},
											{transform.positionNormalized.x - cameraPositionNormalized.x + physicsComponent.hitBoxes[i].points[2].x, transform.positionNormalized.y - cameraPositionNormalized.y + physicsComponent.hitBoxes[i].points[2].y},
											{transform.positionNormalized.x - cameraPositionNormalized.x + physicsComponent.hitBoxes[i].points[3].x, transform.positionNormalized.y - cameraPositionNormalized.y + physicsComponent.hitBoxes[i].points[3].y},
											{255, 128, 255, 255}
											);
									}
									break;
								default:
									// unknown hitbox
									Renderer::DrawRect(
										{transform.positionNormalized.x - cameraPositionNormalized.x + transform.sizeNormalized.x/2, transform.positionNormalized.y - cameraPositionNormalized.y + transform.sizeNormalized.y/2}, 
										{transform.sizeNormalized.x, transform.sizeNormalized.y}, 
										{255, 255, 255, 255}
										);
								}
							}
						} else {
							// not a hitbox
							Renderer::DrawRect(
								{transform.positionNormalized.x - cameraPositionNormalized.x + transform.sizeNormalized.x/2, transform.positionNormalized.y - cameraPositionNormalized.y + transform.sizeNormalized.y/2}, 
								{transform.sizeNormalized.x, transform.sizeNormalized.y},
								{128, 128, 128, 255}
								);
						}
					}
				}
			}
			
			// Renderer::SetActiveScreen({0, 0, 320, 240});
			Renderer::SetActiveScreenDefault();
			
			if (Application::IsRenderWireframes())
			{
				// viewport
				Renderer::DrawRect(
					{viewportPlaceNormalized.x + viewportPlaceNormalized.z/2, viewportPlaceNormalized.y + viewportPlaceNormalized.w/2}, 
					{viewportPlaceNormalized.z, viewportPlaceNormalized.w},
					{255, 128, 128, 255}
					);
			}
		}
	}
}


