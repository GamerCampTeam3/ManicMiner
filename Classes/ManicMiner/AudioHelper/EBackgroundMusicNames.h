// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Bib & Henrique																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This header file contains the different music themes,													//
//				an enum class and an std::map mapping those enums to their respective audio file directory				//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _E_BACKGROUND_MUSIC_NAMES_
#define _E_BACKGROUND_MUSIC_NAMES_


#include < map >
#include < string >

enum class EBackgroundMusicName
{
	CrystalCoralReef1,
	CrystalCoralReef2,
	CrystalCoralReef3,
	CrystalCoralReef4,
	DeepSeaOilRig1,
	DeepSeaOilRig2,
	DeepSeaOilRig3,
	DeepSeaOilRig4,
	Shipwreck1,
	Shipwreck2,
	Shipwreck3,
	UnderwaterVolcano1,
	UnderwaterVolcano2,
	UnderwaterVolcano3,
	UnderwaterVolcano4,
};


const static std::map < const EBackgroundMusicName, const std::string > k_mapBackgroundMusicFiles =
{
	{ EBackgroundMusicName::CrystalCoralReef1,	"audio/Music/CrystalCoralReef/V1.wav"	},
	{ EBackgroundMusicName::CrystalCoralReef2,	"audio/Music/CrystalCoralReef/V2.wav"	},
	{ EBackgroundMusicName::CrystalCoralReef3,	"audio/Music/CrystalCoralReef/V3.wav"	},
	{ EBackgroundMusicName::CrystalCoralReef4,	"audio/Music/CrystalCoralReef/V4.wav"	},
	{ EBackgroundMusicName::DeepSeaOilRig1,		"audio/Music/DeepSeaOilRig/V1.wav"		},
	{ EBackgroundMusicName::DeepSeaOilRig2,		"audio/Music/DeepSeaOilRig/V2.wav"		},
	{ EBackgroundMusicName::DeepSeaOilRig3,		"audio/Music/DeepSeaOilRig/V3.wav"		},
	{ EBackgroundMusicName::DeepSeaOilRig4,		"audio/Music/DeepSeaOilRig/V4.wav"		},
	{ EBackgroundMusicName::Shipwreck1,			"audio/Music/Shipwreck/V1.wav"			},
	{ EBackgroundMusicName::Shipwreck2,			"audio/Music/Shipwreck/V2.wav"			},
	{ EBackgroundMusicName::Shipwreck3,			"audio/Music/Shipwreck/V3.wav"			},
	{ EBackgroundMusicName::UnderwaterVolcano1,	"audio/Music/UnderwaterVolcano/V1.wav"	},
	{ EBackgroundMusicName::UnderwaterVolcano2,	"audio/Music/UnderwaterVolcano/V2.wav"	},
	{ EBackgroundMusicName::UnderwaterVolcano3,	"audio/Music/UnderwaterVolcano/V3.wav"	},
	{ EBackgroundMusicName::UnderwaterVolcano4,	"audio/Music/UnderwaterVolcano/V4.wav"	}
};

#endif // End of #ifndef _E_BACKGROUND_MUSIC_NAMES_