#include "Curve.h"

namespace sfg
{
	bool Curve::isClosed()
	{
		return closed_;
	}

	float Curve::getCurveLength()
	{
		return totalLength_;
	}

	void Curve::computeCurveLength(float step)
	{
		totalLength_ = 0.0F;
		float maxT = getMaxT();
		segmentLengths_.resize(static_cast<int>(maxT));
		for (int i = 0; i < maxT; i++)
		{
			totalLength_ += (segmentLengths_[i] = computeSegmentLength(i, step));
		}
	}

	float Curve::computeSegmentLength(int pointIndex, float step)
	{
		float length = 0.0F;

		sf::Vector2f lastPoint, nextPoint;
		lastPoint = getPoint(static_cast<float>(pointIndex));

		for (float t = 0.0F; t < 1.0F; t += step)
		{
			float cursor = static_cast<float>(pointIndex) + t;
			if (cursor == static_cast<float>(pointIndex) + 1.0F) break;

			nextPoint = getPoint(cursor);
			length += std::sqrtf(
				(nextPoint.x - lastPoint.x) * (nextPoint.x - lastPoint.x) +
				(nextPoint.y - lastPoint.y) * (nextPoint.y - lastPoint.y)
			);
			lastPoint = nextPoint;
		}
		return length;
	}

	float Curve::getNormalisedOffset(float p)
	{
		if (controlPoints_.empty()) return 0.0F;
		if (segmentLengths_.empty() || segmentLengths_.size() != static_cast<int>(getMaxT()))
			computeCurveLength();

		int i = 0;
		while (i < segmentLengths_.size() && p > segmentLengths_[i])
		{
			p -= segmentLengths_[i];
			i++;
		}
		return static_cast<float>(i) + (p / segmentLengths_[i]);
	}
}
