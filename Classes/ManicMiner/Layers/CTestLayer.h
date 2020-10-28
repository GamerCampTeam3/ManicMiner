// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
//	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:		CTestLayer is the copy of GCGameLayerPlatformer, used to start testing our own logic and classes	//
//					There really is no point in even checking it out.													//																														//
//																														//
//					Completely commented out and excluded from the build in project settings							//
//					Only exists still for submission purposes															//
// -------------------------------------------------------------------------------------------------------------------- //




//#ifndef _CTESTLAYER_H_
//#define _CTESTLAYER_H_
//
//#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
//
//
//
//////////////////////////////////////////////////////////////////////////////
//// Forward Declarations													  //
//////////////////////////////////////////////////////////////////////////////
//class CGCObjSprite;														  //
//class CPlayer;														  //
//class CGCObjPlatform;													  //
//class CGCObjGroupPlatform;												  //
//class CGCObjItem;														  //
//class CGCObjGroupItem;													  //
//class CGCObjInvader;													  //
//class CGCObjGroupInvader;												  //
//class CGCObjGroupProjectilePlayer;										  //
//////////////////////////////////////////////////////////////////////////////
//
//class CTestLayer: public IGCGameLayer, public b2ContactListener
//{
//private:
//	// object groups
//	CGCObjGroupPlatform* m_pcGCGroupPlatform;
//	CGCObjGroupItem* m_pcGCGroupItem;
//	CGCObjGroupInvader* m_pcGCGroupInvader;
//	CGCObjGroupProjectilePlayer* m_pcGCGroupProjectilePlayer;
//
//	// backgrounds
//	CGCObjSprite* m_pcGCSprBackGround;
//
//	// Mario
//	CPlayer* m_pcGCOPlayer;
//
//	////////////////////////////////////////////////////////////////////////// 
//	// reset handling
//	bool m_bResetWasRequested;
//
//	void RequestReset()
//	{
//		m_bResetWasRequested = true;
//	}
//
//	void ResetRequestWasHandled()
//	{
//		m_bResetWasRequested = false;
//	}
//
//	bool ResetWasRequested()
//	{
//		return m_bResetWasRequested;
//	}
//public:
//	CTestLayer( void );
//	virtual ~CTestLayer( void );
//
//	//////////////////////////////////////////////////////////////////////////
//	// player actions 
//	enum EPlayerActions
//	{
//		EPA_Up = 0,
//		EPA_Down,
//		EPA_Left,
//		EPA_Right,
//		EPA_Fire
//	};
//	// player actions 
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	// 'selector' callbacks for menu buttons
//	void Callback_OnQuitButton ( Ref* pSender );
//	void Callback_OnResetButton( Ref* pSender );
//
//	// called from VOnUpdate
//	void HandleCollisions( void );
//
//	//////////////////////////////////////////////////////////////////////////
//	// CCNode interface...
//	virtual void onEnter();
//	// CCNode interface...
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	// IGCGameLayer interface
//
//	virtual	void VOnCreate	( void )			override;
//	virtual void VOnUpdate	( f32 fTimeStep )	override;
//	virtual	void VOnDestroy	( void )			override;
//
//	// IGCGameLayer interface
//	//////////////////////////////////////////////////////////////////////////
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// b2ContactListener interface - see b2ContactListener for details of 
//	// when these get called and what they are
//
//	virtual void BeginContact(	b2Contact* pB2Contact );
//	virtual void EndContact  (	b2Contact* pB2Contact );
//	virtual void PreSolve	 (	b2Contact* pB2Contact, const b2Manifold* pOldManifold	);
//	virtual void PostSolve	 (	b2Contact* pB2Contact, const b2ContactImpulse* pImpulse );
//
//	void PlayerCollidedInvader(CPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact );
//	void ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact );
//};
//
//#endif //#ifndef _CTESTLAYER_H_