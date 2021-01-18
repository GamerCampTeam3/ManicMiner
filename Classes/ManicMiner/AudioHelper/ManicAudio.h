// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:		ManicAudio.h acts as a helper header file to play audio through										//
//					Cocos2d's SimpleAudioEngine:																		//
//					This page contains the ESoundEffectName enums and their respective									//
//					audio file directory pair, configured with an std::map < ESoundEffectName, std::string >			//
//					Just pass the right sound name to PlaySoundEffect() or PreloadSoundEffect()							//
// -------------------------------------------------------------------------------------------------------------------- //

// There is some source code in this header file but that is a necessity, caused by its design
// The idea of this header file was to very simply be able to play audio from anywhere, without having to include
// any classes or singleton ( that would defeat half the point of this header file )
// All the functions here are inline, which need to be declared and defined in a header file, so they work anywhere

#ifndef __MANIC_AUDIO_HELPER__
#define __MANIC_AUDIO_HELPER__

#include "EBackgroundMusicNames.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include < string >
#include < map >

// -------------------------------------------------------------------------------------------------------------------- //
// Enum Class	:	ESoundEffectName																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Defines a name for every sound effect in the game													//
//																														//
// See Also		:	k_mapSoundFiles	& PlaySoundEffect()																	//
// -------------------------------------------------------------------------------------------------------------------- //
enum class ESoundEffectName
{
	Death,
	KeyCollected,
	SwitchFlipped,
	RunningFootsteps,
	Jump,
	Falling,
	Crumbling_Stage,
	Crumbling_Full
};

// -------------------------------------------------------------------------------------------------------------------- //
// Std::map		:	k_mapSoundFiles																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Types		:	ESoundEffectName, std::string																		//
//																														//
// Purpose		:	Maps every sound < ESoundEffectName > to a specific file directory < std::string >					//
//																														//
// See Also		:	ESoundEffectName & PlaySoundEffect()																//
// -------------------------------------------------------------------------------------------------------------------- //
const static std::map < const ESoundEffectName, const std::string > k_mapSoundEffectFiles =
{
	{ ESoundEffectName::Death,				"audio/SFX/death_transition.wav"},
	{ ESoundEffectName::KeyCollected,		"audio/SFX/item_pickup.wav"		},
	{ ESoundEffectName::SwitchFlipped,		"audio/SFX/switch_1.wav"		},
	{ ESoundEffectName::RunningFootsteps,	"audio/SFX/wilfred_run_1.wav"	},
	{ ESoundEffectName::Jump,				"audio/SFX/jump_full.wav"		},
	{ ESoundEffectName::Falling,			"audio/SFX/jump_down.wav"		},
	{ ESoundEffectName::Crumbling_Stage,	"audio/Crumbling_Stage.wav"		},
	{ ESoundEffectName::Crumbling_Full,		"audio/Crumbling_Full.wav"		}
};


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PlaySoundEffect																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::playEffect()				//
//					Because the above function takes in a const char* for the sound file directory, we use this			//
//					one that takes in an ESoundEffectName, making it much easier and cleaner to play our desired sound	//
//																														//
// Parameters	:	const ESoundEffectName eSoundName																	//
//					enum value that represents the sound we want to play												//
//																														//
// See also		:	ESoundEffectName & k_mapSoundFiles																	//
// -------------------------------------------------------------------------------------------------------------------- //
// Example		:	#include "ManicMiner/AudioHelper/ManicAudio.h"														//	
//																														//	
//					void CCoin::OnPickedUp()																			//
//					{																									//
//						// Play CoinCollected sound effect	[ would need to be added to k_mapSoundFiles ] 				//
//						PlaySoundEffect( ESoundEffectName::CoinCollected );												//
//																														//
//						// Delete coin object																			//
//						DeleteSelf();																					//
//					}																									//
// -------------------------------------------------------------------------------------------------------------------- //
inline unsigned int PlaySoundEffect( const ESoundEffectName eSoundName )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();
	
	// Obtain correct directory from the k_mapSoundFiles, converting from string to const char*
	const char* sAudioDirectory = k_mapSoundEffectFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	return pAudioEngine->playEffect( sAudioDirectory, false, 1.0f, 1.0f, 1.0f );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopSoundEffect																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::stopEffect()				//
//																														//
// Parameters	:	const unsigned int uiSoundID																		//
//					the ID of the soundEffect one wants to stop															//
// -------------------------------------------------------------------------------------------------------------------- //
inline void StopSoundEffect( const unsigned int uiSoundID )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	// Stop desired sound effect using designated ID, which is returned when calling PlaySoundEffect()
	return pAudioEngine->stopEffect( uiSoundID );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PreloadSoundEffect																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::preloadEffect()				//
//					Because the above function takes in a const char* for the sound file directory, we use this			//
//					one that takes in an ESoundEffectName, making it much easier and cleaner to preload our desired sound//
//																														//
// Parameters	:	const ESoundEffectName eSoundName																	//
//					enum value that represents the sound we want to play												//
// -------------------------------------------------------------------------------------------------------------------- //
inline void PreloadSoundEffect( const ESoundEffectName eSoundName )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	// Obtain correct directory from the k_mapSoundFiles, converting from string to const char*
	const char* sAudioDirectory = k_mapSoundEffectFiles.at( eSoundName ).c_str();

	// Play desired sound effect
	pAudioEngine->preloadEffect( sAudioDirectory );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	PlayBackgroundMusic																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::playBackgroundMusic()		//
//																														//
// Parameters	:	const EBackgroundMusicName eMusicName																//
//					enum of the Background Music we want to play														//
//																														//
// See Also		:	EBackgroundMusicName.h																				//
// -------------------------------------------------------------------------------------------------------------------- //
inline void PlayBackgroundMusic( const EBackgroundMusicName eMusicName )
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	// Obtain correct directory from the k_mapSoundFiles, converting from string to const char*
	const char* sAudioDirectory = k_mapBackgroundMusicFiles.at( eMusicName ).c_str();

	// Play desired background music
	pAudioEngine->playBackgroundMusic( sAudioDirectory, true );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopBackgroundMusic																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::stopBackgroundMusic()		//
//																														//
// Parameters	:	none																								//
// -------------------------------------------------------------------------------------------------------------------- //
inline void StopBackgroundMusic()
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	pAudioEngine->stopBackgroundMusic( true );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	StopAllSound																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	This function acts as a substitute to CocosDenshion::SimpleAudioEngine::stopAllEffects()			//
//					and CocosDenshion::SimpleAudioEngine::stopBackgroundMusic()											//
//																														//
// Parameters	:	none																								//
// -------------------------------------------------------------------------------------------------------------------- //
inline void StopAllSound()
{
	// Get SimpleAudioEngine singleton
	auto pAudioEngine = SimpleAudioEngine::getInstance();

	pAudioEngine->stopAllEffects();
	pAudioEngine->stopBackgroundMusic( true );
}
#endif // #ifndef __MANIC_AUDIO_HELPER__