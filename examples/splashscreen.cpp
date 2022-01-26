#include <SFML/Graphics.hpp>
#include "../Game.h"

class SplashScreen : public Scene
{
	// Duration of the splashscreen
	sf::Time timer;

	// Texture and sprite of the splashscreen
	sf::Texture imgtext;
	sf::Sprite img;

	// Hérité via Scene
	void initialize() override
	{
		//Load splash image
		sf::Image image;
		image.loadFromFile("./assets/splash.png");
		imgtext.loadFromImage(image);
		img.setTexture(imgtext);

		game().setVideoMode(sf::VideoMode(image.getSize().x, image.getSize().y), sf::Style::None);

		// Set the shape and position of the window
		game().setWindowShape(image);
		window().setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - image.getSize().x) / 2,
			(sf::VideoMode::getDesktopMode().height - image.getSize().y) / 2));

		timer = sf::Time::Zero;
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
	}

	void update(sf::Time dt) override
	{
		timer += dt;

		if (timer.asSeconds() > 2)
			// Do your thing
			game().stop();
	}

	void render() override
	{
		//Draw splashscreen
		window().clear();
		window().draw(img);
		window().display();
	}
};

int main() {

	Game game("Splashscreen example", sf::VideoMode(600, 600), sf::Style::None);

	game.addScene("Splash", new SplashScreen());
	game.setCurrentScene("Splash");

	game.launch();

	return 0;
}