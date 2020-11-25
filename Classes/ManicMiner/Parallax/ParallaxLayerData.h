#ifndef _PARALLAX_LAYER_DATA_H_
#define _PARALLAX_LAYER_DATA_H_

struct SParallaxLayerData
{
	SParallaxLayerData( const char* kpszPlist_image, const int kiZOrder, const float kfMovementScaled )
		: kpszPlist_image( kpszPlist_image )
		, kiZOrder( kiZOrder )
		, kfMovementScale( kfMovementScaled * 0.0025f )
	{}





	// Layer Plist
	const char* kpszPlist_image;
	// Depth / Z-Order
	const int kiZOrder;
	// Move scale, only affects horizontal movement
	// The bigger the value, the more the layer will move
	// Should be bigger the closer the layers are to the camera
	const float kfMovementScale;
};
#endif // #ifndef _PARALLAX_LAYER_DATA_H_