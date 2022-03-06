#ifndef INCLUDE_SPRITE
#define INCLUDE_SPRITE
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Config.h"
#include "Animation.h"
#include "Scene.h"
#include "Hitbox.h"

// TODO : MET TOUT BIEN

namespace sfg
{
	class Hitbox;

	class Sprite : public sf::Sprite
	{
	private:
		std::map<std::string, Animation, std::less<>> anims;
		std::map<std::string, std::vector<std::vector<Hitbox>>, std::less<>> hitboxes;
		std::string currentAnim;
		bool flipped = false;

		ResourceManager* resources;
		Config* config;

	public:

		void init(Scene* scene);

		void createAnim(const std::string& key, const std::string& resourceName, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, bool repeat = true);

		void resetAnim(const std::string& key);

		void setCurrentAnim(const std::string& key);

		void loadHitboxes(const std::string& key, const std::string& configName);

		void drawHitboxes(sf::RenderTarget& target);

		void flip(bool orientation);

		void animate(const sf::Time& dt);

		bool isAnimationFinished() const;

		void setAnimationRepeat(const std::string& key, bool repeat);
		bool getAnimationRepeat(const std::string& key);

		std::string getAnimationKey() const;

		float getAnimationProgress() const;

		void assertKey(const std::string& key);

		bool isFlipped() const;

		bool loadFromConfig(const std::string& key);

		bool doesCollide(sfg::Sprite& sprite);

		sf::Vector2f avoidCollide(sfg::Sprite& sprite, sfg::Axis axis = sfg::Axis::None);
	};

}
#endif // !INCLUDE_SPRITE
