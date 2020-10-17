#ifndef _RESOLUTIONHELPER_H_
#define _RESOLUTIONHELPER_H_


#ifndef MANIC_PTM_RATIO
#define MANIC_PTM_RATIO ( 60.0f )
#endif //#ifndef MANIC_PTM_RATIO


//////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/8690619/how-to-get-screen-resolution-in-c	//
//////////////////////////////////////////////////////////////////////////////////////
#include "wtypes.h"
#include <iostream>
using namespace std;

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution()
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect( hDesktop, &desktop );
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	int horizontal = desktop.right;
	int vertical = desktop.bottom;
}


#endif //#ifndef _RESOLUTIONHELPER_H_