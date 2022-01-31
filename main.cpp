#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.h"
#include "Sprite.h"

using namespace sfg;

class ConfigScene : public Scene
{
	Sprite building;

	// Hérité via Scene
	void initialize() override
	{
		building.init(this);

		building.loadFromConfig("config/building");

		building.setPosition(100, 100);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		if (ev.type == sf::Event::Resized)
		{
			building.setPosition(100, 100);
		}
	}	

	void update(sf::Time dt) override
	{
		building.animate(dt);
	}

	void render() override
	{
		window().draw(building);
	}
};

int main() {

	Game game("Assets/config.json");

	game.addScene("ConfigScene", new ConfigScene(), "config/ConfigScene");
	game.setCurrentScene("ConfigScene");

	game.launch();

	return 0;
}