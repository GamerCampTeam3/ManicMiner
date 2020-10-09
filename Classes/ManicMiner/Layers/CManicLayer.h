#ifndef _CMANICLAYER_H_
#define _CMANICLAYER_H_

#include <stdlib.h>

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

////////////////////////////////////////////////////////////////////////////
// Forward Declarations													  //
////////////////////////////////////////////////////////////////////////////
class CGameInstance;													  //
class CGCObjSprite;														  //
class CGCObjPlayer;														  //
class CGCObjPlatform;													  //
class CGCObjGroupPlatform;												  //
class CGCObjItem;														  //
class CGCObjGroupItem;													  //
class CGCObjInvader;													  //
class CGCObjGroupInvader;												  //
class CGCObjGroupProjectilePlayer;										  //
class CGCObjEnemy;													  //
class CGCObjGroupEnemy;													  //
////////////////////////////////////////////////////////////////////////////

class CManicLayer : public IGCGameLayer, public b2ContactListener
{
private:
	// Reference to the GameInstance
	CGameInstance& m_rGameInstance;

	// object groups
	CGCObjGroupPlatform* m_pcGCGroupPlatform;
	CGCObjGroupItem* m_pcGCGroupItem;
	CGCObjGroupProjectilePlayer* m_pcGCGroupProjectilePlayer;
	CGCObjGroupEnemy* m_pcGCGroupEnemy;

	// backgrounds
	CGCObjSprite* m_pcGCSprBackGround;

	// Mario
	CGCObjPlayer* m_pcGCOPlayer;

public:
	CManicLayer( void );
	CManicLayer( CGameInstance& rGameInstance );
	virtual ~CManicLayer( void );

	//////////////////////////////////////////////////////////////////////////
	// player actions 
	enum EPlayerActions
	{
		EPA_Up = 0,
		EPA_Down,
		EPA_Left,
		EPA_Right,
		EPA_Fire
	};
	// player actions 
	//////////////////////////////////////////////////////////////////////////

	// called from VOnUpdate
	void HandleCollisions( void );

	//////////////////////////////////////////////////////////////////////////
	// CCNode interface...
	virtual void onEnter();
	// CCNode interface...
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer interface

	virtual	void VOnCreate( void )			override;
	virtual void VOnUpdate( f32 fTimeStep )	override;
	virtual	void VOnDestroy( void )			override;


	// IGCGameLayer interface
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// b2ContactListener interface - see b2ContactListener for details of 
	// when these get called and what they are

	virtual void BeginContact( b2Contact* pB2Contact );
	virtual void EndContact( b2Contact* pB2Contact );
	virtual void PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold );
	virtual void PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse );

	void PlayerCollidedInvader( CGCObjPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact );
	void PlayerCollidedEnemy(CGCObjPlayer& rPlayer, CGCObjEnemy& rEnemy, const b2Contact& rcContact);

	void ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact );


	////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////

	CGCObjPlayer& GetPlayer();


};
#endif // #ifndef _CMANICLAYER_H_