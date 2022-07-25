#include "Resources/Resourcesmanager.h"

void Resources::ResourcesManager::Delete(std::string const& filename)
{
	manager.erase(filename);
}