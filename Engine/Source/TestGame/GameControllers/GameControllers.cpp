#include <vector>
#include <string>
#include <Controller.h>

namespace Game
{
    std::unordered_map<std::string, std::function<std::unique_ptr<Game::Controller>(nlohmann::json&)>> controllers =
	{
    };

    std::vector<std::string> GetControllers()
    {
        std::vector<std::string> engineControllers = GetEngineControllers();
        std::vector<std::string> gameControllers = nostd::GetKeysFromMap(controllers);
        gameControllers.insert(gameControllers.end(),engineControllers.begin(),engineControllers.end());
        return gameControllers;
    }

    JUUID CreateController(std::string name, SUUUID sceneObject, nlohmann::json& json)
    {
        if (!controllers.contains(name)) return CreateEngineController(name,sceneObject,json);
        std::unique_ptr<Game::Controller> controller = controllers.at(name)(json);
        JUUID uuid = RegisterController(name, sceneObject, controller);
        return uuid;
    }
}