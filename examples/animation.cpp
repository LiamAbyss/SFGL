#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.h"
#include "Sprite.h"

using namespace sfg;
class AnimatedScene : public Scene
{
	sfg::Sprite building;
	sfg::Sprite projectile;
	sf::Sprite background;
	sf::Sprite ground;

	float groundLevel = 0;
	bool inAnimation = false;

	// Hérité via Scene
	void initialize() override
	{
		resources().load("background", "Assets/city1.png");
		resources().load("ground", "Assets/ground.png");
		resources().load("building/walk", "Assets/walk.png");
		resources().load("building/dash", "Assets/dash.png");
		resources().load("building/punch", "Assets/punch.png");
		resources().load("building/throw", "Assets/throw.png");
		resources().load("building/thunder", "Assets/thunder.png");
		resources().load("projectile", "Assets/water.png");

		int repeats = 5;

		// BACKGROUND
		GTexture backTexture = resources().getTexture("background");
		backTexture->setRepeated(true);
		background.setTexture(*backTexture);
		background.setTextureRect(sf::IntRect(0, 0, backTexture->getSize().x * repeats, backTexture->getSize().y));
		background.setScale(
			static_cast<float>(window().getSize().x) / static_cast<float>(backTexture->getSize().x),
			static_cast<float>(window().getSize().y) / static_cast<float>(backTexture->getSize().y)
		);
		background.setPosition(-(float)backTexture->getSize().x * (float)(repeats / 2), 0);

		// GROUND
		GTexture groundTexture = resources().getTexture("ground");
		groundTexture->setRepeated(true);
		ground.setTexture(*groundTexture);
		ground.setTextureRect(sf::IntRect(0, 0, groundTexture->getSize().x * repeats, groundTexture->getSize().y));
		ground.setScale(
			static_cast<float>(window().getSize().x) / static_cast<float>(groundTexture->getSize().x),
			static_cast<float>(window().getSize().y) * 0.15F / static_cast<float>(groundTexture->getSize().y)
		);
		ground.setPosition(-(float)groundTexture->getSize().x * (float)(repeats / 2), window().getSize().y * 0.85F);

		// PLAYER
		building.init(resources());

		building.createAnim("walk", "building/walk", 6, 0, 5, 10);
		building.createAnim("dash", "building/dash", 4, 0, 3, 10, false);
		building.createAnim("punch", "building/punch", 3, 0, 2, 10, false);
		building.createAnim("throw", "building/throw", 7, 0, 6, 10, false);
		building.createAnim("thunder", "building/thunder", 10, 0, 9, 10, false);
		building.setCurrentAnim("walk");

		groundLevel = ground.getPosition().y;
		building.setPosition(50, groundLevel);

		projectile.init(resources());
		projectile.createAnim("default", "projectile", 4, 0, 3, 5);
		projectile.setCurrentAnim("default");
		projectile.setScale(1.8F, 1.8F);
		projectile.setPosition(-200, -200);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		if (inAnimation) return;
		if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Q)
			{
				building.setCurrentAnim("punch");
				inAnimation = true;
			}
			else if (ev.key.code == sf::Keyboard::D)
			{
				building.setCurrentAnim("dash");
				inAnimation = true;
			}
			else if (ev.key.code == sf::Keyboard::S)
			{
				building.setCurrentAnim("throw");
				inAnimation = true;
			}
			else if (ev.key.code == sf::Keyboard::Z)
			{
				building.setCurrentAnim("thunder");
				inAnimation = true;
			}
		}
	}

	void update(sf::Time dt) override
	{
		building.setOrigin(0, building.getGlobalBounds().height);
		building.animate(dt);
		projectile.animate(dt);
		float deltaX = 0;
		float posY = groundLevel;
		float speed = 300.F;
		float height = 200.F;

		if (!inAnimation)
		{
			// Flipping
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				building.flip(false);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				building.flip(true);

			// Moving
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				deltaX += dt.asSeconds() * speed;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				deltaX -= dt.asSeconds() * speed;
		}
		else
		{
			std::string key = building.getAnimationKey();
			if (key == "dash")
			{
				deltaX += dt.asSeconds() * speed * 2 * (building.isFlipped() ? -1 : 1);
			}
			else if (key == "thunder")
			{
				float progress = building.getAnimationProgress();
				if (progress < 0.5F)
				{
					posY = progress * -height + groundLevel;
				}
				else if (progress >= 0.8F)
					posY = (progress - 1.F) * height * 2 + groundLevel;
				else
					posY = building.getPosition().y;
			}

			inAnimation = !building.isAnimationFinished();
			if (!inAnimation)
			{
				if (key == "throw")
				{
					projectile.flip(building.isFlipped());
					sf::FloatRect rect = building.getGlobalBounds();
					sf::Vector2f offset(35, 85);
					if (projectile.isFlipped())
					{
						projectile.setPosition(rect.left - projectile.getGlobalBounds().width + offset.x, rect.top + offset.y);
					}
					else
					{
						projectile.setPosition(rect.left + rect.width - offset.x, rect.top + offset.y);
					}
				}
				else if (key == "thunder")
				{
					game().getCamera().shake(20, sf::milliseconds(150));
				}
				building.resetAnim(building.getAnimationKey());
				building.setCurrentAnim("walk");
			}
		}
		building.move(deltaX, 0);
		building.setPosition(building.getPosition().x, posY);
		projectile.move(dt.asSeconds() * speed * 0.9F * (projectile.isFlipped() ? -1 : 1), 0);
		game().getCamera().setCenterX(building.getPosition().x + building.getGlobalBounds().width / 2);
	}

	void render() override
	{
		window().draw(background);
		window().draw(ground);

		window().draw(building);
		
		window().draw(projectile);
	}
};

int main() {

	Game game("Animated scene example", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("Anim", new AnimatedScene());
	game.setCurrentScene("Anim");

	game.launch();

	return 0;
}