#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

namespace sfg
{
	class Animation
	{
	private:
		GTexture spritesheet;
		uint32_t framerate;
		sf::Time frameDuration;
		size_t framesInSpritesheet;
		size_t start;
		size_t end;
		size_t currentFrame;

		bool repeat;
		bool flipped;

		sf::Time sinceLastFrame = sf::Time::Zero;

		sf::Sprite* sprite;

		void setTextureRect();

	public:
		uint32_t getFramerate() const;
		void setFramerate(uint32_t framerate);

		size_t getCurrentFrame() const;
		void setCurrentFrame(size_t currentFrame);

		bool getRepeat() const;
		void setRepeat(bool repeat);

		bool isFlipped() const;
		void flip(bool flipped);


		void setSpritesheet();

		void reset();

		void nextFrame(const sf::Time& dt);

		bool isFinished() const;

		float getProgress() const;

		static Animation build(GTexture spritesheet, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, sf::Sprite& sprite, bool repeat = true);
	};
}
