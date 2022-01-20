#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

#include "Game.h"

class SplashScreen : public Scene
{
	// Hérité via Scene
	virtual void initialize() override
	{
		std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

		//Load splash image
		sf::Image image;
		image.loadFromFile("./assets/splash.png");
		sf::Texture imgtext;
		imgtext.loadFromImage(image);
		sf::Sprite img;
		img.setTexture(imgtext);

		Game& game = getGame();
		sf::RenderWindow& window = getWindow();

		game.setVideoMode(sf::VideoMode(image.getSize().x, image.getSize().y), sf::Style::None);

		// Set the shape and position of the splashscreen
		game.setWindowShape(image);
		window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - image.getSize().x) / 2,
			(sf::VideoMode::getDesktopMode().height - image.getSize().y) / 2));

		//Draw splashscreen and wait a bit
		window.clear();// sf::Color(0, 0, 0, 255));
		window.draw(img);
		window.display();
		sf::sleep(sf::seconds(2));

		game.stop();
	}
	virtual void update(sf::Time dt, sf::Event& ev) override
	{
	}
	virtual void update(sf::Time dt) override
	{
	}
	virtual void render() override
	{
	}
};

int main() {

	Game game("Test", sf::VideoMode(600, 600), sf::Style::None);

	game.addScene("Main", new SplashScreen());
	game.setCurrentScene("Main");

	game.launch();

	return 0;
}