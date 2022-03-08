#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>

//
//singleton class
//
class ResourceManager
{

private:
	ResourceManager();
	static ResourceManager* resourcemanager_;

	static std::map<std::string, std::string> textures;
	static std::map<std::string, std::string> data;
	static std::map<std::string, std::string> maps;
	static std::map<std::string, std::string> config;
	static std::map<std::string, std::string> sounds;
	static std::map<std::pair<int, std::string>, std::string> saveData;

	std::string error;

public:
	ResourceManager(ResourceManager& copy) = delete;

	void operator=(const ResourceManager&) = delete;

	static ResourceManager* GetInstance();

	std::string& TextureAt(const std::string& value);
	std::string& DataAt(const std::string& value);
	std::string& MapAt(const std::string& value);
	std::string& ConfigAt(const std::string& value);
	std::string& SoundAt(const std::string& value);
	std::string& SaveDataAt(const int save, const std::string& value);

	std::map<std::string, std::string>& GetMaps();
};

#endif