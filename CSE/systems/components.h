#ifndef CSE_COMPONENTS_H
#define CSE_COMPONENTS_H

#include <CSE/core.h>

namespace CSE
{
	// Convention:
	// If a component has one variable in store, then the variable is called Value
	
	struct NameComponent 
	{
		std::string Value = "";
		
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name)
		: Value(name) {};
	};
	
	struct UUIDComponent 
	{
		uint64_t ID = 0;
		
		UUIDComponent() 
		: ID(Utils::UUID::Generate()) 
		{};
		UUIDComponent(const UUIDComponent&) = default;
		UUIDComponent(const uint64_t& uuid)
		: ID(uuid) 
		{};
	};
}

#endif
