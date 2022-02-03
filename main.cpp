#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.h"
#include "Geometry.h"
#include "Combo.h"
#include "SFUtils.h"

using namespace sfg;


class Test : public Scene
{
	Combo combo;

	// Hérité via Scene
	void initialize() override
	{
		sf::Time delay = sf::milliseconds(300);
		combo
			.hold(sf::Keyboard::S, delay)
			.hold(sf::Keyboard::D, delay)
			.unhold(sf::Keyboard::S, delay)
			.then(sf::Keyboard::Numpad4)
			.orElse()
			.hold(sf::Keyboard::S, delay)
			.hold(sf::Keyboard::D, delay)
			.unhold(sf::Keyboard::S, delay)
			.unhold(sf::Keyboard::D, delay)
			.then(sf::Keyboard::Numpad4)
			.orElse()
			.then(sf::Keyboard::S, delay)
			.then(sf::Keyboard::D, delay)
			.then(sf::Keyboard::Numpad4)
			.orElse()
			.then(sf::Keyboard::S, delay)
			.hold(sf::Keyboard::D, delay)
			.then(sf::Keyboard::Numpad4);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		if (combo.handle(ev))
			std::cout << "HADOKEN" << std::endl;
		if (ev.type == sf::Event::KeyReleased)
			std::cout << "Released ";
		if (ev.type == sf::Event::KeyPressed)
			std::cout << "Pressed ";
		if (ev.type == sf::Event::KeyReleased || ev.type == sf::Event::KeyPressed)
			std::cout << SFUtils::getKeyName(ev.key.code) << std::endl;
		if (ev.key.code == sf::Keyboard::Key::Space)
			std::cout << "\n";
	}	

	void update(sf::Time dt) override
	{

	}

	void render() override
	{
	}
};

int main() {

	Game game("Test", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("test", new Test());
	game.setCurrentScene("test");

	game.launch();

	return 0;
}