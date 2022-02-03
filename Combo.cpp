#include "Combo.h"

void sfg::Combo::reset()
{
	for (auto& c : currentItems)
		c = -1;
	clk.restart();
}

sfg::Combo& sfg::Combo::then(sf::Keyboard::Key key, sf::Time delay)
{
	ComboItem item;
	item.key = key;
	item.delay = delay;
	item.hold = 0;
	if (combos.empty())
	{
		combos.emplace_back(std::vector<ComboItem>());
		currentItems.emplace_back(-1);
	}
	combos[combos.size() - 1].push_back(item);
	return *this;
}

sfg::Combo& sfg::Combo::hold(sf::Keyboard::Key key, sf::Time delay)
{
	ComboItem item;
	item.key = key;
	item.delay = delay;
	item.hold = 1;
	if (combos.empty())
	{
		combos.emplace_back(std::vector<ComboItem>());
		currentItems.emplace_back(-1);
	}
	combos[combos.size() - 1].push_back(item);
	return *this;
}

sfg::Combo& sfg::Combo::unhold(sf::Keyboard::Key key, sf::Time delay)
{
	ComboItem item;
	item.key = key;
	item.delay = delay;
	item.hold = -1;
	if (combos.empty())
	{
		combos.emplace_back(std::vector<ComboItem>());
		currentItems.emplace_back(-1);
	}
	combos[combos.size() - 1].push_back(item);
	return *this;
}

sfg::Combo& sfg::Combo::orElse()
{
	combos.emplace_back(std::vector<ComboItem>());
	currentItems.emplace_back(-1);
	return *this;
}

bool sfg::Combo::handle(const sf::Event& ev)
{
	for (int i = 0; i < combos.size(); i++)
	{
		const auto& items = combos[i];
		auto& currentItem = currentItems[i];
		if (currentItem >= 0 && items[currentItem].delay < clk.getElapsedTime())
		{
			// Reset combo
			currentItem = -1;
			continue;
		}

		if (!(ev.type == sf::Event::KeyPressed || ev.type == sf::Event::KeyReleased))
			continue;

		if (items[currentItem + 1].key == ev.key.code)
		{
			if (items[currentItem + 1].hold == 0 && ev.type == sf::Event::KeyPressed)
				continue;

			/*if (currentItem + 2 < items.size())
			{
				if (items[currentItem + 1].hold == 1)
				{
					std::cout << "Hold : waiting " << items[currentItem + 2].key << std::endl;
				}
				if (items[currentItem + 1].hold == 0)
				{
					std::cout << "Pressed : waiting " << items[currentItem + 2].key << std::endl;
				}
				if (items[currentItem + 1].hold == -1)
				{
					std::cout << "Unhold : waiting " << items[currentItem + 2].key << std::endl;
				}
			}
			else
			{
				if (items[currentItem + 1].hold == 1)
				{
					std::cout << "Hold" << std::endl;
				}
				if (items[currentItem + 1].hold == 0)
				{
					std::cout << "Pressed" << std::endl;
				}
				if (items[currentItem + 1].hold == -1)
				{
					std::cout << "Unhold" << std::endl;
				}
			}*/
			if (
				items[currentItem + 1].hold <= 0 && ev.type == sf::Event::KeyReleased ||
				items[currentItem + 1].hold == 1 && ev.type == sf::Event::KeyPressed
				)
			{
				// Next item of combo
				currentItem++;
				if (currentItem + 1 >= items.size())
				{
					// Complete combo
					currentItem = -1;
					reset();
					return true;
				}
				clk.restart();
			}
			else
			{
				// Reset combo
				currentItem = -1;
			}
			continue;
		}
		else
		{
			if (currentItem != 0 || ev.type == sf::Event::KeyPressed)
				// Reset combo
				currentItem = -1;
		}
	}
	return false;
}
