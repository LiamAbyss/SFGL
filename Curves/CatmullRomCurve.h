#pragma once
#include "Curve.h"

class CatmullRomCurve : public Curve
{

private:
	std::vector<sf::Vector2f> inputPoints;
public:

	std::vector<sf::Vector2f> getControlPoints() const;

	void setControlPoints(const std::vector<sf::Vector2f>& points) override;

	float getMaxT() override;

	sf::Vector2f getPoint(float t) override;

	sf::Vector2f getGradient(float t) override;

	void setClosed(bool closed) override;
};

