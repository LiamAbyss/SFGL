#ifndef INCLUDE_GEOMETRY
#define INCLUDE_GEOMETRY

#include <SFML/Graphics.hpp>
#include <cmath>

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_PI_180 0.017453292519943295769
#define M_180_PI 57.295779513082320876798
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.707106781186547524401

namespace sfg
{
	float getOrientation(const sf::Vector2f& from, const sf::Vector2f& to);

	float toDegrees(float radians);

	float toRadians(float degrees);

	float distance2(const sf::Vector2f& a, const sf::Vector2f& b);

	float distance(const sf::Vector2f& a, const sf::Vector2f& b);
}
#endif // !INCLUDE_GEOMETRY
