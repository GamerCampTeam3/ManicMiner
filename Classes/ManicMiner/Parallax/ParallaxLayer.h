// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira & Umeer Rama																		//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _PARALLAX_LAYER_H_
#define _PARALLAX_LAYER_H_

#include "ParallaxLayerData.h"
#include "2d/CCSprite.h"

class CGCObjSprite;
namespace cocos2d
{
	class Scene;
	class Sprite;
}

// Abstract
class CParallaxLayer
{
public:
	CParallaxLayer();
	virtual ~CParallaxLayer();

	// Init -> Creates the SParallaxLayerData and CGCObjSprite, sets layer properties
	void Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData );
	
	// Purely Virtual Update and Reset
	virtual void VUpdate() = 0;
	virtual void VReset() = 0;

	// Sprite Getter
	cocos2d::Sprite* GetSprite() const;
	// Data Getter
	const SParallaxLayerData* GetData() const;

private:
	CGCObjSprite* m_pcSprite;

	// Needs to be pointer because it contains a const char* for plist
	SParallaxLayerData* m_psData;
};
#endif // #ifndef _PARALLAX_LAYER_H_