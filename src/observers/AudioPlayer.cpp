#include "AudioPlayer.hpp"

std::map<std::string, sf::SoundBuffer> AudioPlayer::buffers;
std::deque<sf::Sound> AudioPlayer::sounds;
std::deque<std::string> AudioPlayer::activeSounds;
std::map<std::string, sf::Sound> AudioPlayer::loopSounds;
sf::Music AudioPlayer::activeTrack;
std::map<int, std::vector<Musicbox>> AudioPlayer::musicFlow;
std::pair<int, int> AudioPlayer::chosenMusic;

AudioPlayer::AudioPlayer()
{
}

void AudioPlayer::Process(const int message, int id, int amount)
{
	(void)id;
	(void)amount;
	std::cout << "AUDIOPLAYER:: IS PROCESSING MESSAGE" << std::endl;
	switch (message)
	{
		case SubjectActions::GATHER_ITEM:
			PlaySound("MINE");
			std::cout << "AUDIOMANAGER PLAYED MINE SOUND" << std::endl;
			break;
		case SubjectActions::GIVE_QUEST:
			PlaySound("GETQUEST");
			std::cout << "AUDIOMANAGER PLAYED GETQUEST SOUND" << std::endl;
			break;
		case SubjectActions::NEXT_PAGE:
			PlaySound("NEXTDIALOGUE");
			std::cout << "AUDIOMANAGER PLAYED NEXT DIALOGUE SOUND" << std::endl;
			break;
		case SubjectActions::ORE_MINED:
			PlaySound("OREMINED");
			break;
		case SubjectActions::ENTER_GAMESTATE:
			PlayMusic("ADVENTURETHEME");
			break;
		case SubjectActions::ENTER_MAINMENU:
			PlayMusic("MAINMENUTHEME");
			break;
		case SubjectActions::ENTER_SETTINGS:
			PlayMusic("MAINMENUTHEME");
			break;
		case SubjectActions::ENTER_FIGHT:
			PauseAllOnLoop(true);
			PlayMusicTheme(id);
			break;
		case SubjectActions::BTN_CLICKED:
			PlaySound("BTNCLICK");
			break;
		case SubjectActions::LEVEL_UP:
			PlaySound("LEVELUP");
			break;
		default:
			break;
	}
}

void AudioPlayer::PlaySound(const std::string& name)
{

	if (activeSounds.empty() == false)
	{
		if (activeSounds.back() == name)
		{
			if (sounds.back().getStatus() == sf::SoundSource::Status::Stopped)
			{
				sounds.pop_back();
				activeSounds.pop_back();
			}
			else
			{
				return;
			}
		}
	}

	activeSounds.push_front(name);
	sounds.push_front(sf::Sound(buffers.at(name)));
	sounds.front().play();
	if (sounds.back().getStatus() == sf::SoundSource::Status::Stopped)
	{
		sounds.pop_back();
		activeSounds.pop_back();
	}
}

void AudioPlayer::PlayMusic(const std::string& name)
{
	ResourceManager* resourceman = ResourceManager::GetInstance();
	if (!activeTrack.openFromFile(resourceman->SoundAt(name)))
		std::cout << "Could not play music: " << name << std::endl;

	activeTrack.setLoop(true);
	activeTrack.setVolume(50.f);
	activeTrack.play();
}

void AudioPlayer::PlayMusicTheme(int id)
{
	switch (id)
	{
		case MusicThemes::FORESTTHEME:
			chosenMusic.first = MusicThemes::FORESTTHEME;
			chosenMusic.second = std::rand() % musicFlow.at(MusicThemes::FORESTTHEME).size();
			if (musicFlow.at(MusicThemes::FORESTTHEME).at(chosenMusic.second).hasStart == true)
			{
				if (!activeTrack.openFromFile(musicFlow.at(MusicThemes::FORESTTHEME).at(chosenMusic.second).startMusic))
					std::cout << "AUDIOMANAGER:: could not load file: " << musicFlow.at(MusicThemes::FORESTTHEME).at(chosenMusic.second).startMusic << std::endl;
				activeTrack.setLoop(false);
				activeTrack.play();
			}
			else
			{
				if (!activeTrack.openFromFile(musicFlow.at(MusicThemes::FORESTTHEME).at(chosenMusic.second).loopMusic))
					std::cout << "AUDIOMANAGER:: could not load file: " << musicFlow.at(MusicThemes::FORESTTHEME).at(chosenMusic.second).startMusic << std::endl;
				activeTrack.setLoop(true);
				activeTrack.play();
			}
			break;
		case MusicThemes::CAVETHEME:
			chosenMusic.first = MusicThemes::CAVETHEME;
			chosenMusic.second = std::rand() % musicFlow.at(MusicThemes::CAVETHEME).size();
			if (musicFlow.at(MusicThemes::CAVETHEME).at(chosenMusic.second).hasStart == true)
			{
				if (!activeTrack.openFromFile(musicFlow.at(MusicThemes::CAVETHEME).at(chosenMusic.second).startMusic))
					std::cout << "AUDIOMANAGER:: could not load file: " << musicFlow.at(MusicThemes::CAVETHEME).at(chosenMusic.second).startMusic << std::endl;
				activeTrack.setLoop(false);
				activeTrack.play();
			}
			else
			{
				if (!activeTrack.openFromFile(musicFlow.at(MusicThemes::CAVETHEME).at(chosenMusic.second).loopMusic))
					std::cout << "AUDIOMANAGER:: could not load file: " << musicFlow.at(MusicThemes::CAVETHEME).at(chosenMusic.second).startMusic << std::endl;
				activeTrack.setLoop(true);
				activeTrack.play();
			}
			break;
		default:

			break;
	}
}

