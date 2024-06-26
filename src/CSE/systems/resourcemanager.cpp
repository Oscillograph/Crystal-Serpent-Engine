#include <CSE/systems/resourcemanager.h>
#include <CSE/systems/renderer/texture.h>
#include <CSE/systems/scene.h>
#include <CSE/systems/layer.h>
#include <CSE/systems/window.h>
#include <CSE/systems/renderer.h>

namespace CSE
{
	std::unordered_map<ResourceType, ResourceCache> ResourceManager::m_Cache;
	
	Resource::Resource(
		const std::string& _path,
		ResourceType _type,
		void* _data
		)
	: path(_path), type(_type), data(_data)
	{
	}
	
	int ResourceManager::LoadResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it != m_Cache[type].end())
				return -2; // already exists
			
			Resource* resource = new Resource(
				path,
				type,
				nullptr
				);
			
			m_Cache[type][path] = resource;
			
			switch (type) 
			{
			case ResourceType::Text_Plain:
				{
					// open file
					std::ifstream m_FileInput(path);
					if (!m_FileInput.is_open())
						m_FileInput.open(path);
					CSE_CORE_LOG("- file \"", path.c_str(), "\" opened for reading.");
					
					// construct a text object and read file contents into it
					// it should be connected somehow to text system and font system
					std::string contents = "";
					char* buffer = new char[1024];
					if (m_FileInput.good())
					{
						if (m_FileInput.rdstate() && std::ifstream::eofbit)
							CSE_CORE_LOG("- end of file reached somehow.");
						while (!m_FileInput.eof())
						{
							m_FileInput.getline(buffer, 1024, '\n');
							contents.append(buffer);
							contents.append("\n");
						}
						m_FileInput.seekg(0);
						m_FileInput.clear();
						CSE_CORE_LOG("- job Done!");
					} else {
						CSE_CORE_LOG("- we're not good!");
					}
					
					delete[] buffer;
					resource->data = new Data::Text(contents);
					
					// close file
					if (!m_FileInput.is_open())
						m_FileInput.close();
					CSE_CORE_LOG("- file \"", path.c_str(), "\" closed.");
				}
				break;
			case ResourceType::Texture:
				{
					resource->data = new Texture(path, user->GetRenderer());
				}
				break;
			default:
				{}
				break;
			}
		}
		
		return 0;
	}
	
	Resource* ResourceManager::UseResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		CSE_CORE_LOG("Requesting resource of type ", (int)type, " with path \"", path.c_str(), "\".");
		auto it = m_Cache[type].find(path);
		if (it == m_Cache[type].end())
		{
			CSE_CORE_LOG("Resource Manager: There is no such resource with path \"", path.c_str(), "\".");
			CSE_CORE_LOG("Resource Manager: loading a resource...");
			LoadResource(type, path, user);
			m_Cache[type][path]->users.push_back(user);
			CSE_CORE_LOG("Resource Manager: resource is loaded, its user is registered.");
			return m_Cache[type][path];
		}
		
		// register a new user if necessary
		bool newUser = true;
		for (auto usersIterator = (*it).second->users.begin(); usersIterator != (*it).second->users.end(); usersIterator++)
		{
			if ((*usersIterator) == user)
				newUser = false; // false alarm: the user was registered before
		}
		if (newUser)
			(*it).second->users.push_back(user);
		
		return (*it).second;
	}
	
	int ResourceManager::DropResource(Resource* resource, const ResourceUser& user)
	{
		if (resource != nullptr)
		{
			return DropResource(resource->type, resource->path, user);
		} else {
			CSE_CORE_LOG("Asking to drop a resource which is nullptr!");
			return -1; // resource is nullptr
		}
	}
	
	int ResourceManager::DropResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		CSE_CORE_LOG("Resource dropping procedure...");
		if (m_Cache.find(type) == m_Cache.end())
		{
			CSE_CORE_LOG("resource type not found");
			return -1; // no such resource type cache registered
		} else {
			CSE_CORE_LOG("resource type found");
			auto it = m_Cache[type].find(path);
			if (it == m_Cache[type].end())
			{
				CSE_CORE_LOG("resource not found");
				return -2; // couldn't find a resource
			}
		
			// unregister a user
			if (m_Cache[type][path] != nullptr)
			{
				CSE_CORE_LOG("m_Cache[type][path] != nullptr");
				for (auto it = m_Cache[type][path]->users.begin(); it != m_Cache[type][path]->users.end(); it++)
				{
					CSE_CORE_LOG(*it, " ", user);
					if ((*it) == user)
					{
						CSE_CORE_LOG("resource user found");
						(*it) = nullptr;
						m_Cache[type][path]->users.erase(it);
						CSE_CORE_LOG("user unregistered");
						break;
					}
				}
				
				// if there are no users left, mark the resource for deletion
				if (m_Cache[type][path]->users.size() == 0)
				{
					m_Cache[type][path]->deletionFlag = true;
					CSE_CORE_LOG("Flagged resource \"", path.c_str(), "\" for deletion.");
				}
			} else {
				CSE_CORE_LOG("m_Cache[type][path] == nullptr");
			}
		}
		
		return 0;
	}
	
	int ResourceManager::UnloadResource(ResourceType type, const std::string& path)
	{
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it == m_Cache[type].end())
				return -1; // couldn't find a resource
			
			for (auto it = m_Cache[type][path]->users.begin(); it != m_Cache[type][path]->users.end(); it++)
			{
				(*it) = nullptr;
				m_Cache[type][path]->users.erase(it);
			}
			
			delete m_Cache[type][path];
		}
		
		return 0;
	}
	
	int ResourceManager::LoadTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey)
	{
		ResourceType type = ResourceType::Texture;
		
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it != m_Cache[type].end())
				return -2; // already exists
			
			Resource* resource = new Resource(
				path,
				type,
				nullptr
				);
			
			// TODO: monitor layers among users
			resource->data = new Texture(path, user->GetRenderer(), {colorKey.x, colorKey.y, colorKey.z});
			
			m_Cache[type][path] = resource;
			
		}
		
		return 0;
	}
	
	Resource* ResourceManager::UseTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey)
	{
		ResourceType type = ResourceType::Texture;
		CSE_CORE_LOG("Requesting a texture with path \"", path.c_str(), "\".");
		auto it = m_Cache[type].find(path);
		if (it == m_Cache[type].end())
		{
			CSE_CORE_LOG("Resource Manager: There is no such resource with path \"", path.c_str(), "\".");
			CSE_CORE_LOG("Resource Manager: loading a resource...");
			LoadTexture(path, user, colorKey);
			m_Cache[type][path]->users.push_back(user);
			CSE_CORE_LOG("Resource Manager: resource is loaded, its user is registered.");
			return m_Cache[type][path];
		}
		
		// register a new user if necessary
		bool newUser = true;
		for (auto usersIterator = (*it).second->users.begin(); usersIterator != (*it).second->users.end(); usersIterator++)
		{
			if ((*usersIterator) == user)
				newUser = false; // false alarm: the user was registered before
		}
		if (newUser)
			(*it).second->users.push_back(user);
		
		return (*it).second;
	}
	
	void ResourceManager::Init()
	{
		CSE_CORE_LOG("Resource Manager: Initialization.");
		if (m_Cache.empty())
		{
			InitCache(ResourceType::Text_Plain);
			InitCache(ResourceType::Texture);
			InitCache(ResourceType::Audio);
		}
	}
	
	void ResourceManager::InitCache(ResourceType type)
	{
		m_Cache[type].reserve(256);
		CSE_CORE_LOG("Resource Manager: Cache type ", (int)type, " has been prepared to store elements.");
	}
	
	void ResourceManager::GarbageCollector()
	{
		// select all types of resources
		CSE_CORE_LOG("Resource Manager: Garbage collector starts...");
		ResourceType type;
		for (auto typeIterator = m_Cache.begin(); typeIterator != m_Cache.end(); typeIterator++)
		{
			type = (*typeIterator).first;
			// CSE_CORE_LOG("- choosing resource category ", (int)type, "...");
			if (m_Cache.find(type) != m_Cache.end())
			{
				// CSE_CORE_LOG("- walking through resource category ", (int)type, "...");
				// for every resource of the selected type do
				for (auto cacheIterator = m_Cache[type].begin(); cacheIterator != m_Cache[type].end(); )
				{
					// CSE_CORE_LOG("- - selected a resource \"", (*cacheIterator).second->path.c_str(), "\"...");
					if ((*cacheIterator).second == nullptr)
					{
						// CSE_CORE_LOG("- - resource was deleted before...");
						cacheIterator = m_Cache[type].erase(cacheIterator);
					} else {
						// when do we delete a resource from the cache?
						if (((*cacheIterator).second->deletionFlag) || // when it was marked to
							((*cacheIterator).second->data == nullptr) || // when there is no data
							((*cacheIterator).second->users.size() == 0) // when there are no users
							)
						{
							// CSE_CORE_LOG("- - resource is flagged for deletion...");
							// nullify all users
							for (auto usersIterator = (*cacheIterator).second->users.begin(); usersIterator != (*cacheIterator).second->users.end(); usersIterator++)
							{
								(*usersIterator) = nullptr;
							}
							// CSE_CORE_LOG("- - users list cleared...");
							
							// delete the resource data
							if ((*cacheIterator).second->data != nullptr)
								delete (*cacheIterator).second->data;
							(*cacheIterator).second->data = nullptr;
							
							// delete the resource itself
							delete (*cacheIterator).second;
							(*cacheIterator).second = nullptr;
							cacheIterator = m_Cache[type].erase(cacheIterator);
							// CSE_CORE_LOG("- - resource deleted...");
						} else {
							cacheIterator++;
						}
					}
				}
			}
		}
		CSE_CORE_LOG("Resource Manager: Garbage collector stops.");
	}
	
	void ResourceManager::ShutDown()
	{
		CSE_CORE_LOG("Resource Manager: Shutdown...");
		CSE_CORE_LOG("- flagging all resources for deletion...");
		for (uint32_t i = 0; i < 100; i++)
		{
			if (m_Cache.find((ResourceType)i) != m_Cache.end())
			{
				// for every resource of the selected type do
				for (auto cacheIterator = m_Cache[(ResourceType)i].begin(); cacheIterator != m_Cache[(ResourceType)i].end(); cacheIterator++)
				{
					if ((*cacheIterator).second)
					{
						(*cacheIterator).second->deletionFlag = true;
					}
				}
			}
		}
		CSE_CORE_LOG("...done.");
		CSE_CORE_LOG("Resource Manager: call Garbage collector to do the rest.");
		GarbageCollector();
		CSE_CORE_LOG("Resource Manager: Shutdown complete.");
	}
}
