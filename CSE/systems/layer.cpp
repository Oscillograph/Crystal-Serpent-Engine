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
		OnUpdate(time);
		
		if (m_Scene != nullptr)
		{
			m_Scene->OnUpdate(time); // user-defined update scene function
			if (m_Scene->GetPhysicsProcessor() != nullptr)
				m_Scene->UpdatePhysics(time); // engine-defined physics update mechanic
		}
		return true;
	}
	
	bool Layer::LoadScene(Scene* scene)
	{
		m_Scene = scene;
		m_Scene->SetLayer(this);
		if (!m_Scene->IsInitialized())
		{
			m_Scene->Init();
		}
		m_Scene->Load();
		
		// CSE_CORE_LOG("Layer ", m_Name, " attached.");
		if (m_Scene->GetPhysicsSystem() != PhysicsSystem::None)
		{
			if (m_Viewport == nullptr)
			{
				m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {0, 0, GetWindow()->GetPrefs().width, GetWindow()->GetPrefs().height});
				m_Viewport->SetScene(m_Scene);
			}
		} else {
			m_Viewport = new Viewport(m_Scene->GetActiveCamera(), {80, 60, GetWindow()->GetPrefs().width / 2, GetWindow()->GetPrefs().height / 2});
			m_Viewport->SetScene(m_Scene);
		}
		
		return true;
	}
	
	bool Layer::UnloadScene(Scene* scene)
	{
		scene->SetLayer(nullptr);
		m_Scene = nullptr;
		scene->Unload();
		
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
						GetWindow()->GetPrefs().width * (viewportPlaceNormalized.x + transform.positionNormalized.x), 
						GetWindow()->GetPrefs().height * (viewportPlaceNormalized.y + transform.positionNormalized.y),
						viewportPlace.z * transform.sizeNormalized.x,
						viewportPlace.w * transform.sizeNormalized.y,
					};
					
					// CSE_CORE_LOG("Entity ", e.GetComponent<CSE::NameComponent>().value);
					Renderer::DrawTiledTexture(
						spriteComponent.texture->GetTexture(),
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
											{transform.positionNormalized.x + center.x, transform.positionNormalized.y + center.y}, 
											{transform.size.x, transform.size.y},
											{255, 128, 255, 255}
											);
									}
									break;
								case PhysicsDefines::HitBoxType::Rectangle:
									{
										Renderer::DrawRect(
											{transform.positionNormalized.x + physicsComponent.hitBoxes[i].points[0].x, transform.positionNormalized.y + physicsComponent.hitBoxes[i].points[0].y},
											{transform.positionNormalized.x + physicsComponent.hitBoxes[i].points[1].x, transform.positionNormalized.y + physicsComponent.hitBoxes[i].points[1].y},
											{transform.positionNormalized.x + physicsComponent.hitBoxes[i].points[2].x, transform.positionNormalized.y + physicsComponent.hitBoxes[i].points[2].y},
											{transform.positionNormalized.x + physicsComponent.hitBoxes[i].points[3].x, transform.positionNormalized.y + physicsComponent.hitBoxes[i].points[3].y},
											{255, 128, 255, 255}
											);
									}
									break;
								default:
									Renderer::DrawRect(
										{transform.positionNormalized.x, transform.positionNormalized.y}, 
										{transform.size.x, transform.size.y}, 
										{255, 255, 255, 255}
										);
								}
							}
						} else {
							Renderer::DrawRect(
								{viewportPlaceNormalized.x + transform.positionNormalized.x, viewportPlaceNormalized.y + transform.positionNormalized.y}, 
								{transform.size.x, transform.size.y}
								);
						}
					}
				}
			}
			
			if (Application::IsRenderWireframes())
			{
				Renderer::DrawRect(
					{viewportPlaceNormalized.x + viewportPlaceNormalized.z/2, viewportPlaceNormalized.y + viewportPlaceNormalized.w/2}, 
					{viewportPlaceNormalized.z, viewportPlaceNormalized.w},
					{255, 128, 128, 255}
					);
			}
		}
	}
}


