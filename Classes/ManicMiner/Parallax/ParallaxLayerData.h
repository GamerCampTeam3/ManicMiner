#ifndef _PARALLAX_LAYER_DATA_H_
#define _PARALLAX_LAYER_DATA_H_

static const float s_kfMovementScaleMultiplier = 0.0025f;

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
	//// Move scale, only affects horizontal movement
	//// The bigger the value, the more the layer will move
	//// Should be bigger the closer the layers are to the camera
	//const float kfMovementScale;
#endif // #ifndef _PARALLAX_LAYER_DATA_H_