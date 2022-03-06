#include "Geometry.h"

float sfg::getOrientation(const sf::Vector2f& from, const sf::Vector2f& to)
{
	sf::Vector2f gradient = to - from;
	return -atan2(-gradient.y, gradient.x);
}

sf::Vector2f sfg::getProjAxis(const sf::Vector2f& a, const sf::Vector2f& b)
{
	sf::Vector2f axisProj;
	axisProj = {
		-(b.y - a.y),
		b.x - a.x
	};
	float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
	axisProj = { axisProj.x / d, axisProj.y / d };

	return axisProj;
}

float sfg::toDegrees(float radians)
{
	return radians * static_cast<float>(M_180_PI);
}

float sfg::toRadians(float degrees)
{
	return degrees * static_cast<float>(M_PI_180);
}

float sfg::distance2(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

float sfg::distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return std::sqrtf(distance2(a, b));
}