void AudioPlayer::LoadSounds()
{
	ResourceManager* resourceman = ResourceManager::GetInstance();

	if (!buffers["MINE"].loadFromFile(resourceman->SoundAt("MINE")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD MINE SOUND" << std::endl;
	}
	if (!buffers["NEXTDIALOGUE"].loadFromFile(resourceman->SoundAt("NEXTDIALOGUE")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD NEXTDIALOGUE SOUND" << std::endl;
	}
	if (!buffers["GETQUEST"].loadFromFile(resourceman->SoundAt("GETQUEST")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD GETQUEST SOUND" << std::endl;
	}
	if (!buffers["OREMINED"].loadFromFile(resourceman->SoundAt("OREMINED")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD OREMINED SOUND" << std::endl;
	}
	if (!buffers["WALKING"].loadFromFile(resourceman->SoundAt("WALKING")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD WALKING SOUND" << std::endl;
	}
	if (!buffers["BTNCLICK"].loadFromFile(resourceman->SoundAt("BTNCLICK")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD BTNCLICK SOUND" << std::endl;
	}
	if (!buffers["HURT"].loadFromFile(resourceman->SoundAt("HURT")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD HURT SOUND" << std::endl;
	}
	if (!buffers["DODGE"].loadFromFile(resourceman->SoundAt("DODGE")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD DODGE SOUND" << std::endl;
	}
	if (!buffers["LEVELUP"].loadFromFile(resourceman->SoundAt("LEVELUP")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD LEVEL UP SOUND" << std::endl;
	}
	if (!buffers["QUICKSTRIKE"].loadFromFile(resourceman->SoundAt("QUICKSTRIKE")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD QUICK STRIKE SOUND" << std::endl;
	}
	if (!buffers["SLASH"].loadFromFile(resourceman->SoundAt("SLASH")))
	{
		std::cout << "AUDIOPLAYER::COULD NOT LOAD SLASH SOUND" << std::endl;
	}

	musicFlow[MusicThemes::FORESTTHEME] = {
		Musicbox(0, true, resourceman->SoundAt("FORESTTHEMELOOP"), resourceman->SoundAt("FORESTTHEMESTART")),
		Musicbox(1, false, resourceman->SoundAt("FORESTTHEME"))
	};
	musicFlow[MusicThemes::CAVETHEME] = {
		Musicbox(0, true, resourceman->SoundAt("CAVETHEMELOOP"), resourceman->SoundAt("CAVETHEMESTART")),
		Musicbox(1, false, resourceman->SoundAt("CAVETHEME"))
	};
}

void AudioPlayer::PlaySoundOnLoop(const std::string& name, bool play)
{
	for (auto& a : loopSounds)
	{
		if (a.first == name)
		{
			if (play == false)
			{
				a.second.stop();
				return;
			}
			else
			{
				if (a.second.getStatus() == sf::SoundSource::Status::Playing)
				{
					return;
				}
				if (a.second.getStatus() == sf::SoundSource::Status::Stopped)
				{
					a.second.play();
				}
				if (a.second.getStatus() == sf::SoundSource::Status::Paused)
				{
					a.second.play();
				}
				return;
			}
		}
	}

	loopSounds[name] = sf::Sound(buffers.at(name));
	loopSounds.at(name).setLoop(true);
	if (play)
	{
		loopSounds.at(name).play();
	}
	else
	{
		loopSounds.at(name).stop();
	}
}
void AudioPlayer::PauseAllOnLoop(bool play)
{
	if (!play)
	{
		return;
	}
	else
	{
		for (auto& a : loopSounds)
		{
			a.second.pause();
		}
	}
}

void AudioPlayer::CheckMusic()
{
	if (activeTrack.getLoop() == false)
	{
		if (activeTrack.getStatus() == sf::SoundSource::Status::Stopped)
		{
			for (auto& a : musicFlow.at(chosenMusic.first))
			{
				if (a.id == chosenMusic.second)
				{
					if (!activeTrack.openFromFile(musicFlow.at(chosenMusic.first).at(chosenMusic.second).loopMusic))
						std::cout << "AUDIOMANAGER:: could not load file: " << musicFlow.at(chosenMusic.first).at(chosenMusic.second).loopMusic << std::endl;
					activeTrack.setLoop(true);
					activeTrack.play();
				}
			}
		}
	}
}
