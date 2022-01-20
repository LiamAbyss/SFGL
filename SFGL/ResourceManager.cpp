#include "ResourceManager.h"

std::string getMimeType(const std::string& extension)
{
    // return mime type based on extension
    HKEY hKey = nullptr;
	std::string result = "application/unknown";

	// open registry key
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, extension.c_str(),
		0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		// define buffer
		char szBuffer[256] = { 0 };
		
		// get content type
		if (
			DWORD dwBuffSize = sizeof(szBuffer); 
			RegQueryValueEx(hKey, "Content Type", nullptr, nullptr, (LPBYTE)szBuffer, &dwBuffSize) == ERROR_SUCCESS
			)
		{
			// success
			result = szBuffer;
		}

		// close key
		RegCloseKey(hKey);
	}

	// return result
	return result;
}

long long getFileSize(const std::string& filePath)
{
	struct stat stat_buf;
	int rc = stat(filePath.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool ResourceManager::load(const std::string& key, const std::string& file)
{
    // If the file has no extension, return false
	if (file.find(".") == std::string::npos) return false;

    // Get the mime type of the file
	std::string type = getMimeType(file.substr(file.find_last_of('.'), file.size() - file.find_last_of('.')));

    // If the file is an image, load it as a texture
	if(type.substr(0, type.find("/")) == "image")
	{
		textureHolder.emplace(key, std::make_shared<sf::Texture>());
		textureHolder[key]->loadFromFile(file);
	}
    // If the file is a json, load it as a json
	else if(type == "application/json")
	{
		std::ifstream jFile(file);
		if(jFile.is_open())
		{
			nlohmann::json j;
			jFile >> j;
			jsonHolder.emplace(key, std::make_shared<nlohmann::json>(j));
			return true;
		}
		else 
		{
			std::cout << "Failed to load json " << file << ". Reason: Unable to open file" << std::endl;
		}
	}
    // If the file is an audio file
	else if(type.substr(0, type.find("/")) == "audio")
	{
		long long size = getFileSize(file);
		if(size < 0)
		{
			std::cout << "Failed to load audio " << file << ". Reason: Unable to open file" << std::endl;
		}
        // If the file is short, load it as a sound buffer
		else if(static_cast<unsigned long long>(size) <= audioLimit)
		{
			soundHolder.emplace(key, std::make_shared<sf::SoundBuffer>());
			soundHolder[key]->loadFromFile(file);
			return true;
		}
        // If the file is long, load it as a music
		else
		{
			musicHolder.emplace(key, std::make_shared<sf::Music>());
			musicHolder[key]->openFromFile(file);
			return true;
		}
	}
    // If the file is a font, load it as a font
	else if(file.substr(file.find_last_of('.'), file.size() - file.find_last_of('.')) == ".ttf")
	{
		fontHolder.emplace(key, std::make_shared<sf::Font>());
		fontHolder[key]->loadFromFile(file);
		return true;
	}

	return false;
}

GFont ResourceManager::getFont(const std::string& key)
{
    // If the font is not loaded, return a nullptr
	if (fontHolder.find(key) != fontHolder.end())
	{
		return fontHolder[key];
	}
	return nullptr;
}

GTexture ResourceManager::getTexture(const std::string& key)
{
    // If the texture is not loaded, return a nullptr
	if (textureHolder.find(key) != textureHolder.end())
	{
		return textureHolder[key];
	}
	return nullptr;
}

GMusic ResourceManager::getMusic(const std::string& key)
{
    // If the music is not loaded, return a nullptr
	if (musicHolder.find(key) != musicHolder.end())
	{
		return musicHolder[key];
	}
	return nullptr;
}

GJson ResourceManager::getJson(const std::string& key)
{
    // If the json is not loaded, return a nullptr
	if (jsonHolder.find(key) != jsonHolder.end())
	{
		return jsonHolder[key];
	}
	return nullptr;
}

GSoundBuffer ResourceManager::getSoundBuffer(const std::string& key)
{
    // If the sound buffer is not loaded, return a nullptr
	if (soundHolder.find(key) != soundHolder.end())
	{
		return soundHolder[key];
	}
	return nullptr;
}

void ResourceManager::unload(const std::string& key)
{
	if (musicHolder.find(key) != musicHolder.end())
	{
		musicHolder.erase(key);
	}
	else if (textureHolder.find(key) != textureHolder.end())
	{
		textureHolder.erase(key);
	}
	else if (jsonHolder.find(key) != jsonHolder.end())
	{
		jsonHolder.erase(key);
	}
	else if (soundHolder.find(key) != soundHolder.end())
	{
		soundHolder.erase(key);
	}
	else if (fontHolder.find(key) != fontHolder.end())
	{
		fontHolder.erase(key);
	}
}

void ResourceManager::setAudioLimit(unsigned long long limit)
{
	audioLimit = limit;
}

unsigned long long ResourceManager::getAudioLimit() const
{
	return audioLimit;
}
