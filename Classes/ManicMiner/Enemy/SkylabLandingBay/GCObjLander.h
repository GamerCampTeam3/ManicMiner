////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJLANDER_H_
#define _GCOBJLANDER_H_

//#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
//#include "../../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
//#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//#include "ManicMiner/Enums/ELanderTypes.h"

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//  Its purpose is to provide the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjLander
: public CGCObjSpritePhysics
{
private:

	enum class ELanderState {
		EWaitingToDeploy,
		EDeploying,

	};



	float m_currentTime;



	ELanderState				m_cLanderState;

	cocos2d::Vec2				m_cAnchorPoint;
	float						m_fSpeed;
    float 						m_fStartDelay;
	float 						m_fReDeployDelay;



	std::string					m_pszAnimation;
	std::string                 m_pszPlist;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

	cocos2d::Vec2				m_cDest;
	cocos2d::Vec2				m_cCurrentPos;
	float m_fMoveDelta;




	//std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;
	cocos2d::Animation* pAnimation;
		
public:

	//CGCObjLander();

	//CGCObjLander(GCTypeID idDerivedType);


	CGCObjLander(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, const float fStartDelay, const float fReDeployDelay, CGCFactoryCreationParams& ParamsInput);
		//CGCFactoryCreationParams& ParamsInput);

	//GCFACTORY_DECLARE_CREATABLECLASS(CGCObjLander);
	
	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjLander();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceRelease	() override;
	virtual void VOnReset() override;

	//virtual void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;


};
#endif // #ifndef _GCOBJLANDER_H_