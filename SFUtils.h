#ifndef INCLUDE_SF_UTILS
#define INCLUDE_SF_UTILS

#include <SFML/Graphics.hpp>
#include <string>

namespace sfg
{
    static class SFUtils
    {
    public:
        static std::string getKeyName(const sf::Keyboard::Key& key);
    };
}
#endif // !INCLUDE_SF_UTILS
