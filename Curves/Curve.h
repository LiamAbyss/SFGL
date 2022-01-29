#pragma once
#include <SFML/Graphics.hpp>

class Curve
{
protected:
	bool closed_ = false;
	std::vector<sf::Vector2f> controlPoints_;
	float totalLength_ = 0.0F;
	std::vector<float> segmentLengths_;

public:

	virtual float getMaxT() = 0;
	virtual void setControlPoints(const std::vector<sf::Vector2f>& points) = 0;
	virtual sf::Vector2f getPoint(float t) = 0;
	virtual sf::Vector2f getGradient(float t) = 0;
	virtual void setClosed(bool closed) = 0;

	bool isClosed();

	float getCurveLength();

	void computeCurveLength(float step = 0.005F);

	float computeSegmentLength(int pointIndex, float step = 0.005F);

	float getNormalisedOffset(float p);

	virtual ~Curve() = default;
};

