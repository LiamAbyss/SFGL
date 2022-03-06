#include "Hitbox.h"

void sfg::Hitbox::setSprite(sf::Sprite* sprite)
{
	this->sprite = sprite;
}

void sfg::Hitbox::flip(bool state)
{
	if (flipped == state) return;
	float centerX = sprite->getGlobalBounds().width / 2;

	for (auto& v : baseVertices)
		v.position.x += 2 * (centerX - v.position.x);

	//pos.x += sprite->getGlobalBounds().width - pos.x - getWidth();
	//scale.x *= -1;
	flipped = state;
}

bool sfg::Hitbox::doesCollide(const Hitbox& h1, const Hitbox& h2)
{
	return doesCollideSAT(h1, h2);
}

sf::Vector2f sfg::Hitbox::avoidCollide(const Hitbox& h1, const Hitbox& h2, sfg::Axis axis)
{
	return doesCollideSAT_Static(h1, h2, axis);
}