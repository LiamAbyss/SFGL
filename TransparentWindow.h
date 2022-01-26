#ifndef INCLUDE_TRANSPARENT_WINDOW
#define INCLUDE_TRANSPARENT_WINDOW
#include <SFML/Graphics.hpp>

#if defined (SFML_SYSTEM_WINDOWS)
#include <Windows.h>

/**
* Sets the shape of the window according to an image with transparent parts
* \param hWnd The window handle
* \param image The image to shape the window
*/
bool setWShape(HWND hWnd, const sf::Image& image);

/** 
 * Sets the transparency of the window
 * \param hWnd The window handle
 * \param alpha the opacity (0-255)
 */
bool setWTransparency(HWND hWnd, unsigned char alpha);

#elif defined (SFML_SYSTEM_LINUX)
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>

bool setWShape(Window wnd, const sf::Image& image);

bool setWTransparency(Window wnd, unsigned char alpha);

#undef None // None conflicts with SFML
#elif defined (SFML_SYSTEM_MACOS)
bool setWShape(sf::WindowHandle handle, const sf::Image& image);
bool setWTransparency(sf::WindowHandle handle, unsigned char alpha);
#else
bool setWShape(sf::WindowHandle handle, const sf::Image& image);

bool setWTransparency(sf::WindowHandle handle, unsigned char alpha);
#endif

#endif // !INCLUDE_TRANSPARENT_WINDOW