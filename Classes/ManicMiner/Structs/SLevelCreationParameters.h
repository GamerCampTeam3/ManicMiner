#ifndef _SLEVELCREATIONPARAMATERS_H_
#define _SLEVELCREATIONPARAMATERS_H_

#include "SLevelValues.h"
#include <string>
#include "ManicMiner/AudioHelper/EBackgroundMusicNames.h"
#include "ManicMiner/Parallax/EParallaxTheme.h"

/// <summary>
/// This structs holds all information needed for a level to be created.
/// </summary>
/// <param name="slevelValues"> <c>  Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized. </param>
/// <param name="eParallaxTheme"><c> An enum that will be used to set the different parallax layers of the current level. </param>
/// <param name="szLevelpath"><c> A std::string that wil hold the location of the level file (.oel) </param>
/// <param name="szLevelName"><c> A std::string that wil hold the name of the level to be displayed by CHUD. </param>
/// <param name="v2PlayerStartPos"><c>A vector2 that is used to place the player on the level. </param>
/// <param name="pszPlatformBackground"><c>A char* that should hold the path to the background plist. </param>
struct SLevelCreationParameters
{
	/// <summary>
	/// This specific constructor can initialize values to whatever you want it to be.
	/// </summary>
	/// <param name="slevelValues"> <c>  Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized. </param>
	/// <param name="szLevelbackground"><c>An enum that will be used to set the different parallax layers of the current level. </param>
	/// <param name="szLevelpath"><c>A std::string that wil hold the location of the level file (.oel) </param>
	/// <param name="szLevelName"><c>A std::string that wil hold the name of the level to be displayed by CHUD. </param>
	/// <param name="v2PlayerStartPos"><c>A vector2 that is used to place the player on the level. </param>
	/// <param name="pszPlatformBackground"><c>A char* that should hold the path to the background plist. </param>
	/// <param name="pszVisualElement"><c> A char* that should hold the path to the Visual Element plist </param>
	SLevelCreationParameters( SLevelValues slevelValues, EParallaxTheme eParallaxTheme, EBackgroundMusicName eBackgroundMusic, std::string szLevelpath, std::string szLevelname, cocos2d::Vec2 v2StartPos, bool bShouldLookRight, char* pszPlatformbg)
		: sLevelValues					( slevelValues		)
		, eParallaxTheme				( eParallaxTheme	)
		, eBackgroundMusic				( eBackgroundMusic )
		, szLevelPath					( szLevelpath		)
		, szLevelName					( szLevelname		)
		, v2PlayerStartPos				( v2StartPos		)
		, bShouldFaceRight				( bShouldLookRight	)
		, pszPlatformBackground			( pszPlatformbg		)
	{
	}

	// This default constructor will initialize to default values.
	SLevelCreationParameters()
		: sLevelValues					( SLevelValues() )
		, eParallaxTheme				( EParallaxTheme::Cavern )
		, eBackgroundMusic				( EBackgroundMusicName::CrystalCoralReef )
		, szLevelPath					( ""		 )
		, szLevelName					( ""		 )
		, v2PlayerStartPos				( cocos2d::Vec2( 120.0f + 30.0f, 120.0f ) )
		, bShouldFaceRight				( false			 )
		, pszPlatformBackground			( nullptr		 )
	{		
	}


	SLevelValues				sLevelValues;			// Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized.
	EParallaxTheme				eParallaxTheme;			// An enum that will be used to set the different parallax layers of the current level. 
	EBackgroundMusicName		eBackgroundMusic;		// Background music that should play in this level
	std::string					szLevelPath;			// A std::string that wil hold the location of the level file (.oel).
	std::string					szLevelName;			// Used to display the name of the level on the HUD.
	cocos2d::Vec2				v2PlayerStartPos;		// The location at which the player should spawn at.
	bool						bShouldFaceRight;		// Should the player originally should face right or not.
	char*						pszPlatformBackground;	// The path to the background platform .plist
	
};

#endif //ifndef _SLEVELCREATIONPARAMATERS_H_