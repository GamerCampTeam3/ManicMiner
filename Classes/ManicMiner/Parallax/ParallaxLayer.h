#ifndef _PARALLAX_LAYER_H_
#define _PARALLAX_LAYER_H_

#include "ParallaxLayerData.h"

class CGCObjSprite;
namespace cocos2d
{
	class Scene;
	class Sprite;
}

class CParallaxLayer
{
public:
	CParallaxLayer();
	virtual ~CParallaxLayer();

	virtual void Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData );
	
	// Purely Virtual Update
	virtual void Update() = 0;
	
	cocos2d::Sprite* GetSprite() const;
	const SParallaxLayerData* GetData() const;

private:
	CGCObjSprite* m_pcSprite;
	// Needs to be pointer because it contains a const char* for plist
	SParallaxLayerData* m_psData;
};
#endif // #ifndef _PARALLAX_LAYER_H_