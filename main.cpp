#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.h"
#include "Sprite.h"
#include "Geometry.h"

using namespace sfg;


class HitboxScene : public Scene
{
	sfg::Sprite building;
	sfg::Sprite ground;
	sf::Sprite background;

	bool inAnimation = false;

	// H?rit? via Scene
	void initialize() override
	{
		resources().load("background", "Assets/city1.png");
		resources().load("ground", "Assets/ground.png");
		/*resources().load("building/walk", "Assets/building/walk.png");
		resources().load("building/dash", "Assets/building/dash.png");

		config().loadConfig("building/walk", "Assets/building/walk.json");
		config().loadConfig("building/dash", "Assets/building/dash.json");*/
		config().loadConfig("ground/hitbox", "Assets/ground.json");

		// BACKGROUND
		GTexture backTexture = resources().getTexture("background");
		backTexture->setRepeated(true);
		background.setTexture(*backTexture);
		background.setTextureRect(sf::IntRect(0, 0, backTexture->getSize().x, backTexture->getSize().y));
		background.setScale(
			static_cast<float>(window().getSize().x) / static_cast<float>(backTexture->getSize().x),
			static_cast<float>(window().getSize().y) / static_cast<float>(backTexture->getSize().y)
		);

		// GROUND
		GTexture groundTexture = resources().getTexture("ground");
		ground.init(this);
		ground.setScale(
			static_cast<float>(window().getSize().x) / static_cast<float>(groundTexture->getSize().x),
			static_cast<float>(window().getSize().y) * 0.15F / static_cast<float>(groundTexture->getSize().y)
		);
		ground.setPosition(0, (float)window().getSize().y * 0.85F);

		ground.createAnim("ground", "ground", 1, 0, 0, 0);
		ground.setCurrentAnim("ground");

		ground.loadHitboxes("ground", "ground/hitbox");

		// PLAYER
		building.init(this);
		config().loadConfig("config/building", "Assets/building/building.json");
		building.loadFromConfig("config/building");
		/*building.createAnim("walk", "building/walk", 6, 0, 5, 10);
		building.createAnim("dash", "building/dash", 4, 0, 3, 10, false);
		building.setCurrentAnim("walk");


		building.loadHitboxes("walk", "building/walk");
		building.loadHitboxes("dash", "building/dash");*/

		float groundLevel = ground.getPosition().y;
		building.setPosition(50, groundLevel - 400);
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		if (inAnimation) return;
		if (ev.type == sf::Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::D)
			{
				building.setCurrentAnim("dash");
				inAnimation = true;
			}
		}
	}

	void update(sf::Time dt) override
	{
		building.setOrigin(0, building.getGlobalBounds().height);
		float deltaX = 0;
		float speed = 300.F;

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

			inAnimation = !building.isAnimationFinished();
			if (!inAnimation)
			{
				building.resetAnim(building.getAnimationKey());
				building.setCurrentAnim("walk");
			}
		}
		building.move(deltaX, 0);

		building.move(0, 100 * dt.asSeconds());
		building.animate(dt);
		ground.animate(dt);
		if (building.doesCollide(ground))
		{
			sf::Vector2f d = building.avoidCollide(ground, sfg::Axis::Y);

			building.move(d.x, d.y);
		}

		building.animate(sf::Time::Zero);
		ground.animate(sf::Time::Zero);
	}

	void render() override
	{
		window().draw(background);
		window().draw(ground);
		window().draw(building);

		building.drawHitboxes(window());
		ground.drawHitboxes(window());
	}
};

