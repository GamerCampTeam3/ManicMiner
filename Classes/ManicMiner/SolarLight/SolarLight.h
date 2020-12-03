#ifndef _SOLAR_LIGHT_H
#define _SOLAR_LIGHT_H

class CAirManager;
class TexturedSpline;
class CMLSolarPowerGenerator;
class CSolarLightB2RayCastCallBack;


class CSolarLight
{
public:
	CSolarLight( CMLSolarPowerGenerator& rcSolarLevel );
	virtual ~CSolarLight();
	void Update();
	void Init( CMLSolarPowerGenerator& rcSolarLevel );
private:
	void CheckCollisions();
	void BuildNewSpline();

	b2World& m_rcB2World;
	CAirManager& m_rcAirManager;
	TexturedSpline* m_pcTexturedSpline;
	const int m_kiLightRoundness;
	std::vector< cocos2d::Vec2 > m_vPath;
	const int m_kiStartX;
	const int m_kiStartY;
};
#endif