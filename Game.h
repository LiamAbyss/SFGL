#ifndef INCLUDE_GAME
#define INCLUDE_GAME

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include <functional>
#include "ResourceManager.h"
#include "TransparentWindow.h"
#include "Camera.h"
#include "Scene.h"
#include "Config.h"

namespace sfg
{

	class Scene;

	/**
	 * Global container for the game
	 */
	class Game
	{
	private:
		/**
		 * \private
		 * Contains all the scenes of the game.
		 * \see Scene
		 */
		std::map<std::string, std::shared_ptr<Scene>, std::less<>> scenes;

		/**
		 * \private
		 * Manages all the resources.
		 */
		ResourceManager resources_;

		/**
		 * \private
		 * Manages all the configs.
		 */
		Config config_;

		/**
		 * \private
		 * Time since launch() call.
		 */
		sf::Clock gameClock;

		/**
		 * \private
		 * Timer used to keep a stable maximum framerate.
		 */
		sf::Clock framerateClock;

		/**
		 * \private
		 * Time passed to Scene::update() since the last update.
		 */
		sf::Clock updateClock;

		/**
		 * \private
		 * Title of the main window.
		 */
		std::string windowTitle;

		/**
		 * \private
		 * Main window.
		 */
		sf::RenderWindow window_;

		/**
		 * \private
		 * Video mode of the main window.
		 */
		sf::VideoMode windowMode_;

		/**
		 * \private
		 * Framerate of the main window.
		 */
		unsigned short framerateLimit = 60;

		/**
		 * \private
		 * Label of the current scene.
		 * \see Scene
		 */
		std::string currentScene;

		/**
		 * \private
		 * Camera managing the view of the game.
		 */
		Camera camera;

		/**
		 * \private
		 * SFML event passed to Scene::update().
		 */
		sf::Event ev = sf::Event();

	public:

		/**
		 * \public
		 * Game constructor
		 * \param title Title of the main window.
		 * \param mode Dimensions of the main window.
		 * \param style Style of the main window.
		 */
		Game(const std::string& title, const sf::VideoMode& mode, const sf::Uint32& style = 7U);

		/**
		 * \public
		 * Game constructor
		 * \param configPath Path of the game config file
		 */
		Game(const std::string& configPath);

		/**
		 * \public
		 * Adds a scene to be loaded afterwards.
		 * \param label Label of the scene.
		 * \param scene A pointer on the scene allocated with \c new.
		 * \see Scene
		 */
		void addScene(const std::string& label, Scene* scene);

		/**
		 * \public
		 * Adds a scene to be loaded afterwards.
		 * \param label Label of the scene.
		 * \param scene A pointer on the scene allocated with \c new.
		 * \param configPath Path of the config file.
		 * \see Scene
		 */
		void addScene(const std::string& label, Scene* scene, const std::string& config);

		/**
		 * \public
		 * \return A reference to the map Game::scenes.
		 * \see Scene
		 */
		std::map<std::string, std::shared_ptr<Scene>, std::less<>>& getScenes();

		/**
		 * \public
		 * Sets the current scene.
		 * \param label Label of the scene.
		 * \see Scene
		 */
		void setCurrentScene(const std::string& label);

		/**
		 * \public
		 * \return A reference to the Game::resources.
		 * \see ResourceManager
		 */
		ResourceManager& resources();

		/**
		 * \public
		 * \return A reference to the Game::config.
		 * \see Config
		 */
		Config& config();

		/**
		 * \public
		 * \return A reference to the main window.
		 */
		sf::RenderWindow& window();

		/**
		 * \public
		 * \return A reference to the dimensions of the main window.
		 */
		sf::VideoMode& videoMode();

		/**
		 * \public
		 * \param mode New video mode.
		 * \param style New Style of the main window.
		 */
		void setVideoMode(const sf::VideoMode& mode, const sf::Uint32& style = 7U);

		/**
		 * \public
		 * \return The maximum framerate of the game.
		 */
		unsigned short getFramerateLimit() const;

		/**
		 * \public
		 * \param rate The maximum framerate of the game.
		 */
		void setFramerateLimit(unsigned short rate);

		/**
		 * \public
		 * Launches the game.
		 */
		void launch();

		/**
		 * \public
		 * Stops the game
		 */
		void stop();

		/**
		 * \public
		 * Sets the shape of the window according to an image with transparent parts.
		 * \param image The image to shape the window.
		 */
		bool setWindowShape(const sf::Image& image) const;

		/**
		 * \public
		 * Sets the transparency of the window.
		 * \param alpha the opacity (0-255).
		 */
		bool setWindowTransparency(const unsigned char alpha) const;

		/**
		 * \public
		 * \return A reference on the game camera.
		 * \see Camera
		 */
		Camera& getCamera();

		/**
		 * \public
		 * \return A reference on the game event.
		 */
		sf::Event& getEvent();
	};
}

#endif // !INCLUDE_GAME