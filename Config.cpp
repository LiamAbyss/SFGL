#include "Config.h"

bool sfg::Config::loadConfig(const std::string& key, const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	if (!file.is_open()) return false;

	nlohmann::json config;
	file >> config;
	file.close();
	configs.try_emplace(key, config);
	return true;
}

nlohmann::json sfg::Config::getConfig(const std::string& key)
{
	if (configs.find(key) == configs.end())
		throw std::runtime_error(key + " was not loaded in the configuration.");

	return configs.at(key);
}
