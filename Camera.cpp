#include "Camera.h"

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
}

void Camera::update(sf::Time dt)
{
    if (time.asSeconds() > 0)
	{
		float shakeFreqX = rand() % 40 + 30;  
		float shakeFreqY = rand() % 40 + 30;
		float shakeFreqY2 = rand() % 40 + 30;
		float shakeSizeX = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float shakeSizeY = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float shakeSizeY2 = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float t = time.asSeconds();  
		float xAdjustment = sin(t * shakeFreqX) * shakeSizeX;  
		float yAdjustment = sin(t * shakeFreqY) * shakeSizeY + cos(t * shakeFreqY2) * shakeSizeY2;  

		move(xAdjustment, yAdjustment);
		time = sf::microseconds(time.asMicroseconds() - dt.asMicroseconds());
	}
	else if (time.asSeconds() < 0)
	{
		time = sf::Time::Zero;
		setCenter(lastCenter);
	}
}