#include "Game.h"

Game::Game(const std::string& windowTitle, const sf::VideoMode& videoMode, const sf::Uint32& style) 
    : windowTitle(windowTitle), windowMode_(videoMode)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize the window
    window_.create(videoMode, windowTitle, style);
    window_.setFramerateLimit(framerateLimit);

    // Initialize the camera
    camera.setCenter(window_.getDefaultView().getCenter());
    camera.setSize(window_.getDefaultView().getSize());
    camera.setWindow(window_);
}

void Game::addScene(const std::string& label, Scene* scene)
{
    scenes.try_emplace(label, std::shared_ptr<Scene>(scene));
    scenes[label]->setGame((Game&)*this);
}

std::map<std::string, std::shared_ptr<Scene>, std::less<>>& Game::getScenes()
{
    return scenes;
}

void Game::setCurrentScene(const std::string& label)
{
    currentScene = label;
    scenes[label]->initialize();
    scenes[label]->setInitialized(true);
}

ResourceManager& Game::resources()
{
    return resources_;
}

sf::RenderWindow& Game::window()
{
    return window_;
}

sf::VideoMode& Game::videoMode()
{
    return windowMode_;
}

void Game::setVideoMode(const sf::VideoMode& mode, const sf::Uint32& style)
{
    window_.create(mode, windowTitle, style);
    window_.setFramerateLimit(framerateLimit);
    windowMode_ = mode;

    camera.setCenter(window_.getDefaultView().getCenter());
    camera.setSize(window_.getDefaultView().getSize());
    // TODO : Check camera.setWindow(window);
}

unsigned short Game::getFramerateLimit() const
{
    return framerateLimit;
}

void Game::setFramerateLimit(unsigned short rate)
{
    framerateLimit = rate;
    window_.setFramerateLimit(framerateLimit);
}

void Game::launch()
{
    // Check if scenes is empty
    if (scenes.empty())
        throw std::runtime_error("No scene has been added to the game.");
    
    // Check if the current scene is set
    if (currentScene.empty())
        throw std::runtime_error("No scene has been set as the current scene.");
    
    // Check if the current scene is in the map
    if (scenes.find(currentScene) == scenes.end())
        throw std::runtime_error("The scene " + currentScene + " was not found");
    
    window_.setFramerateLimit(framerateLimit);
    
    gameClock.restart();
    // TODO : Find usage for framerateClock.restart();
    updateClock.restart();

    // Main loop
    while (window_.isOpen())
    {
        // Handle events
        while (window_.pollEvent(ev))
        {
            scenes[currentScene]->update(updateClock.getElapsedTime(), ev);
            if (ev.type == sf::Event::Closed)
                window_.close();
        }

        // Update the scene
        scenes[currentScene]->update(updateClock.getElapsedTime());

        // Update the camera
        camera.update(updateClock.restart());
        window_.setView(camera);

        // Draw the scene
        window_.clear();
        scenes[currentScene]->render();
        window_.display();

        // Update the framerate
        // TODO : Find usage for framerateClock.restart();
    }

    // Print the game time
    std::cout << "Game time : " << gameClock.getElapsedTime().asSeconds() << " seconds" << std::endl;
}

void Game::stop()
{
    window_.close();
}

bool Game::setWindowShape(const sf::Image& image) const
{
	return ::setWShape(window_.getSystemHandle(), image);
}

bool Game::setWindowTransparency(unsigned char alpha) const
{
	return ::setWTransparency(window_.getSystemHandle(), alpha);
}

Camera& Game::getCamera()
{
    return camera;
}

sf::Event& Game::getEvent()
{
    return ev;
}
