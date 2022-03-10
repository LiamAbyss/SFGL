#include "Utils.h"

/**
 * Returns the name of the key passed as parameter.
 */
std::string sfg::utils::sfml::getKeyName(const sf::Keyboard::Key& key)
{
    // 0-25 are letters
    if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
        return std::string(1, static_cast<char>(key + 'A'));

    // 26-35 are NumX
    std::string res = "Num";
    if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
        return std::string(res + std::to_string(key - sf::Keyboard::Num0));

    // From numpad 0 to numpad 9
    res = "Numpad";
    if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
        return std::string(res + std::to_string(key - sf::Keyboard::Numpad0));
    
    // From F1 to F15
    res = "F";
    if (key >= sf::Keyboard::F1 && key <= sf::Keyboard::F15)
        return std::string(res + std::to_string(key - sf::Keyboard::F1));

    // Remaining keys
    switch (key)
    {
        case sf::Keyboard::Escape:
            return "Escape";
        case sf::Keyboard::LControl:
            return "LControl";
        case sf::Keyboard::LShift:
            return "LShift";
        case sf::Keyboard::LAlt:
            return "LAlt";
        case sf::Keyboard::LSystem:
            return "LSystem";
        case sf::Keyboard::RControl:
            return "RControl";
        case sf::Keyboard::RShift:
            return "RShift";
        case sf::Keyboard::RAlt:
            return "RAlt";
        case sf::Keyboard::RSystem:
           return "RSystem";
        case sf::Keyboard::Menu:
            return "Menu";
        case sf::Keyboard::LBracket:
            return "LBracket";
        case sf::Keyboard::RBracket:
            return "RBracket";
        case sf::Keyboard::SemiColon:
            return "SemiColon";
        case sf::Keyboard::Comma:
            return "Comma";
        case sf::Keyboard::Period:
            return "Period";
        case sf::Keyboard::Quote:
            return "Quote";
        case sf::Keyboard::Slash:
            return "Slash";
        case sf::Keyboard::BackSlash:
            return "BackSlash";
        case sf::Keyboard::Tilde:
            return "Tilde";
        case sf::Keyboard::Equal:
            return "Equal";
        case sf::Keyboard::Hyphen:
            return "Hyphen";
        case sf::Keyboard::Space:
            return "Space";
        case sf::Keyboard::Enter:
            return "Enter";
        case sf::Keyboard::BackSpace:
            return "BackSpace";
        case sf::Keyboard::Tab:
            return "Tab";
        case sf::Keyboard::PageUp:
            return "PageUp";
        case sf::Keyboard::PageDown:
            return "PageDown";
        case sf::Keyboard::End:
            return "End";
        case sf::Keyboard::Home:
            return "Home";
        case sf::Keyboard::Insert:
            return "Insert";
        case sf::Keyboard::Delete:
            return "Delete";
        case sf::Keyboard::Add:
            return "Add";
        case sf::Keyboard::Subtract:
            return "Subtract";
        case sf::Keyboard::Multiply:
            return "Multiply";
        case sf::Keyboard::Divide:
            return "Divide";
        case sf::Keyboard::Left:
            return "Left";
        case sf::Keyboard::Right:
            return "Right";
        case sf::Keyboard::Up:
            return "Up";
        case sf::Keyboard::Down:
            return "Down";
        case sf::Keyboard::Pause:
            return "Pause";
        case sf::Keyboard::Unknown:
        default:
            return "Unknown";
    }
}

std::vector<std::string> sfg::utils::strings::split(std::string str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    return tokens;
}

std::string sfg::utils::strings::join(const std::vector<std::string>& strings, const std::string& delimiter)
{
    std::stringstream ss;

    for (size_t i = 0; i < strings.size(); i++)
    {
        ss << strings[i];
        if (i != strings.size() - 1) {
            ss << delimiter;
        }
    }
    return ss.str();
}

std::string sfg::utils::strings::toUpper(const std::string& str)
{
    std::stringstream upper;
    for (const auto& item : str)
    {
        upper << toupper(item);
    }
    return upper.str();
}

std::string sfg::utils::strings::toLower(const std::string& str)
{
    std::stringstream lower;
    for (const auto& item : str)
    {
        lower << tolower(item);
    }
    return lower.str();
}

std::string sfg::utils::strings::strip(const std::string& str)
{
    auto start_it = str.begin();
    auto end_it = str.rbegin();

    while (std::isspace(*start_it) != 0)
        ++start_it;
    while (std::isspace(*end_it) != 0)
        ++end_it;

    return std::string(start_it, end_it.base());
}

template<typename TK, typename TV>
std::vector<TK> sfg::utils::maps::extractKeys(std::map<TK, TV> const& inputMap)
{
    std::vector<TK> retval;
    retval.reserve(inputMap.size());
    for (auto const& element : inputMap)
    {
        retval.emplace_back(element.first);
    }
    return retval;
}

template<typename TK, typename TV>
std::vector<TV> sfg::utils::maps::extractValues(std::map<TK, TV> const& inputMap)
{
    std::vector<TV> retval;
    retval.reserve(inputMap.size());
    for (auto const& element : inputMap)
    {
        retval.emplace_back(element.second);
    }
    return retval;
}

template<typename TK, typename TV>
std::map<TK, TV> sfg::utils::maps::zip(std::vector<TK> const& keys, std::vector<TV> const& values)
{
    if (keys.size() != values.size())
        throw std::runtime_error("Cannot zip different keys and values sizes : shape (" +
            std::to_string(keys.size()) + ") incompatible with shape (" +
            std::to_string(values.size()) + ")");

    std::map<TK, TV> resultMap;
    
    for (int i = 0; i < keys.size(); i++)
    {
        resultMap.try_emplace(keys[i], values[i]);
    }

    return resultMap;
}

template<typename TK, typename TV>
std::pair<std::vector<TK>, std::vector<TV>> sfg::utils::maps::unzip(std::map<TK, TV> const& inputMap)
{
    return std::make_pair(extractKeys(inputMap), extractValues(inputMap));
}
