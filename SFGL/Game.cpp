#include "Game.h"

Game::Game(const std::string& windowTitle, const sf::VideoMode& videoMode, const sf::Uint32& style)
: windowTitle(windowTitle), windowMode(videoMode) {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize the window
    window.create(videoMode, windowTitle, style);
    window.setFramerateLimit(framerateLimit);

    // Initialize the camera
    camera.setCenter(window.getDefaultView().getCenter());
    camera.setSize(window.getDefaultView().getSize());
    camera.setWindow(window);
}

void Game::addScene(const std::string& label, Scene* scene) {
    scenes.try_emplace(label, std::shared_ptr<Scene>(scene));
    scenes[label]->setGame((Game&) *this);
}

std::map<std::string, std::shared_ptr<Scene>, std::less<>>& Game::getScenes() { return scenes; }

void Game::setCurrentScene(const std::string& label) {
    currentScene = label;
    scenes[label]->initialize();
    scenes[label]->setInitialized(true);
}

ResourceManager& Game::getResources() { return resources; }

sf::RenderWindow& Game::getWindow() { return window; }

sf::VideoMode& Game::getVideoMode() { return windowMode; }

void Game::setVideoMode(const sf::VideoMode& mode, const sf::Uint32& style) {
    window.create(mode, windowTitle, style);
    window.setFramerateLimit(framerateLimit);
    windowMode = mode;

    camera.setCenter(window.getDefaultView().getCenter());
    camera.setSize(window.getDefaultView().getSize());
    // TODO : Check camera.setWindow(window);
}

unsigned short Game::getFramerateLimit() const { return framerateLimit; }

void Game::setFramerateLimit(unsigned short rate) {
    framerateLimit = rate;
    window.setFramerateLimit(framerateLimit);
}

void Game::launch() {
    // Check if scenes is empty
    if (scenes.empty()) throw std::runtime_error("No scene has been added to the game.");

    // Check if the current scene is set
    if (currentScene.empty())
        throw std::runtime_error("No scene has been set as the current scene.");

    // Check if the current scene is in the map
    if (scenes.find(currentScene) == scenes.end())
        throw std::runtime_error("The scene " + currentScene + " was not found");

    window.setFramerateLimit(framerateLimit);

    gameClock.restart();
    // TODO : Find usage for framerateClock.restart();
    updateClock.restart();

    // Main loop
    while (window.isOpen()) {
        // Handle events
        while (window.pollEvent(ev)) {
            scenes[currentScene]->update(updateClock.getElapsedTime(), ev);
            if (ev.type == sf::Event::Closed) window.close();
        }

        // Update the scene
        scenes[currentScene]->update(updateClock.getElapsedTime());

        // Update the camera
        camera.update(updateClock.restart());
        window.setView(camera);

        // Draw the scene
        window.clear();
        scenes[currentScene]->render();
        window.display();

        // Update the framerate
        // TODO : Find usage for framerateClock.restart();
    }

    // Print the game time
    std::cout << "Game time : " << gameClock.getElapsedTime().asSeconds() << " seconds"
              << std::endl;
}

void Game::stop() { window.close(); }

bool Game::setWindowShape(const sf::Image& image) const {
#if defined(SFML_SYSTEM_WINDOWS)
    return ::setWShape(window.getSystemHandle(), image);
#endif
    return false;
}

bool Game::setWindowTransparency(unsigned char alpha) const {
#if defined(SFML_SYSTEM_WINDOWS)
    return ::setWTransparency(window.getSystemHandle(), alpha);
#endif
    return false;
}

Camera& Game::getCamera() { return camera; }

sf::Event& Game::getEvent() { return ev; }
