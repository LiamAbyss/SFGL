#include "CatmullRomCurve.h"

std::vector<sf::Vector2f> CatmullRomCurve::getControlPoints() const
{
	return inputPoints;
}

void CatmullRomCurve::setControlPoints(const std::vector<sf::Vector2f>& points)
{
	// Doesn't work with 18 points ?
	if (points.empty()) return;
	int test = 0;
	if (points.size() == 18)
		test++;
	inputPoints = points;
	if (closed_ && points.size() > 1)
	{
		controlPoints_.clear();
		controlPoints_.push_back(points[points.size() - 1]);
		for (const auto& p : points)
			controlPoints_.push_back(p);
		controlPoints_.push_back(points[0]);
		controlPoints_.push_back(points[1]);
	}
	else
	{
		controlPoints_.clear();
		controlPoints_.push_back(points[0]);
		for (const auto& p : points)
			controlPoints_.push_back(p);
		controlPoints_.push_back(points[points.size() - 1]);
	}
	computeCurveLength();
}

float CatmullRomCurve::getMaxT()
{
	return (float)controlPoints_.size() - 3;
}

sf::Vector2f CatmullRomCurve::getPoint(float t)
{
	if (controlPoints_.size() < 2)
		throw std::runtime_error("A curve should have at least 2 control points");

	int p0, p1, p2, p3;
	if (!closed_)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
	}
	else
	{
		p1 = (int)t + 1;
		p2 = (p1 + 1) % controlPoints_.size();
		p3 = (p2 + 1) % controlPoints_.size();
		p0 = p1 >= 1 ? p1 - 1 : controlPoints_.size() - 1;
	}

	t = t - static_cast<float>(static_cast<int>(t));

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.0F * tt - t;
	float q2 = 3.0F * ttt - 5.0F * tt + 2.0F;
	float q3 = -3.0F * ttt + 4.0F * tt + t;
	float q4 = ttt - tt;

	float tx = 0.5F * (controlPoints_[p0].x * q1 + controlPoints_[p1].x * q2 + controlPoints_[p2].x * q3 + controlPoints_[p3].x * q4);
	float ty = 0.5F * (controlPoints_[p0].y * q1 + controlPoints_[p1].y * q2 + controlPoints_[p2].y * q3 + controlPoints_[p3].y * q4);

	return{ tx, ty };
}

sf::Vector2f CatmullRomCurve::getGradient(float t)
{
	int p0;
	int p1;
	int p2;
	int p3;

	if (!closed_)
	{
		p1 = (int)t + 1;
		p2 = p1 + 1;
		p3 = p2 + 1;
		p0 = p1 - 1;
	}
	else
	{
		p1 = (int)t + 1;
		p2 = (p1 + 1) % controlPoints_.size();
		p3 = (p2 + 1) % controlPoints_.size();
		p0 = p1 >= 1 ? p1 - 1 : controlPoints_.size() - 1;
	}

	t = t - static_cast<float>(static_cast<int>(t));

	float tt = t * t;

	float q1 = -3.0F * tt + 4.0F * t - 1;
	float q2 = 9.0F * tt - 10.0F * t;
	float q3 = -9.0F * tt + 8.0F * t + 1.0F;
	float q4 = 3.0F * tt - 2.0F * t;

	float tx = 0.5F * (controlPoints_[p0].x * q1 + controlPoints_[p1].x * q2 + controlPoints_[p2].x * q3 + controlPoints_[p3].x * q4);
	float ty = 0.5F * (controlPoints_[p0].y * q1 + controlPoints_[p1].y * q2 + controlPoints_[p2].y * q3 + controlPoints_[p3].y * q4);

	return{ tx, ty };
}

void CatmullRomCurve::setClosed(bool closed)
{
	closed_ = closed;
	setControlPoints(inputPoints);
}
