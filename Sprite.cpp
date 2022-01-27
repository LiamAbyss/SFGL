#include "Sprite.h"

void sfg::Sprite::init(ResourceManager& resources)
{
	this->resources = &resources;
}

void sfg::Sprite::createAnim(const std::string& key, const std::string& resourceName, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, bool repeat)
{
	Animation a = Animation::build(
		resources->getTexture(resourceName),
		framesInSpritesheet,
		start, end,
		framerate,
		*this,
		repeat
	);

	anims[key] = a;
}

void sfg::Sprite::resetAnim(const std::string& key)
{
	assertKey(key);
	anims[key].reset();
}

void sfg::Sprite::setCurrentAnim(const std::string& key)
{
	assertKey(key);
	currentAnim = key;
	anims[key].flip(flipped);
	anims[key].setSpritesheet();
}

void sfg::Sprite::flip(bool orientation)
{
	flipped = orientation;
	anims[currentAnim].flip(orientation);
}

void sfg::Sprite::animate(const sf::Time& dt)
{
	anims[currentAnim].nextFrame(dt);
}

bool sfg::Sprite::isAnimationFinished() const
{
	return anims.at(currentAnim).isFinished();
}

void sfg::Sprite::setAnimationRepeat(const std::string& key, bool repeat)
{
	assertKey(key);
	anims[currentAnim].setRepeat(repeat);
}

bool sfg::Sprite::getAnimationRepeat(const std::string& key)
{
	assertKey(key);
	return anims[currentAnim].getRepeat();
}

std::string sfg::Sprite::getAnimationKey() const
{
	return currentAnim;
}

float sfg::Sprite::getAnimationProgress() const
{
	return anims.at(currentAnim).getProgress();
}

void sfg::Sprite::assertKey(const std::string& key)
{
	if (anims.find(key) == anims.end())
	{
		std::stringstream error;
		error << "Animation '" << key << "' was not found in the sprite.";
		throw std::runtime_error(error.str());
	}
}

bool sfg::Sprite::isFlipped() const
{
    return flipped;
}

