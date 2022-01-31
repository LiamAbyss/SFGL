#include "Scene.h"

namespace sfg
{
    void Scene::setGame(Game& game)
    {
        this->game_ = &game;
        this->resources_ = &game.resources();
        this->window_ = &game.window();
        this->config_ = &game.config();
    }

    void Scene::loadFromConfig(const std::string& config)
    {
        nlohmann::json conf = this->config().getConfig(config);

        // Load resources
        nlohmann::json res = conf["resources"];

        for (const auto& r : res)
        {
            resources_->load(r["name"], r["path"]);
        }

        // Load sprite configs
        nlohmann::json spriteConfigs = conf["spriteConfigs"];
        for (const auto& c : spriteConfigs)
        {
            config_->loadConfig(c["name"], c["path"]);
        }
    }

    Game& Scene::game()
    {
        return static_cast<Game&>(*game_);
    }

    ResourceManager& Scene::resources()
    {
        return static_cast<ResourceManager&>(*resources_);
    }

    Config& Scene::config()
    {
        return static_cast<Config&>(*config_);
    }

    sf::RenderWindow& Scene::window()
    {
        return static_cast<sf::RenderWindow&>(*window_);
    }

    void Scene::setInitialized(bool initialized)
    {
        this->initialized = initialized;
    }

    bool Scene::hasBeenInitialized() const
    {
        return initialized;
    }
}