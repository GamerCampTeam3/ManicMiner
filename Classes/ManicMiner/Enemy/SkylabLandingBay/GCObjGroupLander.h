////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPLANDER_H_
#define	_GCOBJECTGROUPLANDER_H_

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif

#include "ManicMiner/Enums/EEnemyTypes.h"
#include <vector>
#include <unordered_map>
#include <queue>

//////////////////////////////////////////////////////////////////////////
// This class is will provide a group of Landers for the Skylab Landing
// Bay level to be introduced at Module 2.
// Note this code is not complete for Module 1.
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupLander
: public CGCObjectGroup
{
private:
	cocos2d::Vec2	m_v2FormationOrigin; // origin of the formation

	void CreateLanders();
	void DestroyLanders();
	void KillLandersNotRequiredAtLevelStartup();
	void RegisterLanderCollision(GCTypeID idLander);
	void ActivateLanders(float fTimeStamp);

	int AllocateRandomLanders(int iNumberToAllocate);
	bool m_bFirstPassDone;
	const int k_iMaxLanderAnchorPoints = 12;
	const int k_iAnchorPointStepDistance = 180;
	const int k_iAnchorPointYPosition = 1000;
	float fStartupCounter;

	struct LanderConfig
	{
		const float fStartDelay;
		const float fRepeatDelay;
	};

	std::vector<LanderConfig> LanderConfigVars;
	std::vector<cocos2d::Vec2> vLanderAnchorPoints;
	std::queue <float> StartupTimingsThresholdQueue;
	std::vector<LanderConfig> LanderConfig;
	std::unordered_map<GCTypeID, int> LanderGCTypeIDLookup ;

	// Note not used in current implementation - as required for the earlier 'random' option.
	std::vector<bool> vLanderActivators;

public:
	
	CGCObjGroupLander();		
	virtual ~CGCObjGroupLander() override;

	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType ) override;
	virtual GCTypeID	VGetTypeId							() override;
	virtual void		VOnGroupResourceAcquire				() override;
	virtual void		VOnGroupResourceAcquire_PostObject	() override;
	virtual void		VOnGroupResourceRelease				() override;
	virtual void		VOnGroupUpdate						(f32 fTimeStep) override;

};

#endif