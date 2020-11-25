#ifndef _PARALLAX_LAYER_H_
#define _PARALLAX_LAYER_H_

#include "ParallaxLayerData.h"
#include "2d/CCSprite.h"


class CGCObjSprite;
namespace cocos2d
{
	class Scene;
}

class CParallaxLayer
{
public:
	CParallaxLayer();
	virtual ~CParallaxLayer();

	void Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData );
	cocos2d::Sprite* GetSprite() const;
private:
	CGCObjSprite* m_pcSprite;
	// Needs to be pointer because it contains a const char* for plist
	SParallaxLayerData* m_psData;
};
#endif // #ifndef _PARALLAX_LAYER_H_