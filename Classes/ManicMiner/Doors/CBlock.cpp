#include "CBlock.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

// Simple constructor that sets the CreationsParams and the reset position
CBlock::CBlock( CGCFactoryCreationParams& rcCreationParams, cocos2d::Vec2 v2ResetPosition )
  : CGCObjSpritePhysics( GetGCTypeIDOf( CBlock ) )
	, m_rFactoryCreationParams	( rcCreationParams	)
	, m_v2ResetPosition			( v2ResetPosition	)
	, m_panimDissolve			( nullptr			)
{
	
}

void CBlock::DissolveBlock()
{
	// De-activate the physics body and give it an offscreen location to avoid collision issues
	GetPhysicsBody()->SetActive( false );
	SetPhysicsTransform( cocos2d::Vec2( 2000.0f, 2000.0f ), 0.0f );

	
	if ( false )
	{
		// Animation stuff, un-note when we have an animation
		// Load in the .Plist
		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
		std::string m_pszPlist = pcCreateParams->strPlistFile;
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( m_pszPlist );

		// Use this animation
		const char* m_pszAnimation = "Dissolve";

		// Load it in and play
		m_panimDissolve = GCCocosHelpers::CreateAnimation( rdictPList, m_pszAnimation );
		m_panimDissolve->retain();
		RunAction( GCCocosHelpers::CreateAnimationActionOnce( m_panimDissolve ) );
	}
	
	// If the animation isn't present, set the opacity to 0
	if (m_panimDissolve == nullptr)
	{
		SetSpriteOpacity( 0 );
	}
}

void CBlock::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	SetSpriteOpacity( 1000 );	
}

void CBlock::VOnResourceAcquire()
{
	VHandleFactoryParams( m_rFactoryCreationParams, GetResetPosition() );
	CGCObjSpritePhysics::VOnResourceAcquire();
	SetResetPosition( m_v2ResetPosition );
}