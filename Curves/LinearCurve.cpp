#include "LinearCurve.h"

namespace sfg
{
	sf::Vector2f LinearCurve::getPoint(float t)
	{
		int p1;
		int p0;
		p1 = static_cast<int>(t) + 1;
		p0 = p1 - 1;
		if (p1 >= controlPoints_.size())
			p1 = controlPoints_.size() - 1;

		t = t - static_cast<float>(static_cast<int>(t));
		sf::Vector2f res;
		res.x = controlPoints_[p0].x + (controlPoints_[p1].x - controlPoints_[p0].x) * t;
		res.y = controlPoints_[p0].y + (controlPoints_[p1].y - controlPoints_[p0].y) * t;

		return res;
	}

	sf::Vector2f LinearCurve::getGradient(float t)
	{
		int p1;
		int p0;
		p1 = static_cast<int>(t) + 1;
		p0 = p1 - 1;
		if (p1 >= controlPoints_.size())
			p1 = controlPoints_.size() - 1;

		sf::Vector2f res;

		// (x * (a - b) + b)' = a - b
		res.x = controlPoints_[p1].x - controlPoints_[p0].x;
		res.y = controlPoints_[p1].y - controlPoints_[p0].y;

		return res;
	}

	void LinearCurve::setClosed(bool closed)
	{
		if (closed == closed_) return;
		if (closed_)
			controlPoints_.pop_back();
		else
			controlPoints_.push_back(controlPoints_[0]);
		closed_ = closed;
	}

	float LinearCurve::getMaxT()
	{
		return static_cast<float>(controlPoints_.size());
	}

	void LinearCurve::setControlPoints(const std::vector<sf::Vector2f>& points)
	{
		controlPoints_ = points;
		if (closed_)
			controlPoints_.push_back(controlPoints_[0]);
	}
}
