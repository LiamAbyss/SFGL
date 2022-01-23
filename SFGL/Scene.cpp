#include "Scene.h"

void Scene::setGame(Game& game)
{
    this->game_ = &game;
    this->resources_ = &game.resources();
    this->window_ = &game.window();
}

Game& Scene::game()
{
    return static_cast<Game&>(*game_);
}

ResourceManager& Scene::resources()
{
    return static_cast<ResourceManager&>(*resources_);
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