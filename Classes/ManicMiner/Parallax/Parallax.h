#ifndef _PARALLAX_H_
#define _PARALLAX_H_

class CAnimatedParallaxLayer;
class CGCObjSprite;
class CPlayer;
class CParallaxLayer;
class CScrollingParallaxLayer;
struct SParallaxLayerData;

namespace cocos2d
{
	class Scene;
}

class CParallax
{
public:
	CParallax( const int iNumScrollingLayers, const int iNumAnimatedLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer );
	virtual ~CParallax();

	void AddScrollingLayer( const SParallaxLayerData& rsLayerData, const float kfMovementScale );
	void AddPlatformOutlines( const char* kpszPlatformOutlinesPlist);
	void Update();
	void Reset();

	cocos2d::Scene& GetScene() const;
private:
	cocos2d::Scene& m_rcScene;
	CPlayer& m_rcPlayer;

	const int m_kiNumScrollingLayers;
	const int m_kiNumAnimatedLayers;
	CScrollingParallaxLayer* m_pacScrollingLayers;
	CGCObjSprite* m_pcPlatformOutlineSprite; // Unique to each level
	//CAnimatedParallaxLayer* m_pacAnimatedLayers;
	// Used only for init purposes
	int m_iCurrentLayer;
};
#endif // #ifndef _PARALLAX_H_