#ifndef _SLEVELCREATIONPARAMATERS_H_
#define _SLEVELCREATIONPARAMATERS_H_

#include "SLevelValues.h"
#include <string>

/// <summary>
/// This structs holds all information needed for a level to be created.
/// </summary>
/// <param name="slevelValues"> <c>  Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized. </param>
/// <param name="szLevelbackground"><c> A char* that will take be used to set the Background of the current level. </param>
/// <param name="szLevelpath"><c> A std::string that wil hold the location of the level file (.oel) </param>
/// <param name="szLevelName"><c> A std::string that wil hold the name of the level to be displayed by CHUD. </param>
struct SLevelCreationParameters
{
	/// <summary>
	/// This specific constructor can initialize values to whatever you want it to be.
	/// </summary>
	/// <param name="slevelValues"> <c>  Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized. </param>
	/// <param name="szLevelbackground"><c>A char* that will take be used to set the Background of the current level. </param>
	/// <param name="szLevelpath"><c>A std::string that wil hold the location of the level file (.oel) </param>
	/// <param name="szLevelName"><c>A std::string that wil hold the name of the level to be displayed by CHUD. </param>
	SLevelCreationParameters( SLevelValues slevelValues, char* szLevelbackground, std::string szLevelpath, std::string szLevelname)
		: sLevelValues					( slevelValues		 )
		, pszLevelBackground			( szLevelbackground	 )
		, szLevelPath					( szLevelpath		 )
		, szLevelName					( szLevelname		 )
	{
	}

	// This default constructor will initialize to default values.
	SLevelCreationParameters()
		: sLevelValues					( SLevelValues() )
		, pszLevelBackground			( nullptr		 )
		, szLevelPath					( ""		 )
		, szLevelName					( ""		 )		
	{		
	}


	SLevelValues				sLevelValues;		// Struct that takes in an "ECollectibleRequirements" and two "int" to be used by the "CLevelManager". It can be Zero initialized.
	char*						pszLevelBackground;	// A char* that will take be used to set the Background of the current level. 
	std::string					szLevelPath;		// A std::string that wil hold the location of the level file (.oel).
	std::string					szLevelName;		// Used to display the name of the level on the HUD.
	
};

#endif //ifndef _SLEVELCREATIONPARAMATERS_H_