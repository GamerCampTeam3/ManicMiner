#ifndef __MANIC_AUDIO_HELPER__
#define __MANIC_AUDIO_HELPER__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <string>
#include <map>

// Declare Enum 'ESoundName' for Every Sound
enum class ESoundName
{
	ESN_Death,
	ESN_MusicTheme,
	ESN_KeyCollected
};

// Declare std::map that links every ESoundName to an audio file directory, type std::string
const static std::map <ESoundName, std::string> k_mapSoundFiles =
{
	{ ESoundName::ESN_Death,				"audio/Transition.wav"					},
	{ ESoundName::ESN_MusicTheme,			"audio/YouWouldNotBelieveYourEyes.wav"	},
	{ ESoundName::ESN_KeyCollected,			"audio/Transition.wav"					}
};

// Helper function that plays a SoundEffect, taking in its respective ESoundName
void PlaySoundEffect( ESoundName eSoundName )
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	audioEngine->playEffect( sAudioDirectory, false, 1.0f, 1.0f, 1.0f );
}

void PreloadSoundEffect( ESoundName eSoundName )
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	audioEngine->preloadEffect( sAudioDirectory );
}

#endif // #ifndef __MANIC_AUDIO_HELPER__