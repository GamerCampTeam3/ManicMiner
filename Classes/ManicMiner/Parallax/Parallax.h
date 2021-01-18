// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira & Umeer Rama																		//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose: This class manages the creation of Parallax Layers and Animated Background Components						//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _PARALLAX_H_
#define _PARALLAX_H_

// -- Fwd Declares ---------------------------------------------------------------------------------------------------- //
class CAnimatedParallaxLayer;																							//
class CGCObjSprite;																										//
class CPlayer;																											//
class CParallaxLayer;																									//
class CScrollingParallaxLayer;																							//
struct SParallaxLayerData;																								//
namespace cocos2d																										//
{																														//
	class Scene;																										//
}																														//
// -------------------------------------------------------------------------------------------------------------------- //

class CParallax
{
public:
// Constructor
	CParallax( const int iNumScrollingLayers, const int iNumAnimatedLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer );
// Deconstructor
	virtual ~CParallax();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	AddScrollingLayer																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates and adds a Parallax Layer with the given rsLayerData to current scene						//
//																														//
// Parameters	:	const SParallaxLayerData& rsLayerData																//
//					Image / PList and Z-Order for specific Parallax Layer												//
//																														//
//					const float kfMovementScale																			//	
//					Scalar that will affect how much a layer moves left / right as the player moves						//
//					in opposite direction, creating an illusion of depth and perspective								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	SParallaxLayerData and CManicLayer::VInitializeBackground()											//
// -------------------------------------------------------------------------------------------------------------------- //
	void AddScrollingLayer( const SParallaxLayerData& rsLayerData, const float kfMovementScale );

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	AddAnimatedLayer																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates and adds an Animated Parallax Layer with the given rsLayerData to current scene				//
//																														//
// Parameters	:	const SParallaxLayerData& rsLayerData																//
//					Image / PList and Z-Order for specific Animated Parallax Layer										//
//																														//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void AddAnimatedLayer(const SParallaxLayerData& rsLayerData);

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	AddPlatformOutlines																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates and adds a Parallax Layer with the given kpszPlatformOutlinesPlist directory				//
//																														//
// Parameters	:	const char* kpszPlatformOutlinesPlist																//
//					Directory for the platform outlines																	//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //	
	void AddPlatformOutlines( const char* kpszPlatformOutlinesPlist);
	
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Update																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Updates the scrolling and the animated parallax layers, runs once per frame							//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //	
	void Update();
	
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Reset																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Resets the scrolling and the animated parallax layers to their initial positions					//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //	
	void Reset();

// Scene Getter
	cocos2d::Scene& GetScene() const;
private:
// Reference to owning Scene, needed to add each layer into the scene
	cocos2d::Scene& m_rcScene;

// Reference to player, needed to adjust the position of the scrolling layers
	CPlayer& m_rcPlayer;

// Number of Scrolling Layers
	const int m_kiNumScrollingLayers;

// Number of Animated Layers
	const int m_kiNumAnimatedLayers;

// Array of Scrolling Parallax Layers
	CScrollingParallaxLayer* m_pacScrollingLayers;

// Sprite for platform outlines (if existent for this level)
	CGCObjSprite* m_pcPlatformOutlineSprite;

// Array of Animated Parallax Layers
	CAnimatedParallaxLayer* m_pacAnimatedLayers;

// Used only for background initialisation purposes
	int m_iCurrentLayer;
};
#endif // #ifndef _PARALLAX_H_