#ifndef INCLUDE_COMBO
#define INCLUDE_COMBO

#include <vector>
#include <SFML/Graphics.hpp>

namespace sfg
{
	class ComboItem
	{
	public:
		sf::Keyboard::Key key;
		sf::Time delay;
		int hold = 0;
	};

	class Combo
	{
	private:
		std::vector<std::vector<ComboItem>> combos;
		std::vector<int> currentItems;
		sf::Clock clk;

		void reset();

	public:
		Combo& then(sf::Keyboard::Key key, sf::Time delay = sf::milliseconds(500));

		Combo& hold(sf::Keyboard::Key key, sf::Time delay = sf::milliseconds(500));

		Combo& unhold(sf::Keyboard::Key key, sf::Time delay = sf::milliseconds(500));

		Combo& orElse();

		bool handle(const sf::Event& ev);
	};

}
#endif // !INCLUDE_COMBO
