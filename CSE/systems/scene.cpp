#include <CSE/systems/scene.h>
#include <CSE/systems/entity.h>
#include <CSE/systems/components.h>

#include <CSE/systems/renderer.h>
#include <CSE/systems/window.h>
#include <CSE/systems/layer.h>

namespace CSE
{
	Scene::Scene()
	{
	}
	
	Scene::~Scene()
	{
		m_Layer = nullptr;
	}
	
	void Scene::Init()
	{
	}
	
	void Scene::OnLoaded()
	{
	}
	
	void Scene::OnUnloaded()
	{
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
	
	void Scene::UpdateGraphics(TimeType sceneTime)
	{
		// TODO: Scene Update mechanism
		Animate(sceneTime);
		Draw();
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
		for (auto entity : m_Registry.view<SpriteComponent>())
		{
			Entity e = Entity(entity, this);
			PositionComponent& position = e.GetComponent<PositionComponent>();
			SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
			
			SDL_FRect place; // where to draw
			SDL_Rect frame; // what to draw from a spritesheet
			
			place = {50, 50, 79, 29};
			frame = {0, 0, 79, 29};
			
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
				
				place = 
				{
					position.NormalizedToWindow(m_Layer->GetWindow()).x - frame.w/2, 
					position.NormalizedToWindow(m_Layer->GetWindow()).y - frame.h/2,
					frame.w, 
					frame.h
				};
				
				// CSE_CORE_LOG("Place: { ", place.x, ", ", place.y, ", ", place.w, ", ", place.h, " }");
			} else {
				frame = 
				{
					0, 
					0, 
					spriteComponent.texture->GetWidth(), 
					spriteComponent.texture->GetHeight() 
				};
				
				place = 
				{
					// position.NormalizedToWindow(m_Layer->GetWindow()).x - spriteComponent.texture->GetWidth()/2,
					0, 
					// position.NormalizedToWindow(m_Layer->GetWindow()).y - spriteComponent.texture->GetHeight()/2,
					0,
					// spriteComponent.texture->GetWidth(),
					m_Layer->GetWindow()->GetPrefs().width,
					// spriteComponent.texture->GetHeight()
					m_Layer->GetWindow()->GetPrefs().height
				};
			}

			CSE::Renderer::DrawTiledTexture(
				spriteComponent.texture->GetTexture(),
				&place,
				&frame,
				m_Layer->GetWindow()->GetScale().x,
				m_Layer->GetWindow()->GetScale().y,
				spriteComponent.tiling
				);
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
	
	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity.GetID());
	}
}


