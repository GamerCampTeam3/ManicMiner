////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJKONG_H_
#define _GCOBJKONG_H_

//#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
//#include "../../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
//#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//  Its purpose is to provide the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjKong
: public CGCObjSpritePhysics
{
private:

	enum class EKongState {
		EWaitingToFall,
		EFalling,

	};



	float m_currentTime;



	EKongState					m_cKongState;

	cocos2d::Vec2				m_cAnchorPoint;
	float						m_fSpeed;
    

	float						m_kfKongFallDuration = 4.0f;



	std::string					m_pszAnimation;
	std::string                 m_pszPlist;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

	cocos2d::Vec2				m_cDest;
	cocos2d::Vec2				m_cCurrentPos;
	float m_fMoveDelta;




	cocos2d::Animation* pAnimation;
		
public:

	

	CGCObjKong(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, CGCFactoryCreationParams& ParamsInput);
		//CGCFactoryCreationParams& ParamsInput);

	//GCFACTORY_DECLARE_CREATABLECLASS(CGCObjKong);
	
	bool m_bKongIsFalling;

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjKong();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceRelease	() override;
	virtual void VOnReset() override;

	//virtual void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	

	void TriggerKongToFall();


};
#endif // #ifndef _GCOBJKONG_H_