//////////////////////////////////////////////////////////////////////////////////////////
// Gamer Camp 2020 / Henrique															//
//////////////////////////////////////////////////////////////////////////////////////////
// ManicAudio.h acts as a helper header file to play audio through						//
// Cocos2d's SimpleAudioEngine:															//
// This page contains the ESoundName enums and their respective							//
// audio file directory pair, configured with an std::map < ESoundName, std::string >.	//
// Just pass the right sound name to PlaySoundEffect() or PreloadSoundEffect()			//
//////////////////////////////////////////////////////////////////////////////////////////

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
	ESN_KeyCollected,
	BabyShark
};

// Declare std::map that links every ESoundName to an audio file directory, type std::string
const static std::map < ESoundName, std::string > k_mapSoundFiles =
{
	{ ESoundName::ESN_Death,				"audio/Transition.wav"					},
	{ ESoundName::ESN_MusicTheme,			"audio/YouWouldNotBelieveYourEyes.wav"	},
	{ ESoundName::ESN_KeyCollected,			"audio/SnakeBumped.wav"					},
	{ ESoundName::BabyShark,				"audio/BabyShark.mp3"					}

};

// Helper function that plays a SoundEffect, taking in its respective ESoundName
inline void PlaySoundEffect( ESoundName eSoundName )
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	audioEngine->playEffect( sAudioDirectory, false, 1.0f, 1.0f, 1.0f );
}

// Helper function that preloads a SoundEffect, taking in its respective ESoundName
inline void PreloadSoundEffect( ESoundName eSoundName )
{
	auto audioEngine = SimpleAudioEngine::getInstance();

	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	audioEngine->preloadEffect( sAudioDirectory );
}

#endif // #ifndef __MANIC_AUDIO_HELPER__