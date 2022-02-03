#include "SFUtils.h"

/**
 * Returns the name of the key passed as parameter.
 */
std::string sfg::SFUtils::getKeyName(const sf::Keyboard::Key& key)
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
