#include <SFML/Graphics.hpp>
#include "Game.h"

class BezierCurve
{
public:
	std::vector<sf::Vector2f> controlPoints;

	sf::Vector2f compute(float t) const
	{
		return compute(controlPoints, t);
	}

	static sf::Vector2f compute(const std::vector<sf::Vector2f>& controlPoints, float t)
	{
		if (controlPoints.empty()) throw std::runtime_error("No control points were defined for the Bezier curve.");

		if (controlPoints.size() == 1)
		{
			return controlPoints[0];
		}
		if (controlPoints.size() == 2)
		{
			float x = controlPoints[0].x + (controlPoints[1].x - controlPoints[0].x) * t;
			float y = controlPoints[0].y + (controlPoints[1].y - controlPoints[0].y) * t;

			return { x, y };
		}

		std::vector<sf::Vector2f> tPoints(controlPoints.size() - 1);

		for (int i = 0; i < controlPoints.size() - 1; i++)
		{
			float x = controlPoints[i].x + (controlPoints[i + 1].x - controlPoints[i].x) * t;
			float y = controlPoints[i].y + (controlPoints[i + 1].y - controlPoints[i].y) * t;

			tPoints[i] = { x, y };
		}
		return compute(tPoints, t);
	}
};

class Cinemator
{
private:
	std::vector<BezierCurve> curves;
	float animDuration = 1.F;
	sf::Time progress = sf::Time::Zero;
	std::vector<sf::Time> pauses;
	sf::Transformable* object;

public:
	void setDuration(float duration)
	{
		animDuration = duration;
	}

	void addPoint(sf::Vector2f point)
	{
		if (curves.size() == pauses.size())
			curves.push_back(BezierCurve());
		curves[curves.size() - 1].controlPoints.push_back(point);
	}

	void addPause(sf::Time pause)
	{
		if (curves.empty()) return;
		if (curves.size() != pauses.size())
			pauses.push_back(sf::Time::Zero);
		pauses[pauses.size() - 1] += pause;
	}

	void setObject(sf::Transformable& obj)
	{
		object = &obj;
	}

	bool playCinematic(sf::Time dt)
	{
		float durationPerCurve = animDuration / static_cast<float>(curves.size());

		// Get the id of the current curve
		auto id = static_cast<int>(static_cast<float>(curves.size()) * (progress.asSeconds() / animDuration));

		if(id > 0 && id < curves.size() && pauses[id - 1].asSeconds() > 0)
		{
			pauses[id - 1] -= dt;
			if (pauses[id - 1].asSeconds() < 0) progress += -pauses[id - 1];
			return true;
		}
		
		progress += dt;
		if (progress.asSeconds() >= animDuration) 
		{
			object->setPosition(curves[curves.size() - 1].controlPoints[curves[curves.size() - 1].controlPoints.size() - 1]);
			return false;
		}

		float t = (progress.asSeconds() - static_cast<float>(id) * durationPerCurve) / durationPerCurve;
		object->setPosition(curves[id].compute(t));

		return true;
	}

};

class Cinematic : public Scene
{
	Cinemator rectCinemator;
	sf::RectangleShape rect;
	sf::Time delay = sf::Time::Zero;

	// Hérité via Scene
	void initialize() override
	{
		rect.setFillColor(sf::Color::Red);
		rect.setPosition(0, 0);
		rect.setSize(sf::Vector2f(30, 30));

		rectCinemator.setObject(rect);
		rectCinemator.addPoint({ 0, 0 });
		rectCinemator.addPoint({ 150, 200 });
		rectCinemator.addPoint({ 50, 310 });
		//rectCinemator.addPause(sf::seconds(1));
		rectCinemator.addPoint({ 50, 310 });
		rectCinemator.addPoint({ 550, 310 });
		rectCinemator.addPoint({ 620, 54 });
		rectCinemator.addPoint({ 0, 0 });
		rectCinemator.setDuration(10);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
	}

	void update(sf::Time dt) override
	{
		delay += dt;
		if(delay.asSeconds() > 2)
		{
			rectCinemator.playCinematic(dt);
		}
	}

	void render() override
	{
		//Draw splashscreen
		window().draw(rect);
	}
};

int main() {

	Game game("Cinematic example", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("Cinematic", new Cinematic());
	game.setCurrentScene("Cinematic");

	game.launch();

	return 0;
}