#ifndef INCLUDE_RESOURCE_MANAGER
#define INCLUDE_RESOURCE_MANAGER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "JsonHeader/json.hpp"
#include <iostream>
#include <Windows.h>
#include <fstream>

using GTexture = std::shared_ptr<sf::Texture>;
using GMusic = std::shared_ptr<sf::Music>;
using GSoundBuffer = std::shared_ptr<sf::SoundBuffer>;
using GJson = std::shared_ptr<nlohmann::json>;
using GFont = std::shared_ptr<sf::Font>;

/** 
 * Manages all the resources.
 * \see Game::resources
 */
class ResourceManager
{
private:
    
	/** 
	 * \private
	 * Contains all the SFML Textures.
	 */
    std::map<std::string, GTexture> textureHolder;

    /** 
	 * \private
	 * Contains all the json files.
	 */
    std::map<std::string, GJson> jsonHolder;

    /** 
	 * \private
	 * Contains all the SFML SoundBuffers.
	 */
    std::map<std::string, GSoundBuffer> soundHolder;

    /** 
	 * \private
	 * Contains all the SFML Musics.
	 */
    std::map<std::string, GMusic> musicHolder;

    /** 
	 * \private
	 * Contains all the SFML fonts.
	 */
    std::map<std::string, GFont> fontHolder;

    /** 
	 * \private
	 * The limit in size between SFML SoundBuffers and Musics.
     * Default value is 1Mo.
	 * \note SoundBuffers are loaded into the memory while Musics are streamed directly from their file.
	 */
    unsigned long long audioLimit = 1000000;

public:

    /**
	 * Loads a resource.
	 * \param key The key associated to the resource.
	 * \param file The file where the resource is located.
	 * \return true If the file could be loaded.
	 * \return false If the file doesn't exist or if the extension is not supported.
	 * \note Currently supported : Audio and image files supported by SFML, JSON files, and font files (.ttf)
	 */
    bool load(const std::string& key, const std::string& file);

    /**
	 * \private
	 * \param key The key associated to the resource.
	 * \return The font associated to the key.
	 * \see GFont
	 */
    GFont getFont(const std::string& key);

    /**
     * \private
     * \param key The key associated to the resource.
     * \return The texture associated to the key.
     * \see GTexture
     */
    GTexture getTexture(const std::string& key);

    /**
     * \private
     * \param key The key associated to the resource.
     * \return The json associated to the key.
     * \see GJson
     */
    GJson getJson(const std::string& key);

    /**
     * \private
     * \param key The key associated to the resource.
     * \return The sound buffer associated to the key.
     * \see GSoundBuffer
     */
    GSoundBuffer getSoundBuffer(const std::string& key);

    /**
     * \private
     * \param key The key associated to the resource.
     * \return The music associated to the key.
     * \see GMusic
     */
    GMusic getMusic(const std::string& key);

    /**
	 * \private
	 * \param key The key associated to the resource.
	 */
    void unload(const std::string& key);

    /**
	 * \private
	 * \param limit The limit in size between SFML SoundBuffers and Musics.
	 */
    void setAudioLimit(unsigned long long limit);

    /**
	 * \private
	 * \return The limit in size between SFML SoundBuffers and Musics.
	 */
    unsigned long long getAudioLimit() const;
};

/** 
 * \param extension The file extension
 * \return The MIME type of the file.
 */
std::string getMimeType(const std::string& extension);

/** 
 * \param filename The name of the file.
 * \return The size of the file in bytes if it exists, -1 otherwise.
 */
long long getFileSize(const std::string& filename);


#endif // !INCLUDE_RESOURCE_MANAGER