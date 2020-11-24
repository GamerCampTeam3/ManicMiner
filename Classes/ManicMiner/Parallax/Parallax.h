#ifndef _PARALLAX_H_
#define _PARALLAX_H_

class CPlayer;
class CParallaxLayer;
struct SParallaxLayerData;

namespace cocos2d
{
	class Scene;
}

class CParallax
{
public:
	CParallax( const int iNumLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer );
	virtual ~CParallax();

	void AddLayer( const SParallaxLayerData& rsLayerData );
	void Update();
	void Reset();

	cocos2d::Scene& GetScene() const;
private:
	cocos2d::Scene& m_rcScene;
	CPlayer& m_rcPlayer;

	const int m_kiNumLayers;
	CParallaxLayer* m_pacLayers;
	// Used only for init purposes
	int m_iCurrentLayer;
};
#endif // #ifndef _PARALLAX_H_