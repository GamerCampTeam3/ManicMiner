// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	CManicLayer is a child of IGCGameLayer																//
//					it serves as the parent class for every level, such as CMLCentralCavern.							//
//					Each level of Manic Miner just needs to inherit from CManicLayer									//
//					and override both VOnCreate() and VOnDestroy(), in order to set up their unique map layout			//
//																														//
//					CManicLayer is also a child of b2ContactListener													//
//					meaning it is capable of handling all the collisions that happen in this layer.						//
//					This is important because we will need to operate these collisions for essential game logic			//
//					i.e. player death, collectible pick ups, and player-platform collisions								//
//																														//
// See also		:	IGCGameLayer & b2ContactListener & CMLCentralCavern & CLevelManager									//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _CMANICLAYER_H_
#define _CMANICLAYER_H_

#include < stdlib.h >

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/GameState/EGameState.h"
#include "ManicMiner/Structs/SLevelCreationParameters.h"


#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

// param0 = X, param1 = Y
#define CC_V2(param0, param1)  cocos2d::Vec2(param0, param1) 

// ----------------------------------------- Fwd declares ------------------------------------------------------------- //
class CCollectible;																										//
class CDoor;																											//
class CGameManager;																									    //
class CGCObjEnemy;																										//
class CGCObjHazard;																										//
class CLevelManager;																									//
class CParallax;																										//
class CPlatform;																										//
class CPlayer;																											//
class CSwitch;																											//
// -------------------------------------------------------------------------------------------------------------------- //


