#ifndef INCLUDE_CONFIG
#define INCLUDE_CONFIG

#include <map>
#include <string>
#include <fstream>
#include "JsonHeader/json.hpp"

namespace sfg
{
	class Config
	{
	private:
		std::map<std::string, nlohmann::json, std::less<>> configs;

	public:
		bool loadConfig(const std::string& key, const std::string& filename);

		nlohmann::json getConfig(const std::string& key);
	};
}

#endif // !INCLUDE_CONFIG
