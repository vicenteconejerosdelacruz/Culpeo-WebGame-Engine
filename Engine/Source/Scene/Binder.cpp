#include "Binder.h"
#include <Scene.h>
#include <SceneObject.h>

using namespace Scene;

#if defined(_DEBUG_UUID_NAMES)
std::string Binder::name(JUUID uuid)
{
	if (bindingName.contains(uuid))
		return bindingName.at(uuid);

	std::string name = GetController(uuid)->sceneObject->name();
	bindingName.insert_or_assign(uuid, name);
	return name;
}
#endif

void Binder::insert(JUUID soA, JUUID soB)
{
	bool AtoB = false;
	auto rangeAtoB = binding.equal_range(soA);
	for (auto& it = rangeAtoB.first; it != rangeAtoB.second; it++)
	{
		if (it->second == soB)
		{
			AtoB = true;
			break;
		}
	}
	if (!AtoB)
	{
#if defined(_DEBUG_UUID_NAMES)
		std::cout << (std::string(std::string("insert:") + name(soA) + " -> " + name(soB) + "\n") << std::endl;
#endif
		binding.insert({ soA,soB });
		GetController(soA)->Bind(soB);
	}

	bool BtoA = false;
	auto rangeBtoA = binding.equal_range(soB);
	for (auto& it = rangeBtoA.first; it != rangeBtoA.second; it++)
	{
		if (it->second == soA)
		{
			BtoA = true;
			break;
		}
	}
	if (!BtoA)
	{
#if defined(_DEBUG_UUID_NAMES)
		std::cout << (std::string(std::string("insert:") + name(soB) + " -> " + name(soA) + "\n") << std::endl;
#endif
		binding.insert({ soB,soA });
		GetController(soB)->Bind(soA);
	}
}

void Binder::erase(JUUID soA)
{
	std::set<JUUID> soBs;
	auto rangeA = binding.equal_range(soA);
	for (auto it = rangeA.first; it != rangeA.second; it++)
	{
		soBs.insert(it->second);
	}
#if defined(_DEBUG_UUID_NAMES)
	std::cout << (std::string(std::string("erase:") + name(soA) + "\n") << std::endl;
#endif

	binding.erase(soA);
	for (auto soB : soBs)
	{
		auto rangeB = binding.equal_range(soB);
		for (auto it = rangeB.first; it != rangeB.second; )
		{
			if (it->second == soA)
			{
				it = binding.erase(it);
#if defined(_DEBUG_UUID_NAMES)
				std::cout << (std::string(std::string("erase:") + name(soB) + " -> " + name(soA) + "\n") << std::endl;
#endif
				if (ControllerExists(soA) && ControllerExists(soB))
				{
					GetController(soA)->Unbind(soB);
					GetController(soB)->Unbind(soA);
				}
			}
			else
				it++;
		}
	}
}

void Binder::erase(JUUID soA, JUUID soB)
{
	GetController(soA)->Unbind(soB);
	GetController(soB)->Unbind(soA);

	auto rangeA = binding.equal_range(soA);
	for (auto it = rangeA.first; it != rangeA.second; )
	{
		if (it->second == soB)
		{
#if defined(_DEBUG_UUID_NAMES)
			std::cout << (std::string(std::string("erase:") + name(soA) + " -> " + name(soB) + "\n") << std::endl;
#endif
			it = binding.erase(it);
		}
		else
			it++;
	}
	auto rangeB = binding.equal_range(soB);
	for (auto it = rangeB.first; it != rangeB.second; )
	{
		if (it->second == soA)
		{
#if defined(_DEBUG_UUID_NAMES)
			std::cout << (std::string(std::string("erase:") + name(soB) + " -> " + name(soA) + "\n") << std::endl;
#endif
			it = binding.erase(it);
		}
		else
			it++;
	}
}