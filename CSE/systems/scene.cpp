#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>
#include <CSE/systems/physics.h>

#include <CSE/systems/application.h>
#include <CSE/systems/renderer.h>
#include <CSE/systems/renderer/camera2d.h>
#include <CSE/systems/window.h>
#include <CSE/systems/layer.h>

namespace CSE
{
	Scene::Scene()
	{
		m_SceneCamera = new Camera2D();
		
		m_PhysicsProcessor = new PhysicsProcessor(PhysicsSystem::CSE);
	}
	
	Scene::Scene(const PhysicsSystem& physicsSystem)
	{
		m_SceneCamera = new Camera2D();
		
		m_PhysicsProcessor = new PhysicsProcessor(physicsSystem);
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
		
		if (m_SceneCamera != nullptr)
			delete m_SceneCamera;
		m_SceneCamera = nullptr;
	}
	
	void Scene::Init()
	{
	}
	
	void Scene::OnInitialized()
	{
		m_Initialized = true;
	}
	
	void Scene::Load()
	{
	}
	
	void Scene::OnLoaded()
	{
	}
	
	void Scene::Unload() // should be called before OnUnloaded
	{
	}
	
	void Scene::OnUnloaded()
	{
		if (m_PhysicsProcessor != nullptr)
		{
			delete m_PhysicsProcessor;
			m_PhysicsProcessor = nullptr;
		}
		if (m_CurrentWorld != nullptr)
		{
			delete m_CurrentWorld;
			m_CurrentWorld = nullptr;
		}
	}
	
	void Scene::OnUpdate(TimeType sceneTime)
	{
	}
	
	void Scene::Start()
	{
	}
	
	void Scene::Stop()
	{
	}
	
	void Scene::Pause()
	{
	}
	
	void Scene::Resume()
	{
	}
	
	/*
	void Scene::UpdateGraphics(TimeType sceneTime)
	{
		// TODO: Scene Update mechanism
		// Animate(sceneTime);
		// Draw();
	}
	
	void Scene::Animate(TimeType sceneTime)
	{
		m_Registry.view<AnimationComponent>().each([=](auto entity, auto& animationComponent){
			auto* frameset = animationComponent.frames[animationComponent.currentAnimation];
			
			if (!animationComponent.paused)
			{
				uint64_t timeTemp = sceneTime;
				if ((timeTemp - animationComponent.timeBefore) >= frameset->timeBetweenFrames)
				{
					animationComponent.timeBefore = timeTemp;
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
	
	void Scene::Draw()
	{
		Renderer::SetActiveScene(this);
		
		for (auto entity : m_Registry.view<SpriteComponent>())
		{
			Entity e = Entity(entity, this);
			PositionComponent& position = e.GetComponent<PositionComponent>();
			TransformComponent& transform = e.GetComponent<TransformComponent>();
			SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
			
			SDL_FRect place; // where to draw
			SDL_Rect frame; // what to draw from a spritesheet
			
			glm::vec2 windowSize = 
			{ 
				m_Layer->GetWindow()->GetPrefs().width, 
				m_Layer->GetWindow()->GetPrefs().height 
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
	*/
	
	void Scene::UpdatePhysics(TimeType sceneTime) // calls physics processor general routine
	{
		if (m_PhysicsProcessor != nullptr)
		{
			m_PhysicsProcessor->GeneralRoutine(this, sceneTime);
			// CSE_CORE_LOG("Physics processor general routine called and processed");
		}
	}
	
	Entity* Scene::CreateEntity(const std::string& name)
	{
		Entity* entity = new Entity( m_Registry.create(), this );
		
		// TODO: create default components
		auto& nameComponent = entity->AddComponent<NameComponent>();
		nameComponent.value = name.empty() ? "Nameless entity" : name;
		// entity->AddComponent<TransformComponent>();
		return entity; 
	}
	
	Entity* Scene::CreateEntityWithUUID(const std::string& name, uint64_t uuid)
	{
		Entity* entity = CreateEntity(name);
		
		if (uuid == 0) // assume that 0 is not a valid uuid
			uuid = Utils::UUID::Generate();
		auto& uuidComponent = entity->AddComponent<UUIDComponent>(uuid);
		
		return entity; 
	}
	
	Entity* Scene::CreatePhysicalEntity(const std::string& name)
	{
		Entity* entity = CreateEntity(name);
		entity->AddComponent<PhysicsComponent>();
		m_PhysicsProcessor->RegisterEntity(entity);
		
		return entity;
	}
	
	void Scene::DestroyEntity(Entity& entity)
	{
		if (entity.HasComponent<PhysicsComponent>())
			m_PhysicsProcessor->UnregisterEntity(&entity);
		
		m_Registry.destroy(entity.GetID());
	}
}


