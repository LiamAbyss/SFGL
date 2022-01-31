#ifndef INCLUDE_SPRITE
#define INCLUDE_SPRITE
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Config.h"
#include "Animation.h"
#include "Scene.h"

namespace sfg
{

	class Sprite : public sf::Sprite
	{
	private:
		std::map<std::string, Animation, std::less<>> anims;
		std::string currentAnim;
		bool flipped = false;

		ResourceManager* resources;
		Config* config;

	public:

		void init(Scene* scene);

		void createAnim(const std::string& key, const std::string& resourceName, size_t framesInSpritesheet, size_t start, size_t end, uint32_t framerate, bool repeat = true);

		void resetAnim(const std::string& key);

		void setCurrentAnim(const std::string& key);

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
	};

}
#endif // !INCLUDE_SPRITE
