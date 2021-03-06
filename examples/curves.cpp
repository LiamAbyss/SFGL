#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.h"
#include "Sprite.h"
#include "Curves/Curves.h"
#include "Geometry.h"

using namespace sfg;

class SplineScene : public Scene
{
	CatmullRomCurve lcurve;

	sf::VertexArray points;
	sf::VertexArray triangle;
	sf::CircleShape triangle2;

	sf::CircleShape circle;

	float cursor = 0.0F;

	std::vector<sf::Vector2f> controlPoints;
	int selected = -1;

	// Hérité via Scene
	void initialize() override
	{
		points.setPrimitiveType(sf::PrimitiveType::LineStrip);
		triangle.setPrimitiveType(sf::PrimitiveType::Triangles);

		circle.setRadius(5);
		circle.setFillColor(sf::Color::Blue);

		triangle2.setPointCount(3);
		triangle2.setRadius(15);
		triangle2.setOrigin(15, 15);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		sf::Vector2f pos = (sf::Vector2f)sf::Mouse::getPosition(window());
		if (ev.type == sf::Event::MouseMoved && selected != -1)
		{
			controlPoints[selected] = pos;
			lcurve.setControlPoints(controlPoints);
			points.clear();
			for (float t = 0.0F; t < lcurve.getMaxT(); t += 0.005F)
			{
				points.append(sf::Vertex(lcurve.getPoint(t)));
			}
		}
		else if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Button::Left)
		{
			for (int i = 0; i < controlPoints.size(); i++)
			{
				auto& p = controlPoints[i];
				if (distance2(p, pos) < 5 * 5)
				{
					selected = i;
				}
			}
		}
		else if (ev.type == sf::Event::MouseButtonReleased)
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				selected = -1;
			}
			if (ev.mouseButton.button == sf::Mouse::Button::Right)
			{
				controlPoints.push_back(pos);
				lcurve.setControlPoints(controlPoints);
				points.clear();
				for (float t = 0.0F; t < lcurve.getMaxT(); t += 0.005F)
				{
					points.append(sf::Vertex(lcurve.getPoint(t)));
				}
			}
		}
		else if (ev.type == sf::Event::KeyReleased)
		{
			if (ev.key.code == sf::Keyboard::Space)
				lcurve.setClosed(!lcurve.isClosed()); 
			points.clear();
			for (float t = 0.0F; t < lcurve.getMaxT(); t += 0.005F)
			{
				points.append(sf::Vertex(lcurve.getPoint(t)));
			}
		}
	}	

	void update(sf::Time dt) override
	{
		if (lcurve.getControlPoints().size() < 2) return;

		if (cursor >= lcurve.getCurveLength())
			cursor -= lcurve.getCurveLength();

		// Draw agent to demonstrate gradient
		float offset = lcurve.getNormalisedOffset(cursor);

		// Get next pos
		sf::Vector2f p1 = lcurve.getPoint(offset);

		// Get orientation
		/*sf::Vector2f g1 = lcurve.getGradient(offset);
		float gLen = std::sqrtf(g1.x * g1.x + g1.y * g1.y);
		sf::Vector2f normalizedG = {g1.x / gLen, g1.y / gLen};

		float r = atan2(-g1.y, g1.x);
		triangle.clear();
		triangle.append(sf::Vector2f(15.0F * sin(r) + p1.x, 15.0F * cos(r) + p1.y));
		triangle.append(sf::Vector2f(-15.0F * sin(r) + p1.x, -15.0F * cos(r) + p1.y));
		sf::Vector2f midPoint = {
			0.5F * (triangle[0].position.x + triangle[1].position.x),
			0.5F * (triangle[0].position.y + triangle[1].position.y),
		};
		triangle.append(sf::Vector2f(midPoint.x + 30.0F * normalizedG.x, midPoint.y + 30.0F * normalizedG.y));*/

		// Or
		float r = getOrientation(lcurve.getPoint(std::max(0.0F, offset - 0.005F)), p1);
	
		triangle2.setPosition(p1);
		triangle2.setRotation(toDegrees(r)+90.F);

		cursor += dt.asSeconds() * 150.F;
	}

	void render() override
	{
		for (const auto& p : lcurve.getControlPoints())
		{
			circle.setPosition(p.x - 5, p.y - 5);
			window().draw(circle);
		}
		window().draw(points);
		window().draw(triangle2);
	}
};

int main() {

	Game game("Interpolating curves scene example", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("Curves", new SplineScene());
	game.setCurrentScene("Curves");

	game.launch();

	return 0;
}