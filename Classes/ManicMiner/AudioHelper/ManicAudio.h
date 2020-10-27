// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:		ManicAudio.h acts as a helper header file to play audio through										//	
//					Cocos2d's SimpleAudioEngine:																		//	
//					This page contains the ESoundName enums and their respective										//
//					audio file directory pair, configured with an std::map < ESoundName, std::string >					//
//					Just pass the right sound name to PlaySoundEffect() or PreloadSoundEffect()							//
// -------------------------------------------------------------------------------------------------------------------- //
// Example	:		#include "ManicMiner/AudioHelper/ManicAudio.h"														//	
//																														//	
//					void CCoin::OnPickedUp()																			//
//					{																									//
//						// Play CoinCollected sound effect	[ would need to be added to k_mapSoundFiles ] 				//
//						PlaySoundEffect( ESoundName::CoinCollected );													//
//																														//
//						// Delete coin object																			//
//						DeleteSelf();																					//
//					}																									//
// -------------------------------------------------------------------------------------------------------------------- //


#ifndef __MANIC_AUDIO_HELPER__
#define __MANIC_AUDIO_HELPER__

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include < string >
#include < map >

// -------------------------------------------------------------------------------------------------------------------- //
// Enum Class	:	ESoundName																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Defines a name for every sound effect in the game													//
//																														//
// See Also		:	k_mapSoundFiles	& PlaySoundEffect()																	//
// -------------------------------------------------------------------------------------------------------------------- //
enum class ESoundName
{
	Death,
	MusicTheme,
	KeyCollected,
	BabyShark
};

// -------------------------------------------------------------------------------------------------------------------- //
// Std::map		:	k_mapSoundFiles																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Types		:	ESoundName, std::string																				//
//																														//
// Purpose		:	Maps every sound < ESoundName > to a specific file directory < std::string >						//
//																														//
// See Also		:	ESoundName & PlaySoundEffect()																		//
// -------------------------------------------------------------------------------------------------------------------- //
const static std::map < const ESoundName, const std::string > k_mapSoundFiles =
{
	{ ESoundName::Death,				"audio/Transition.wav"					},
	{ ESoundName::MusicTheme,			"audio/YouWouldNotBelieveYourEyes.wav"	},
	{ ESoundName::KeyCollected,			"audio/SnakeBumped.wav"					},
	{ ESoundName::BabyShark,			"audio/BabyShark.mp3"					}
};

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PlaySoundEffect																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::playEffect()				//
//					Because the above function takes in a const char* for the sound file directory, we use this			//
//					one that takes in an ESoundName, making it much easier and cleaner to play our desired sound		//
//																														//
// Parameters	:	const ESoundName eSoundName																			//
//					enum value that represents the sound we want to play												//
//																														//
// See also		:	ESoundName & k_mapSoundFiles																		//
// -------------------------------------------------------------------------------------------------------------------- //
inline void PlaySoundEffect( const ESoundName eSoundName )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();
	
	// Obtain correct directory from the k_mapSoundFiles, converting from string to const char*
	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	pAudioEngine->playEffect( sAudioDirectory, false, 1.0f, 1.0f, 1.0f );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PreloadSoundEffect																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::preloadEffect()				//
//					Because the above function takes in a const char* for the sound file directory, we use this			//
//					one that takes in an ESoundName, making it much easier and cleaner to preload our desired sound		//
//																														//
// Parameters	:	const ESoundName eSoundName																			//
//					enum value that represents the sound we want to play												//
//																														//
// See also		:	ESoundName & k_mapSoundFiles																		//
// -------------------------------------------------------------------------------------------------------------------- //
inline void PreloadSoundEffect( const ESoundName eSoundName )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	// Obtain correct directory from the k_mapSoundFiles, converting from string to const char*
	const char* sAudioDirectory = k_mapSoundFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	pAudioEngine->preloadEffect( sAudioDirectory );
}

#endif // #ifndef __MANIC_AUDIO_HELPER__