#ifndef INCLUDE_SF_UTILS
#define INCLUDE_SF_UTILS

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

namespace sfg
{
    static class SFUtils
    {
    public:
        static std::string getKeyName(const sf::Keyboard::Key& key);

        static std::vector<std::string> split(std::string str, const std::string& delimiter);
        
        static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    };
}
#endif // !INCLUDE_SF_UTILS
