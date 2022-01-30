#include "Camera.h"

namespace sfg
{
	void Camera::processShaking(float offsetX, float offsetY)
	{
		sf::View::move(offsetX, offsetY);
	}

	void Camera::setCenterX(float x)
	{
		lastCenter.x = x;
		sf::View::setCenter(x, sf::View::getCenter().y);
	}

	void Camera::setCenterY(float y)
	{
		lastCenter.y = y;
		sf::View::setCenter(sf::View::getCenter().x, y);
	}

	void Camera::setCenter(float x, float y)
	{
		lastCenter = sf::Vector2f(x, y);
		sf::View::setCenter(x, y);
	}

	void Camera::setCenter(const sf::Vector2f& center)
	{
		lastCenter = center;
		sf::View::setCenter(center);
	}

	void Camera::move(float offsetX, float offsetY)
	{
		lastCenter.x += offsetX;
		lastCenter.y += offsetY;
		sf::View::move(offsetX, offsetY);
	}

	void Camera::move(const sf::Vector2f& offset)
	{
		lastCenter.x += offset.x;
		lastCenter.y += offset.y;
		sf::View::move(offset);
	}

	sf::RenderWindow& Camera::getWindow()
	{
		return static_cast<sf::RenderWindow&>(*window);
	}

	void Camera::setWindow(sf::RenderWindow& w)
	{
		window = &w;
	}

	void Camera::shake(float shakeIntensity, sf::Time shakeTime)
	{
		// If the intensity is null, we don't shake
		if (shakeIntensity == 0.0f)
			return;

		// If the time is greater than 0
		if (this->time > sf::Time::Zero)
		{
			setCenter(lastCenter);
			window->setView(*this);
		}

		// Set the parameters
		intensity = shakeIntensity;
		time = shakeTime;
		lastCenter = window->getView().getCenter();

		shakeFreqX = rand() % 40 + 30;
		shakeFreqY = rand() % 40 + 30;
		shakeFreqY2 = rand() % 40 + 30;
		shakeSizeX = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(intensity)) + 1;//static_cast<int>(ceil(intensity / 2)));// +intensity;
		shakeSizeY = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(intensity)) + 1;//static_cast<int>(ceil(intensity / 2)));// +intensity;
		shakeSizeY2 = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(intensity)) + 1;//static_cast<int>(ceil(intensity / 2)));// +intensity;

	}

	void Camera::update(sf::Time dt)
	{
		if (time.asSeconds() > 0)
		{
			float t = time.asSeconds();
			float xAdjustment = sin(t * shakeFreqX) * shakeSizeX;
			float yAdjustment = sin(t * shakeFreqY) * shakeSizeY + cos(t * shakeFreqY2) * shakeSizeY2;

			processShaking(xAdjustment, yAdjustment);
			time = sf::microseconds(time.asMicroseconds() - dt.asMicroseconds());
		}
		else if (time.asSeconds() < 0)
		{
			time = sf::Time::Zero;
			setCenter(lastCenter);
		}
	}
}