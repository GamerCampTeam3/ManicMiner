#ifndef _E_BACKGROUND_MUSIC_NAMES_
#define _E_BACKGROUND_MUSIC_NAMES_


#include < map >
#include < string >

enum class EBackgroundMusicName
{
	CrystalCoralReef,
	DeepSeaOilRig,
	Shipwreck1,
	Shipwreck2,
	UnderwaterVolcano
};


const static std::map < const EBackgroundMusicName, const std::string > k_mapBackgroundMusicFiles =
{
	{ EBackgroundMusicName::CrystalCoralReef,	"audio/Music/CrystalCoralReefSvR.wav"	},
	{ EBackgroundMusicName::DeepSeaOilRig,		"audio/Music/DeepSeaOilRigSvR.wav"		},
	{ EBackgroundMusicName::Shipwreck1,			"audio/Music/shipwreck1default.mp3"		},
	{ EBackgroundMusicName::Shipwreck2,			"audio/Music/shipwreck2reggae.mp3"		},
	{ EBackgroundMusicName::UnderwaterVolcano,	"audio/Music/UnderwaterVolcanoSvR.wav"	}
};

#endif // End of #ifndef _E_BACKGROUND_MUSIC_NAMES_