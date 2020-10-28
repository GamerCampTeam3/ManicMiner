// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	ResolutionHelper.h defines the game's PTM Ratio														//
//					It also includes a function that detects the resolution of the current screen display				//
//					"GetDesktopResolution()"																			//
//																														//
//					This would detect the current resolution and adjust the assets and PTM accordingly					//
//					Because some of us have 2K monitors, it would be awesome if we could play Manic Miner				//
//					on a 2560x1440 resolution. This however was a stretch and we had no time to implement,				//
//					as it is quite high risk near end of pre-production, and other parts of the game					//
//					were definitely a priority.	Unique spritesheets have to be created for each resolution				//
//					( FHD, WQHD and UHD ), perhaps it will be done during Production phase.								//
//																														//
//					At the moment, this header file isn't doing anything, but in Production phase						//
//					any SetPosition() calls should do something like the following:										//
// -------------------------------------------------------------------------------------------------------------------- //
// Implementation:	#include "ManicMiner/ResolutionHelper/ResolutionHelper.h"											//
//																														//
//					// Get current PTM																					//
//					const float fPTM = MANIC_PTM_RATIO;																	//
//																														//
//					// Declare num of coins we want to spawn															//
//					const int iNumCoins = 9;																			//
//																														//
//					// Loop through each coin to set positions															//
//					for ( int iCurrentCoin = 0; iCurrentCoin < iNumCoins; iCurrentCoin++ )								//
//					{																									//
//						// Get offset from centre which is half of the size of the sprite								//
//						// If a coin is 60x60, the offset will be 30.0f													//
//						float fSpriteOffset = fPTM * 0.5f;																//
//																														//
//						// Calculate new position for this coin															//
//																														//
//						// We want the coins to be on the 4th block, counting from the bottom up						//
//						float fCoinY = ( 4.0f * fPTM ) + fSpriteOffset;													//
//																														//
//						// We want the coins to be 2 blocks apart horizontally, starting from the 5th on the left		//
//						// Start by getting the 5-blocks-in offset														//
//						float fXOffset = 4.0f * fPTM																	//
//																														//
//						// Calculate space in-between each coin															//
//						// 2 blocks of free space + 1 block for the coin itself											//
//						float fCoinXSpacing = 3.0f * fPTM;																//
//																														//
//						float fCoinX = fXOffset + ( iCurrentCoin * fCoinXSpacing ) + fSpriteOffset;						//
//																														//
//						// Set position																					//
//						Vec2 newPos = ( fCoinX, fCoinY );																//
//						apMyCoins[ iCurrentCoin ]->SetPosition( newPos );												//
//					}																									//
//																														//
//	As we can see, the PTM value can come in very handy to set the exact positions of elements in grid-like levels 		//
//																														//
// -------------------------------------------------------------------------------------------------------------------------------------------------------- //
// See Also		:	Manic Miner Asset and Resolution Helper	@ Gamer Camp 2020 Wiki																			//
//					http://gamercamp2020.pbworks.com/w/page/141795267/Manic%20Miner%20-%20Art%20Pipeline%2C%20Asset%20List%20and%20Target%20Resolution		//
// -------------------------------------------------------------------------------------------------------------------------------------------------------- //


#ifndef _RESOLUTIONHELPER_H_
#define _RESOLUTIONHELPER_H_

// Define our Manic Miner Pixel-To-Metre Ratio ( 60 for Full HD 1920x1080 )
// 60 Pixels = 1 box2D unit
#ifndef MANIC_PTM_RATIO
#define MANIC_PTM_RATIO ( 60.0f )
#endif //#ifndef MANIC_PTM_RATIO

// -------------------------------------------------------------------------------------------------------------------- //
// The following code was adapted from the following link:																//
// https://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c										//
// -------------------------------------------------------------------------------------------------------------------- //
#include "wtypes.h"
#include < iostream >
using namespace std;

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetDesktopResolution																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Detects main display's resolution, stores the dimensions as 2 integers								//
//					To be implemented: should redefine the MANIC_PTM_RATIO												//
//					by switching between the 3 supported resolutions ( FHD, WQHD, UDH )									//
//																														//
// Parameters	:	none																								//
//																														//
// See also		:	https://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c						//
// -------------------------------------------------------------------------------------------------------------------- //
void GetDesktopResolution()
{
	RECT desktop;

	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();

	// Get the size of screen to the variable desktop
	GetWindowRect( hDesktop, &desktop );

	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// ( iHorizontalSize, iVerticalSize )
	int iHorizontalSize = desktop.right;
	int iVerticalSize = desktop.bottom;
	
	//////////////////////////////////////////////////////
	// Add logic to adjust PTM accordingly				//
	//////////////////////////////////////////////////////
}

#endif //#ifndef _RESOLUTIONHELPER_H_