#include "Polygon.h"

bool sfg::Polygon::doesCollideSAT(const sfg::Polygon& r1, const sfg::Polygon& r2)
{
	const sfg::Polygon* p1 = &r1;
	const sfg::Polygon* p2 = &r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			p1 = &r2;
			p2 = &r1;
		}

		int maxVertex = p1->vertices.getVertexCount();
		if (p1->vertices[0].position == p1->vertices[maxVertex - 1].position)
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

sf::Vector2f sfg::Polygon::doesCollideSAT_Static(const sfg::Polygon& r1, const sfg::Polygon& r2, Axis axis)
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
	sf::Vector2f d;
	switch (axis)
	{
	case sfg::Axis::X:
		d = { 1, 0 };
		break;
	case sfg::Axis::Y:
		d = { 0, 1 };
		break;
	case sfg::Axis::None:
		d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
	default:
		break;
	}
	float s = sqrtf(d.x * d.x + d.y * d.y);
	res = {
		-overlap * d.x / s,
		-overlap * d.y / s
	};
	return res;
}

sfg::Polygon::Polygon()
{
	vertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);
}

void sfg::Polygon::computeBox()
{
	float minX = -1.F;
	float minY = -1.F;
	float maxX = -1.F;
	float maxY = -1.F;

	for (int i = 0; i < vertices.getVertexCount(); i++)
	{
		const auto& v = vertices[i].position;
		if (v.x > maxX)
			maxX = v.x;
		if (v.x < minX || minX < 0)
			minX = v.x;
		if (v.y > maxY)
			maxY = v.y;
		if (v.y < minY || minY < 0)
			minY = v.y;
	}

	left = minX;
	top = minY;
	width = maxX - minX;
	height = maxY - minY;
}

float sfg::Polygon::getHeight()
{
	return height;
}

float sfg::Polygon::getWidth()
{
	return width;
}

float sfg::Polygon::getLeft()
{
	return left;
}

float sfg::Polygon::getTop()
{
	return top;
}

void sfg::Polygon::update()
{
	sf::Transform t;
	t.translate(pos);
	t.rotate(angle);
	t.scale(scale);

	vertices.clear();
	for (const auto& v : baseVertices)
		vertices.append(sf::Vertex(t.transformPoint(v.position), v.color));
}

void sfg::Polygon::update(sf::Color c)
{
	sf::Transform t;
	t.translate(pos);
	t.rotate(angle);

	vertices.clear();
	for (auto& v : baseVertices)
		vertices.append(sf::Vertex(t.transformPoint(v.position), c));
}

void sfg::Polygon::draw(sf::RenderTarget& target) const
{
	target.draw(vertices);
}

sf::Vector2f sfg::Polygon::getAxisProjection(int a, int b)
{
	sf::Transform t;
	t.translate(pos);
	t.rotate(angle);

	sf::Vector2f axisProj;
	sf::Vector2f va = t.transformPoint(baseVertices[a].position);
	sf::Vector2f vb = t.transformPoint(baseVertices[b].position);

	return getProjAxis(va, vb);
}
