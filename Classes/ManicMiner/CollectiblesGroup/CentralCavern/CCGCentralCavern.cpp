#include "CCGCentralCavern.h"

#include "Box2D/Dynamics/b2Body.h"
#include "ManicMiner/Collectible/CCollectible.h"
#include "GamerCamp/GCCocosInterface/SGCFactoryCreationParams.h"


//static CGCFactoryCreationParams s_cCreationParamsKey( "Key", "TexturePacker/Sprites/Key/Key.plist", "Key", b2_staticBody, true );

void CCGCentralCavern::GenerateCollectibles()
{
	CCollectible* pCollectible_0 = new CCollectible( m_cCreationParams, ECollectibleType::Collectible, cocos2d::Vec2( 1740.f, 920.f ), *this );
	CCollectible* pCollectible_1 = new CCollectible( m_cCreationParams, ECollectibleType::Collectible, cocos2d::Vec2( 1820.f, 580.f ), *this );
	CCollectible* pCollectible_2 = new CCollectible( m_cCreationParams, ECollectibleType::Collectible, cocos2d::Vec2( 690.f, 920.f ), *this );
	CCollectible* pCollectible_3 = new CCollectible( m_cCreationParams, ECollectibleType::Collectible, cocos2d::Vec2( 940.f, 880.f ), *this );
	CCollectible* pCollectible_4 = new CCollectible( m_cCreationParams, ECollectibleType::Collectible, cocos2d::Vec2( 1520.f, 700.f ), *this );
}

void CCGCentralCavern::DeleteCollectibles()
{
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )
	{
		// do nothing - DestroyObjectsReverseOrder calls delete!
		GCASSERT( GetGCTypeIDOf( CCollectible ) == pObject->GetGCTypeID(), "wrong type!" );

	} );
}

void CCGCentralCavern::VOnGroupResourceRelease()
{
	CGCObjectGroup::VOnGroupResourceRelease();
	DeleteCollectibles();
}

