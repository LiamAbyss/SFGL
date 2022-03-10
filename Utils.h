#ifndef INCLUDE_SF_UTILS
#define INCLUDE_SF_UTILS

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

namespace sfg
{
    namespace utils
    {
        namespace sfml
        {
            std::string getKeyName(const sf::Keyboard::Key& key);
        }

        namespace strings
        {
            std::vector<std::string> split(std::string str, const std::string& delimiter);
        
            std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

            std::string toUpper(const std::string& str);

            std::string toLower(const std::string& str);

            std::string strip(const std::string& str);
        }

        namespace maps
        {
            template<typename TK, typename TV>
            std::vector<TK> extractKeys(std::map<TK, TV> const& inputMap);

            template<typename TK, typename TV>
            std::vector<TV> extractValues(std::map<TK, TV> const& inputMap);

            template<typename TK, typename TV>
            std::map<TK, TV> zip(std::vector<TK> const& keys, std::vector<TV> const& values);

            template<typename TK, typename TV>
            std::pair<std::vector<TK>, std::vector<TV>> unzip(std::map<TK, TV> const& inputMap);
        }
    };
}
#endif // !INCLUDE_SF_UTILS
