#ifndef INCLUDE_POLYGON
#define INCLUDE_POLYGON

#include <SFML/Graphics.hpp>
#include "Geometry.h"
#include "Axis.h"

namespace sfg
{	
	class Polygon
	{
	protected:
		sf::VertexArray vertices;
		float width = 0.F;
		float height = 0.F;
		float left = 0.F;
		float top = 0.F;

	public:
		static bool doesCollideSAT(const sfg::Polygon& r1, const sfg::Polygon& r2);

		static sf::Vector2f doesCollideSAT_Static(const sfg::Polygon& r1, const sfg::Polygon& r2, Axis axis = Axis::None);

		std::vector<sf::Vertex> baseVertices;
		sf::Vector2f pos;
		sf::Vector2f scale = { 1, 1 };
		float angle = 0.0F;

		Polygon();

		void computeBox();

		float getHeight();

		float getWidth();

		float getLeft();

		float getTop();

		void update();

		void update(sf::Color c);

		void draw(sf::RenderTarget& target) const;

		sf::Vector2f getAxisProjection(int a, int b);
	};
}

#endif // !INCLUDE_POLYGON
