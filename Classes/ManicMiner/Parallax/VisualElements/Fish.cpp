
#ifndef _FISH_H_
#include "Fish.h"
#endif
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"

const char* pszPlist_Fish = "TexturePacker/Sprites/Fish/Fish.plist";

CFish::CFish()
	: CGCObject(GetGCTypeIDOf(CFish))
	, m_pcSprFish				( nullptr )
	, m_v2CurrentMoveToLocation	( cocos2d::Vec2(0.f, 0.f) )
	, m_v2NextMoveToLocation	( cocos2d::Vec2(0.f, 0.f) )
{
	
}

CFish::~CFish()
{
	if(nullptr != m_pcSprFish)
	{
		m_pcSprFish->GetSprite()->release();
		delete m_pcSprFish;
		m_pcSprFish = nullptr;
	}
	m_v2CurrentMoveToLocation = cocos2d::Vec2(0.f, 0.f);
	m_v2NextMoveToLocation = cocos2d::Vec2(0.f, 0.f);
}

void CFish::VOnResourceAcquire()
{
}

void CFish::VOnResourceRelease()
{
}

void CFish::VOnReset()
{
	m_v2CurrentMoveToLocation = cocos2d::Vec2(0.f, 0.f);
	m_v2NextMoveToLocation = cocos2d::Vec2(0.f, 0.f);
}

void CFish::VOnResurrected()
{
}

void CFish::VOnUpdate(float fTimeStep)
{
}

void CFish::Init()
{
	if(nullptr == m_pcSprFish)
	{
		m_pcSprFish = new CGCObjSprite();
	}

	if(nullptr != m_pcSprFish)
	{
		if (nullptr == m_pcSprFish->GetSprite())
		{
			m_pcSprFish->CreateSprite(pszPlist_Fish);
		}

		m_pcSprFish->SetSpritePosition(cocos2d::Vec2(380.f, 600.f));
	}

}
