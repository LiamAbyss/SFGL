#ifndef INCLUDE_LINEAR_CURVE
#define INCLUDE_LINEAR_CURVE
#include "Curve.h"

namespace sfg
{
	class LinearCurve : public Curve
	{
	public:

		sf::Vector2f getPoint(float t) override;

		sf::Vector2f getGradient(float t) override;

		void setClosed(bool closed) override;

		float getMaxT() override;

		void setControlPoints(const std::vector<sf::Vector2f>& points) override;
	};
}


#endif // !INCLUDE_LINEAR_CURVE
