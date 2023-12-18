#include <CSE/systems/layer.h>
#include <CSE/systems/renderer.h>
#include <CSE/systems/viewport.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
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
		if (layer->OnAttach())
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
		
		if (layer->OnDetach())
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
	}
	
	bool Layer::OnAttach()
	{
		return true;
	}
	
	bool Layer::Attach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " attached.");
		return true;
	}
	
	bool Layer::OnDisplay()
	{
		return true;
	}
	
	bool Layer::Display()
	{
		Renderer::SetActiveCamera(m_Scene->GetActiveCamera());
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
			m_Scene->OnInitialized();
		}
		m_Scene->Load();
		m_Scene->OnLoaded();
		return true;
	}
	
	bool Layer::UnloadScene(Scene* scene)
	{
		scene->SetLayer(nullptr);
		m_Scene = nullptr;
		scene->Unload();
		scene->OnUnloaded();
		return true;
	}
	
	bool Layer::Detach()
	{
		// CSE_CORE_LOG("Layer ", m_Name, " detached.");
		UnloadScene(m_Scene);
		return true;
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
		GetScene()->GetRegistry().view<AnimationComponent>().each([=](auto entity, auto& animationComponent){
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
	
	void Layer::Draw()
	{
		Renderer::SetActiveScene(GetScene());
		
		for (auto entity : GetScene()->GetRegistry().view<SpriteComponent>())
		{
			Entity e = Entity(entity, GetScene());
			PositionComponent& position = e.GetComponent<PositionComponent>();
			TransformComponent& transform = e.GetComponent<TransformComponent>();
			SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
			
			SDL_FRect place; // where to draw
			SDL_Rect frame; // what to draw from a spritesheet
			
			glm::vec2 windowSize = 
			{ 
				GetWindow()->GetPrefs().width, 
				GetWindow()->GetPrefs().height 
			};
			
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
					spriteComponent.texture->GetWidth(), 
					spriteComponent.texture->GetHeight() 
				};
			}
			
			place = 
			{
				windowSize.x * (position.x - transform.size.x/2), 
				windowSize.y * (position.y - transform.size.y/2),
				windowSize.x * transform.size.x,
				windowSize.y * transform.size.y,
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
				if (e.HasComponent<PhysicsComponent>())
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
									{position.x + center.x, position.y + center.y}, 
									{transform.size.x, transform.size.y},
									{255, 128, 255, 255}
									);
							}
							break;
						case PhysicsDefines::HitBoxType::Rectangle:
							{
								Renderer::DrawRect(
									{position.x + physicsComponent.hitBoxes[i].points[0].x, position.y + physicsComponent.hitBoxes[i].points[0].y},
									{position.x + physicsComponent.hitBoxes[i].points[1].x, position.y + physicsComponent.hitBoxes[i].points[1].y},
									{position.x + physicsComponent.hitBoxes[i].points[2].x, position.y + physicsComponent.hitBoxes[i].points[2].y},
									{position.x + physicsComponent.hitBoxes[i].points[3].x, position.y + physicsComponent.hitBoxes[i].points[3].y},
									{255, 128, 255, 255}
									);
							}
							break;
						default:
							Renderer::DrawRect({position.x, position.y}, {transform.size.x, transform.size.y}, {255, 255, 255, 255});
						}
					}
				} else {
					Renderer::DrawRect({position.x, position.y}, {transform.size.x, transform.size.y});
				}
			}
		}
	}
}