class CManicLayer: public IGCGameLayer, public b2ContactListener
{
// Bib Edit ----------------------------------------------------------------------------------------------------------- //
protected:																												//
	CGameManager*				m_pcGameManager;
	SLevelCreationParameters	m_sLevelCreationParameters;																//
	cocos2d::Size				m_sizeVisible;																			//
	cocos2d::Point				m_pointOrigin; 																			//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
	
private:
// Henrique Edit ------------------------------------------------------------------------------------------------------ //
																														//
// Reference to the LevelManager in order to proceed																	//
	CLevelManager*		m_pcLevelManager;																				//
																														//
																														//
// Level Loader Instance																								//	
	CGCLevelLoader_Ogmo	m_cLevelLoader;																					//
																														//
// GameState Enum, opens door and may change enemy behaviour															//
	EGameState			m_eGameState;																					//
																														//
// Handling Reset Flag																									//
	bool				m_bWasResetRequested;																			//
																														//
// Handling Going to Next Level Flag																					//
	bool				m_bWasNextLevelRequested;																		//
																														//
// Pointer to Background Sprites																						//
	CParallax*			m_pcParallax;																					//
																														//
// Pointer to player, for easy play access																				//
	CPlayer*			m_pcPlayer;																						//
// -------------------------------------------------------------------------------------------------------------------- //


public:
// Constructor -------------------------------------------------------------------------------------------------------- //
	CManicLayer( void );
// Destructor  -------------------------------------------------------------------------------------------------------- //
	virtual ~CManicLayer( void );




// ------------------------------------ IGCGameLayer Interface -------------------------------------------------------- //
	virtual void VOnUpdate		( f32 fTimeStep )	override;															//
	virtual	void VOnCreate		( void )			override;															//
	virtual	void VOnDestroy		( void )			override;															//
	virtual void VOnReset		( void )			override;															//
// -------------------------------------------------------------------------------------------------------------------- //





// ---------------------------------- b2ContactListener Interface ----------------------------------------------------- //
	virtual void BeginContact	( b2Contact* pB2Contact );																//
	virtual void EndContact		( b2Contact* pB2Contact );																//
	virtual void PreSolve		( b2Contact* pB2Contact, const b2Manifold* pOldManifold );								//
// -------------------------------------------------------------------------------------------------------------------- //






// ---------------------------------- Collision Event Handlers -------------------------------------------------------- //
	void EnemyCollidedPlatform	( CGCObjEnemy&	rcEnemy,									const b2Contact& rcContact	);
	void PlayerCollidedEnemy	( CPlayer&		rcPlayer,		CGCObjEnemy&	rcEnemy,	const b2Contact& rcContact	);
	void PlayerCollidedHazard	( CPlayer&		rcPlayer,		CGCObjHazard&	rcHazard,	const b2Contact& rcContact	);
	void PlayerCollidedDoor		( CPlayer&		rcPlayer,		CDoor&			rcDoor,		const b2Contact& rcContact	);
	void ItemCollected			( CCollectible& rcCollectible,	CPlayer&		rcPlayer,	const b2Contact& rcContact	);
	void SwitchInteracted		( CSwitch&		rcSwitch,		CPlayer& rcPlayer,			const b2Contact& rcContact	);
// -------------------------------------------------------------------------------------------------------------------- //







// ---------------------------------- Getters ------------------------------------------------------------------------- //
	CPlayer&			GetPlayer()						const;															//
	CLevelManager&		GetLevelManager()				const;															//
	const EGameState	GetGameState()					const;															//
	bool				GetWasResetRequested()			const;															//
	bool				GetWasNextLevelRequested()		const;															//
	CParallax&			GetParallax()					const;															//
// -------------------------------------------------------------------------------------------------------------------- //



// ---------------------------------- Setters ------------------------------------------------------------------------- //
	void SetLevelManager( CLevelManager&		rcLevelManager	);														//
	void SetGameState	( const EGameState		gameState		);														//
	void SetGameManager	( CGameManager&			rcGameManager );														//
// -------------------------------------------------------------------------------------------------------------------- //






// ------------------------------------ Bib Edit ---------------------------------------------------------------------- //
public:																													//
																														//
	//////////////////////////////////////////////////////////////////////////											//
	// player actions 																									//
	enum EPlayerActions																									//
	{																													//
		EPA_Up = 0,																										//
		EPA_Down,																										//
		EPA_Left,																										//
		EPA_Right,
		EPA_Cheat,//
		EPA_Jump																										//
	};																													//																						//
																														//
	////////////////////////////////////////////////////////////////////////////////									//
	// CCNode interface onEnter -> We override to initialize the keyboard manager										//
	virtual void onEnter() override;																					//
	////////////////////////////////////////////////////////////////////////////////									//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //





// ------------------------------------ Henrique Edit ----------------------------------------------------------------- //
public:																													//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// --------------------------------- Game State & Player Related Logic ------------------------------------------------ //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnFinishedLooting																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_eGameState to EGameState::Escaping, changes enemies behaviours if applicable					//
//					Called when player finishes collecting all collectibles												//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OnFinishedLooting();																							//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnEscaped																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_eGameState to EGameState::Victory															//
//					Will also play the air manager animation and points increasing [ to be implemented ]				//
//					Requests next level transition																		//
//					Called when player reaches the door after finishing the looting state								//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OnEscaped();																									//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnDeath																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Decreases player life count and then checks if should reset level or go back to Main Menu ( LOSE )	//
//					Called when player dies ( collided with hazard or enemy or ran out of air )							//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See also		:	CPlayer																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OnDeath();																										//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OutOfLives																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Forces player back to the main menu																	//
//					Called when player loses all 3 lives																//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void OutOfLives();																									//
																														//
																														//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// --------------------------------- Level Reset Handling Logic ------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	RequestReset																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_bWasResetRequested to true, called when the player dies and layer needs resetting			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void RequestReset();																								//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetRequestWasHandled																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_bWasResetRequested to false, called when the layer is done resetting after a request			//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void ResetRequestWasHandled();																						//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetLevel																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Effectively resets this CManicLayer, calling VOnReset() and ResetRequestWasHandled()				//
//					Called from the update loop if level needs resetting												//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void ResetLevel();																									//
																														//
																														//
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------- Loading Next Level Handling Logic ------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
																														//
																														//
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	RequestNextLevel																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_bWasNextLevelRequested to true, so that by the end of this update loop						//
//					the CLevelManager will GoToNextLevel()				 												//
//					Called from OnEscaped() ( when player escapes through the door )									//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void RequestNextLevel();																							//
																														//
																														//
																														//
// End of Henrique edit ----------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------------------- //

private:
	void PlayerBeganContactWithPlatform( CPlatform& rcPlatform );

// Umeer placed this nice button on the top right corner of the screen ------------------------------------------------ //
// On click, request next level to be loaded
	void CB_OnGameExitButton( Ref* pSender );

public:
	virtual void VInitParams() = 0;
	void PostInit();

protected:
	virtual void VInitializeBackground();
};
#endif // #ifndef _CMANICLAYER_H_