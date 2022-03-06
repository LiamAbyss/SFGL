#include "Sprite.h"

void sfg::Sprite::init(Scene* scene)
{
	this->resources = &scene->resources();
	this->config = &scene->config();
}

void sfg::Sprite::createAnim(const std::string& key, const std::string& resourceName, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, bool repeat)
{
	if (resources == nullptr || config == nullptr)
		throw std::runtime_error("Didn't call sfg::Sprite::init() before sfg::Sprite::createAnim()");

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

void sfg::Sprite::loadHitboxes(const std::string& key, const std::string& configName)
{
	assertKey(key);

	nlohmann::json hitboxConfig = config->getConfig(configName)["entity"];

	hitboxes[key] = std::vector<std::vector<Hitbox>>();

	for (const auto& hConfig : hitboxConfig["frames"])
	{
		hitboxes[key].emplace_back(std::vector<Hitbox>());
		auto& frame = hitboxes[key][hitboxes[key].size() - 1];
		for (const auto& box : hConfig)
		{
			Hitbox h;
			h.setSprite(this);
			auto& vertices = h.baseVertices;

			h.type = box["type"] == "hitbox" ? Hitbox::Type::HITBOX : Hitbox::Type::HURTBOX;
			for (const auto& v : box["vertices"])
			{
				vertices.emplace_back(
					sf::Vector2f(v["x"], v["y"]),
					h.type == Hitbox::Type::HURTBOX ? sf::Color::Red : sf::Color::Blue
				);
			}
			h.pos = getPosition();
			h.angle = getRotation();
			h.scale = getScale();
			h.update();
			h.computeBox();
			frame.emplace_back(h);
		}
	}
}

void sfg::Sprite::drawHitboxes(sf::RenderTarget& target)
{
	if (hitboxes.find(currentAnim) != hitboxes.end())
	{
		const auto& activeHitboxes = hitboxes[currentAnim][anims[currentAnim].getCurrentFrame()];

		for (const auto& h : activeHitboxes)
		{
			h.draw(target);
		}
	}
}

void sfg::Sprite::flip(bool orientation)
{
	if (flipped == orientation) return;

	flipped = orientation;
	anims[currentAnim].flip(orientation);

	for (auto& [hKey, hAnim] : hitboxes)
	{
		for (auto& hFrame : hAnim)
		{
			for (auto& h : hFrame) h.flip(orientation);
		}
	}
}

void sfg::Sprite::animate(const sf::Time& dt)
{
	anims[currentAnim].nextFrame(dt);
	if (hitboxes.find(currentAnim) != hitboxes.end())
	{
		auto& activeHitboxes = hitboxes[currentAnim][anims[currentAnim].getCurrentFrame()];
		for (auto& h : activeHitboxes)
		{
			//h.flip(flipped);
			h.pos = getPosition() - getOrigin();
			h.angle = getRotation();
			h.scale = getScale();
			h.update();
		}
	}
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
		{
			setCurrentAnim(anim["name"]);
			didSetCurrentAnim = true;
		}
	}

	// Load hitboxes
	nlohmann::json boxes = conf["hitboxes"];

	for (const auto& box : boxes)
	{
		config->loadConfig(box["config"], box["path"]);
		loadHitboxes(box["name"], box["config"]);
	}
}

bool sfg::Sprite::doesCollide(sfg::Sprite& sprite)
{
	const auto& activeHitboxes2 = sprite.hitboxes[sprite.currentAnim][sprite.anims[sprite.currentAnim].getCurrentFrame()];
	if (hitboxes.find(currentAnim) != hitboxes.end())
	{
		const auto& activeHitboxes = hitboxes[currentAnim][anims[currentAnim].getCurrentFrame()];
		for (const auto& h : activeHitboxes)
		{
			for (const auto& h2 : activeHitboxes2)
			{
				if (Hitbox::doesCollide(h, h2))
					return true;
			}
		}
	}
	return false;
}

sf::Vector2f sfg::Sprite::avoidCollide(sfg::Sprite& sprite, sfg::Axis axis)
{
	const auto& activeHitboxes2 = sprite.hitboxes[sprite.currentAnim][sprite.anims[sprite.currentAnim].getCurrentFrame()];
	if (hitboxes.find(currentAnim) != hitboxes.end())
	{
		float maxX = 0.F;
		float maxY = 0.F;
		const auto& activeHitboxes = hitboxes[currentAnim][anims[currentAnim].getCurrentFrame()];
		for (const auto& h : activeHitboxes)
		{
			for (const auto& h2 : activeHitboxes2)
			{
				sf::Vector2f d = Hitbox::avoidCollide(h, h2, (sfg::Axis)axis);
				if (std::abs(d.x) > std::abs(maxX))
					maxX = d.x;
				if (std::abs(d.y) > std::abs(maxY))
					maxY = d.y;
			}
		}
		return { maxX, maxY };
	}
	return { 0, 0 };
}

