#pragma once
#include "Curve.h"

class LinearCurve : public Curve
{
public:

	sf::Vector2f getPoint(float t) override;

	sf::Vector2f getGradient(float t) override;

	void setClosed(bool closed) override;

	float getMaxT() override;

	void setControlPoints(const std::vector<sf::Vector2f>& points) override;
};

