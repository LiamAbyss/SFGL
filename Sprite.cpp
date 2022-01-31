#include "Sprite.h"

void sfg::Sprite::init(Scene* scene)
{
	this->resources = &scene->resources();
	this->config = &scene->config();
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

bool sfg::Sprite::loadFromConfig(const std::string& key)
{
	nlohmann::json conf = config->getConfig(key);

	std::string name = conf["name"];

	// Load resources
	nlohmann::json res = conf["resources"];

	for (const auto& r : res)
	{
		resources->load(r["name"], r["path"]);
	}

	// Load anims
	nlohmann::json animations = conf["animations"];

	bool didSetCurrentAnim = false;

	for (const auto& anim : animations)
	{
		createAnim(
			anim["name"],
			anim["resource"],
			anim["framesInSpritesheet"],
			anim["startFrame"],
			anim["endFrame"],
			anim["framerate"],
			anim["loop"]
		);

		if (anim["active"] && didSetCurrentAnim)
			throw std::runtime_error("Several animations have been initialized as \"active\" : " + currentAnim + " and " + (std::string)anim["name"]);
		else if (anim["active"])
			setCurrentAnim(anim["name"]);
	}
}

