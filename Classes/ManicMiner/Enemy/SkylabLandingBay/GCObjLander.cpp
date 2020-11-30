////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October & Henrique Teixeira 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include < string.h >
#include "GCObjLander.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

USING_NS_CC;
// Declare PTM
static const float s_fPTM = 60.0f;

// Top Pixel Y Coordinate for landers
static const float s_fMaxVisibleY = 960.0f - s_fPTM;

// How long each explosion takes
static const float s_fMaxExplosionTime = 0.5f;

CGCObjLander::CGCObjLander		( const int( &paiDeploySlots )[4], const int kiExplosionYSlot, const float kfTimeForFirstCollision, CGCFactoryCreationParams& ParamsInput )
	: CGCObjSpritePhysics		( GetGCTypeIDOf( CGCObjLander ) )
	, m_eLanderState			( ELanderState::Deploying		)
	, m_rFactoryCreationParams	( ParamsInput					)
	, m_kaiDeployXSlots			{ paiDeploySlots[0], paiDeploySlots[1], paiDeploySlots[2], paiDeploySlots[3] }
	, m_kiExplosionYSlot		( kiExplosionYSlot				)
	, m_iCurrentSlotIndex		( 0								)
	, m_fSpeed					( 0.0f							)
	, m_fCurrentExplosionTime	( 0.0f							)
{
	// Get the pixel X coordinate of the first deploy position
	const float fInitialX = s_fPTM * m_kaiDeployXSlots[0];
	// Set reset position to be first deploy position
	Vec2 v2InitialPos( fInitialX, s_fMaxVisibleY );
	SetResetPosition( v2InitialPos );

	// Calculate speed
	// Our landers are 2 blocks high
	const int kiLanderHeight = 2;
	// Top block is 15 [0, 15]
	const int kiTopBlock = 15;
	// Because we adjusted PTM to 60.0f, 1 Box2D unit is 1 block
	// The distance for the first collision is the following equation
	const int kiFirstCollisionDistanceB2Units = kiTopBlock - kiLanderHeight - m_kiExplosionYSlot;
	// Our speed is this distance, divided by the time that it takes for the collision to happen
	// Turned negative, as the landers move downwards
	m_fSpeed = ( kiFirstCollisionDistanceB2Units / kfTimeForFirstCollision ) * -1.0f;
}



//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjLander::~CGCObjLander()
{}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity, reset position and 
// initial facing direction.
//////////////////////////////////////////////////////////////////////////
// virtual function
//////////////////////////////////////////////////////////////////////////
void CGCObjLander::VOnResourceAcquire( void )
{

	// Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjLander );    
	VHandleFactoryParams( m_rFactoryCreationParams, GetResetPosition() );

	// Call base class verion.
	CGCObjSpritePhysics::VOnResourceAcquire();

	if( m_pszAnimation.length() > 0 )
	{

		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();

		std::string m_pszPlist = pcCreateParams->strPlistFile;

		// Note m_pszAnimation is sourced from the data file so not set here.
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( m_pszPlist );
		pAnimation = GCCocosHelpers::CreateAnimation( rdictPList, m_pszAnimation );
		pAnimation->retain();

		RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );


		// use below as data driven from OGMO to set animation speed if required?
		//pAnimation->setDelayPerUnit(0.0f);

	}
	SetVisible( false );
}

void CGCObjLander::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();

	// Reset state info
	m_eLanderState = ELanderState::Deploying;
	m_iCurrentSlotIndex = 0;
}