int main() {

	Game game("Hitboxes example", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("Hitbox", new HitboxScene());
	game.setCurrentScene("Hitbox");

	game.launch();

	return 0;
}


//#include <SFML/Graphics.hpp>
//#include <sstream>
//#include "Game.h"
//#include "Geometry.h"
//#include "Polygon.h"
//
//using namespace sfg;

/*namespace sfg
{
	class Polygon
	{
	public:
		sf::VertexArray vertices;
		std::vector<sf::Vertex> baseVertices;
		sf::Vector2f pos;
		float angle = 0.0F;

		void update()
		{
			sf::Transform t;
			t.translate(pos);
			t.rotate(angle);

			vertices.clear();
			for (auto& v : baseVertices)
				vertices.append(sf::Vertex(t.transformPoint(v.position), v.color));
		}

		void update(sf::Color c)
		{
			sf::Transform t;
			t.translate(pos);
			t.rotate(angle);

			vertices.clear();
			for (auto& v : baseVertices)
				vertices.append(sf::Vertex(t.transformPoint(v.position), c));
		}

		sf::Vector2f getAxisProjection(int a, int b)
		{
			sf::Transform t;
			t.translate(pos);
			t.rotate(angle);

			sf::Vector2f axisProj;
			sf::Vector2f va = t.transformPoint(baseVertices[a].position);
			sf::Vector2f vb = t.transformPoint(baseVertices[b].position);
			axisProj = {
				-(vb.y - va.y),
				vb.x - va.x
			};
			float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
			axisProj = { axisProj.x / d, axisProj.y / d };

			return axisProj;
		}
	};
}*/

/*class Collision : public Scene
{
	sfg::Polygon rect = sfg::Polygon();
	sfg::Polygon tri = sfg::Polygon();

	float triLength = 50;
	float sqLength = 3200;
	float r = 0.F;

	/*sf::Vector2f doesCollideSAT_Static(const sfg::Polygon& r1, const sfg::Polygon& r2)
	{
		const sfg::Polygon* p1 = &r1;
		const sfg::Polygon* p2 = &r2;

		sf::Vector2f res = { 0, 0 };

		float overlap = INFINITY;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				p1 = &r2;
				p2 = &r1;
			}

			int maxVertex = p1->vertices.getVertexCount();
			if (p1->vertices[0].position == p1->vertices[p1->vertices.getVertexCount() - 1].position)
				maxVertex--;

			for (int a = 0; a < maxVertex; a++)
			{
				int b = (a + 1) % p1->vertices.getVertexCount();

				sf::Vector2f axisProj = {
					-(p1->vertices[b].position.y - p1->vertices[a].position.y),
					p1->vertices[b].position.x - p1->vertices[a].position.x
				};
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < p1->vertices.getVertexCount(); p++)
				{
					float q = (p1->vertices[p].position.x * axisProj.x + p1->vertices[p].position.y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < p2->vertices.getVertexCount(); p++)
				{
					float q = (p2->vertices[p].position.x * axisProj.x + p2->vertices[p].position.y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return { 0,0 };

				// Calculate actual overlap along projected axis, and store the minimum
				overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);
			}
		}
		// If we got here, the objects have collided, we will displace r1
		// by overlap along the vector between the two object centers
		sf::Vector2f d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
		float s = sqrtf(d.x * d.x + d.y * d.y);
		res = {
			-overlap * d.x / s,
			-overlap * d.y / s
		};
		return res;
	}

	bool doesCollideSAT(sfg::Polygon& r1, sfg::Polygon& r2)
	{
		sfg::Polygon* p1 = &r1;
		sfg::Polygon* p2 = &r2;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				p1 = &r2;
				p2 = &r1;
			}

			int maxVertex = p1->vertices.getVertexCount();
			if (p1->vertices[0].position == p1->vertices[p1->vertices.getVertexCount() - 1].position)
				maxVertex--;

			for (int a = 0; a < maxVertex; a++)
			{
				int b = (a + 1) % p1->vertices.getVertexCount();

				sf::Vector2f axisProj = {
					-(p1->vertices[b].position.y - p1->vertices[a].position.y),
					p1->vertices[b].position.x - p1->vertices[a].position.x
				};
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < p1->vertices.getVertexCount(); p++)
				{
					float q = (p1->vertices[p].position.x * axisProj.x + p1->vertices[p].position.y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < p2->vertices.getVertexCount(); p++)
				{
					float q = (p2->vertices[p].position.x * axisProj.x + p2->vertices[p].position.y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		return true;
	}

	bool doesCollideDiags(sfg::Polygon& r1, sfg::Polygon& r2)
	{
		sfg::Polygon* poly1 = &r1;
		sfg::Polygon* poly2 = &r2;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			// Check diagonals of polygon...
			for (int p = 0; p < poly1->vertices.getVertexCount(); p++)
			{
				sf::Vector2f line_r1s = poly1->pos;
				sf::Vector2f line_r1e = poly1->vertices[p].position;

				// ...against edges of the other
				for (int q = 0; q < poly2->vertices.getVertexCount(); q++)
				{
					sf::Vector2f line_r2s = poly2->vertices[q].position;
					sf::Vector2f line_r2e = poly2->vertices[(q + 1) % poly2->vertices.getVertexCount()].position;

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	sf::Vector2f doesCollideDiags_Static(const sfg::Polygon& r1, const sfg::Polygon& r2)
	{
		const sfg::Polygon* poly1 = &r1;
		const sfg::Polygon* poly2 = &r2;

		sf::Vector2f res = { 0, 0 };

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			// Check diagonals of polygon...
			for (int p = 0; p < poly1->vertices.getVertexCount(); p++)
			{
				sf::Vector2f line_r1s = poly1->pos;
				sf::Vector2f line_r1e = poly1->vertices[p].position;

				sf::Vector2f displacement = { 0,0 };

				// ...against edges of the other
				for (int q = 0; q < poly2->vertices.getVertexCount(); q++)
				{
					sf::Vector2f line_r2s = poly2->vertices[q].position;
					sf::Vector2f line_r2e = poly2->vertices[(q + 1) % poly2->vertices.getVertexCount()].position;

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
						displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
					}
				}
				if (displacement.x > 6000)
					std::cout << "ok";
				res.x += displacement.x * (shape == 0 ? -1 : +1);
				res.y += displacement.y * (shape == 0 ? -1 : +1);
			}
		}

		return res;
	}*/

	// H?rit? via Scene
	/*void initialize() override
	{
		rect.pos = { 300, 3700 };
		//rect.vertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);
		rect.baseVertices.push_back(sf::Vector2f(-sqLength, -sqLength));
		rect.baseVertices.push_back(sf::Vector2f(-sqLength, sqLength));
		rect.baseVertices.push_back(sf::Vector2f(sqLength, sqLength));
		rect.baseVertices.push_back(sf::Vector2f(sqLength, -sqLength));
		rect.baseVertices.push_back(sf::Vector2f(-sqLength, -sqLength));

		tri.pos = { 100, 100 };
		//tri.vertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);
		tri.baseVertices.push_back(sf::Vector2f(-triLength, -triLength));
		tri.baseVertices.push_back(sf::Vector2f(-triLength, triLength));

		sf::Vector2f midPoint = (tri.baseVertices[0].position + tri.baseVertices[1].position);
		midPoint = { midPoint.x / 2, midPoint.y / 2 };
		tri.baseVertices.push_back(sf::Vector2f(triLength, midPoint.y));

		tri.baseVertices.push_back(sf::Vector2f(-triLength, -triLength));
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
	}

	void update(sf::Time dt) override
	{
		sf::Vector2f axisProj;

		axisProj = tri.getAxisProjection(0, 1);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			tri.pos -= axisProj * 300.F * dt.asSeconds();

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			tri.pos += axisProj * 300.F * dt.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			tri.angle -= 100 * dt.asSeconds();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			tri.angle += 100 * dt.asSeconds();
		}

		rect.update();
		tri.update();

		sf::Color color = sf::Color::White;
		sf::Vector2f d = sfg::Polygon::doesCollideDiags_Static(tri, rect);
		if (d != sf::Vector2f(0,0))
		{
			color = sf::Color::Red;
			tri.pos += d;
		}

		tri.update(color);

	}

	void render() override
	{
		rect.draw(window());
		tri.draw(window());
	}
};

int main() {

	Game game("Collision detection", sf::VideoMode(1300, 900), sf::Style::Default);

	game.addScene("Collision", new Collision());
	game.setCurrentScene("Collision");

	game.launch();

	return 0;
}*/