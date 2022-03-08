#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include "queue"

#include "Observer.hpp"
#include "managers/ResourceManager.hpp"

enum MusicThemes
{
	FORESTTHEME,
	CAVETHEME
};

struct Musicbox
{
	int id;
	bool playedStart;
	bool hasStart;
	std::string startMusic;
	std::string loopMusic;

	Musicbox(int id, bool hasStart, const std::string& loopMusic, const std::string& startMusic = "")
	{
		this->id = id;
		this->hasStart = hasStart;
		this->startMusic = startMusic;
		this->loopMusic = loopMusic;
	}
};

class AudioPlayer : public IObserver
{

private:
	static std::map<std::string, sf::SoundBuffer> buffers;
	static std::deque<sf::Sound> sounds;
	static std::deque<std::string> activeSounds;
	static std::map<std::string, sf::Sound> loopSounds;

	static sf::Music activeTrack;
	static std::map<int, std::vector<Musicbox>> musicFlow;
	static std::pair<int, int> chosenMusic;

public:
	AudioPlayer();

	void Process(const int message, int id, int amount = 1);

	static void LoadSounds();

	static void PlaySoundOnLoop(const std::string& name, bool play);

	static void PauseAllOnLoop(bool pause);

	static void PlaySound(const std::string& name);

	static void CheckMusic();

	void PlayMusic(const std::string& name);

	void PlayMusicTheme(int id);
};

#endif