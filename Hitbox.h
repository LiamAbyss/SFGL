#ifndef INCLUDE_HITBOX
#define INCLUDE_HITBOX

#include "Polygon.h"
#include "Sprite.h"

namespace sfg
{
	class Sprite;

	class Hitbox : public Polygon
	{
	private:
		bool flipped = false;
		sf::Sprite* sprite;

	public:

		void setSprite(sf::Sprite* sprite);

		void flip(bool state);

		enum class Type
		{
			HITBOX = 0,
			HURTBOX = 1
		};

		Type type = Type::HITBOX;

		static bool doesCollide(const Hitbox& h1, const Hitbox& h2);

		static sf::Vector2f avoidCollide(const Hitbox& h1, const Hitbox& h2, Axis axis = Axis::None);
	};


}
#endif // !INCLUDE_HITBOX
