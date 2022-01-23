#include "Scene.h"

void Scene::setGame(Game& game)
{
    this->game = &game;
    this->resources = &game.getResources();
    this->window = &game.getWindow();
}

{
}

Game& Scene::getGame()
{
    return static_cast<Game&>(*game);
}

ResourceManager& Scene::getResources()
{
    return static_cast<ResourceManager&>(*resources);
}

sf::RenderWindow& Scene::getWindow()
{
    return static_cast<sf::RenderWindow&>(*window);
}

void Scene::setInitialized(bool initialized)
{
    this->initialized = initialized;
}

bool Scene::hasBeenInitialized() const
{
    return initialized;
}