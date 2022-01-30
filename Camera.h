#ifndef INCLUDE_CAMERA
#define INCLUDE_CAMERA

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

namespace sfg
{
    class Camera : public sf::View
    {
    private:
        /**
         * \private
         * Duration of the camera's shake.
         */
        sf::Time time = sf::Time::Zero;

        /**
         * \private
         * Intensity of the camera's shake.
         */
        float intensity = 0.0F;

        /**
         * \private
         * Last position of the camera.
         */
        sf::Vector2f lastCenter;

        /**
         * \private
         * New position of the camera.
         */
        sf::Vector2f newCenter;

        /**
         * \private
         * Window of the camera.
         */
        sf::RenderWindow* window = nullptr;

        float shakeFreqX;
        float shakeFreqY;
        float shakeFreqY2;
        float shakeSizeX;
        float shakeSizeY;
        float shakeSizeY2;

        void processShaking(float offsetX, float offsetY);

    public:

        void setCenterX(float x);
        void setCenterY(float y);
        void setCenter(float x, float y);
        void setCenter(const sf::Vector2f& center);
        void move(float offsetX, float offsetY);
        void move(const sf::Vector2f& offset);

        /**
         * \public
         * \return A reference on the game window
         */
        sf::RenderWindow& getWindow();

        /**
         * \public
         * \param w The game window
         */
        void setWindow(sf::RenderWindow& w);

        /**
         * \public
         * Shakes the camera according to the given parameters
         * \param shakeIntensity The intensity of the shaking
         * \param shakeTime The duration of the shaking
         */
        void shake(float shakeIntensity, sf::Time shakeTime);

        /**
         * \public
         * \param dt The time since the last frame
         */
        void update(sf::Time dt);
    };
}

#endif // !INCLUDE_CAMERA