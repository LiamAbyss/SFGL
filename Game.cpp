#include "Game.h"

namespace sfg
{
    Game::Game(const std::string& windowTitle, const sf::VideoMode& videoMode, const sf::Uint32& style)
        : windowTitle(windowTitle), windowMode_(videoMode)
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        // Initialize the window
        window_.create(videoMode, windowTitle, style);
        window_.setFramerateLimit(framerateLimit);
        window_.setKeyRepeatEnabled(false);

        // Initialize the camera
        camera.setCenter(window_.getDefaultView().getCenter());
        camera.setSize(window_.getDefaultView().getSize());
        camera.setWindow(window_);
    }

    Game::Game(const std::string& configPath)
    {
        nlohmann::json conf;
        std::ifstream file;
        file.open(configPath);

        if (!file.is_open())
            throw std::runtime_error("Couldn't open config file");

        file >> conf;
        file.close();

        sf::Uint32 style = 0;

        for (const auto& s : conf["style"])
        {
            if (s == "Close")
                style |= sf::Style::Close;
            else if (s == "Default")
                style |= sf::Style::Default;
            else if (s == "Fullscreen")
                style |= sf::Style::Fullscreen;
            else if (s == "None")
                style |= sf::Style::None;
            else if (s == "Resize")
                style |= sf::Style::Resize;
            else if (s == "Titlebar")
                style |= sf::Style::Titlebar;
        }

        windowTitle = conf["title"];
        windowMode_ = sf::VideoMode(conf["videoMode"]["width"], conf["videoMode"]["height"]);

        srand(static_cast<unsigned int>(time(nullptr)));

        // Initialize the window
        window_.create(windowMode_, windowTitle, style);
        window_.setFramerateLimit(framerateLimit);
        window_.setKeyRepeatEnabled(false);

        // Initialize the camera
        camera.setCenter(window_.getDefaultView().getCenter());
        camera.setSize(window_.getDefaultView().getSize());
        camera.setWindow(window_);

        // Load resources
        nlohmann::json res = conf["resources"];

        for (const auto& r : res)
        {
            resources_.load(r["name"], r["path"]);
        }
        
        // Load scenes
        nlohmann::json sc = conf["sceneConfigs"];

        for (const auto& s : sc)
        {
            config_.loadConfig(s["name"], s["path"]);
        }

        // Load sprite configs
        nlohmann::json spriteConfigs = conf["spriteConfigs"];
        for (const auto& c : spriteConfigs)
        {
            config_.loadConfig(c["name"], c["path"]);
        }

    }

    void Game::addScene(const std::string& label, Scene* scene)
    {
        scenes.try_emplace(label, std::shared_ptr<Scene>(scene));
        scenes[label]->setGame((Game&)*this);
    }

    void Game::addScene(const std::string& label, Scene* scene, const std::string& config)
    {
        addScene(label, scene);
        scenes[label]->loadFromConfig(config);
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

    Config& Game::config()
    {
        return config_;
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
        window_.setKeyRepeatEnabled(false);
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

                if (ev.type == sf::Event::Resized)
                {
                    getCamera().setSize(static_cast<float>(ev.size.width), static_cast<float>(ev.size.height));
                }
                else if (ev.type == sf::Event::Closed)
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
}