//////////////////////////////////////////////////////////////////////////
// This function is called when an enemy is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnResurrected( void )
{
	SetResetPosition( cocos2d::Vec2( 0.0f, 0.0f ) );
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
	SetVelocity( cocos2d::Vec2( 0.0f, m_fSpeed ) );
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjLander::VOnUpdate( float fTimeStep )
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnUpdate( fTimeStep );

	// Switch behaviour depending on state
	switch( m_eLanderState )
	{
	// ----------------------------------------------------------------------------------------	//
	// RESETTING	---------------------------------------------------------------------------	//
	// ----------------------------------------------------------------------------------------	//
	case ELanderState::Resetting:
	{
		// Get the pixel X coordinate of the goal deploy position
		const float kfNextDeployX = s_fPTM * m_kaiDeployXSlots[m_iCurrentSlotIndex];

		// Set a Vec2 for the position we want our lander to be in to deploy
		const Vec2 kv2DeployPos( kfNextDeployX, s_fMaxVisibleY );


		// How far off are we from Goal X
		float fCurrentXOffset = ( GetSpritePosition() - kv2DeployPos ).x;
		// Enforce positive value for the if statement
		if( fCurrentXOffset < 0.0f )
		{
			fCurrentXOffset *= -1.0f;
		}

		// How far off are we from Goal Y
		float fCurrentYOffset = ( GetSpritePosition() - kv2DeployPos ).y;
		// Enforce positive value for the if statement
		if( fCurrentYOffset < 0.0f )
		{
			fCurrentYOffset *= -1.0f;
		}

		// Secure spawn position accuracy of at least 90%
		// This means that once the lander is on the correct block, 
		// it must be within a margin of 6px off from the centre of the block
		const float kfMaxOffset = s_fPTM * 0.1f;


		// If both our X and Y are close enough to the Goal X and Goal Y
		if( fCurrentXOffset <= kfMaxOffset &&
			fCurrentYOffset <= kfMaxOffset )
		{
			// Start "Deploying"
			m_eLanderState = ELanderState::Deploying;
			SetVelocity( cocos2d::Vec2( 0.0f, m_fSpeed ) );
			SetVisible( true );
		}
		// Else, we are not close enough yet
		else
		{
			// Adjust the velocity one more time, to get closer to the goal
			const Vec2 kv2CurrentPosition = GetSpritePosition();
			Vec2 v2MoveIn1Frame = ( kv2DeployPos - kv2CurrentPosition );
			SetVelocity( v2MoveIn1Frame );
		}
	}
	break;
	// ----------------------------------------------------------------------------------------	//
	// DEPLOYING ------------------------------------------------------------------------------	//
	// ----------------------------------------------------------------------------------------	//
	case ELanderState::Deploying:
	{
		// Get pixel coordinate of where the lander will explode
		const float fExplosionY = s_fPTM * ( m_kiExplosionYSlot + 2 );
		// If lander has reached the explosion Y coordinate
		if( GetSpritePosition().y <= fExplosionY )
		{
			// Change state
			m_eLanderState = ELanderState::Exploding;
			// Stop moving
			SetVelocity( cocos2d::Vec2::ZERO );
			// TODO: add explosion animation
		}
	}
	break;
	// ----------------------------------------------------------------------------------------	//
	// EXPLODING ------------------------------------------------------------------------------	//
	// ----------------------------------------------------------------------------------------	//
	case ELanderState::Exploding:
	{
		// Update explosion timer
		m_fCurrentExplosionTime += fTimeStep;

		// If timer is over
		if( m_fCurrentExplosionTime >= s_fMaxExplosionTime )
		{
			// Reset timer
			m_fCurrentExplosionTime = 0.0f;

			// Go to next slot
			m_iCurrentSlotIndex++;

			// Reset to first slot if needed
			if( m_iCurrentSlotIndex == 4 )
			{
				m_iCurrentSlotIndex = 0;
			}

			// Change state
			m_eLanderState = ELanderState::Resetting;

			// Make it invisible, as it will be moving towards
			// the next slot for a few frames
			SetVisible( false );

			// Adjust velocity so that it moves towards the next deployment position
			// Get next pixel X coordinate for deployment
			const float kfNextDeployX = s_fPTM * m_kaiDeployXSlots[m_iCurrentSlotIndex];
			const Vec2 kv2NextDeployPos( kfNextDeployX, s_fMaxVisibleY );

			// Set lander velocity, should be able to travel the entire distance in 1 frame
			// But for some reason box2d doesn't allow it so it actually doesn't
			const Vec2 kv2MoveIn1Frame = ( kv2NextDeployPos - GetSpritePosition() ) / fTimeStep;
			SetVelocity( kv2MoveIn1Frame );
		}
	}
	break;
	}
}

void CGCObjLander::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();

	if( m_pszAnimation.length() > 0 )
	{
		pAnimation->release();
		pAnimation = nullptr;
	}
}

ELanderState CGCObjLander::GetState() const
{
	return m_eLanderState;
}