// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _PARALLAX_LAYER_DATA_H_
#define _PARALLAX_LAYER_DATA_H_

struct SParallaxLayerData
{
	SParallaxLayerData( const char* kpszPlist_image, const int kiZOrder )
		: kpszPlist_image( kpszPlist_image )
		, kiZOrder( kiZOrder )
	{}
	// Layer Plist
	const char* kpszPlist_image;
	// Depth / Z-Order
	const int kiZOrder;
};
#endif // #ifndef _PARALLAX_LAYER_DATA_H_