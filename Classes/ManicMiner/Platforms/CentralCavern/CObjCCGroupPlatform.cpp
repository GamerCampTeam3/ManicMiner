#include "CObjCCGroupPlatform.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

#include "ManicMiner/Enums/EPlayerMovement.h"
#include "ManicMiner/Platforms/CBrickPlatform.h"
#include "ManicMiner/Platforms/CCrumblingPlatform.h"
#include "ManicMiner/Platforms/CGroundPlatform.h"
#include "ManicMiner/Platforms/CMovingPlatform.h"
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Platforms/CRegularPlatform.h"

// creation params
static CGCFactoryCreationParams s_cCreationParams_Platform_1x1("CPlatform_Regular_1x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_1x1.plist", "CN_Platform_Regular_1x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_2x1("CPlatform_Regular_2x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_2x1.plist", "CN_Platform_Regular_2x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_3x1("CPlatform_Regular_3x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_3x1.plist", "CN_Platform_Regular_3x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_4x1("CPlatform_Regular_4x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_4x1.plist", "CN_Platform_Regular_4x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_8x1("CPlatform_Regular_8x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_8x1.plist", "CN_Platform_Regular_8x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_13x1("CPlatform_Regular_13x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_13x1.plist", "CN_Platform_Regular_13x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_15x1("CPlatform_Regular_15x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Regular/Platform_Regular_15x1.plist", "CN_Platform_Regular_15x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_Ground("CPlatform_Ground", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Ground/Platform_Ground.plist", "CN_Platform_Ground", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_Brick_3x1("CPlatform_Brick_3x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Brick/Platform_Brick_3x1.plist", "CN_Platform_Brick_3x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_Brick_1x16("CPlatform_Brick_1x16", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Brick/Platform_Brick_1x16.plist", "CN_Platform_Brick_1x16", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_Crumbling_1x1("CPlatform_Crumbling_1x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Crumbling/Platform_Crumbling_1x1.plist", "CN_Platform_Crumbling_1x1", b2_staticBody, true);
static CGCFactoryCreationParams s_cCreationParams_Platform_Moving_20x1("CPlatform_Moving_20x1", "TexturePacker/Sprites/Platform/CentralCavern/CentralCavern/Moving/Platform_Moving_20x1.plist", "Platform_Moving_20x1", b2_staticBody, true);

CObjCCGroupPlatform::CObjCCGroupPlatform()
{	
}

CObjCCGroupPlatform::~CObjCCGroupPlatform()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CObjCCGroupPlatform::VHandlesThisTypeId(GCTypeID idQueryType)
{
	return(GetGCTypeIDOf(CPlatform) == idQueryType);
}

//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CObjCCGroupPlatform::VGetTypeId()
{
	return GetGCTypeIDOf(CObjCCGroupPlatform);
}

void CObjCCGroupPlatform::VOnGroupResourceAcquire()
{

	CGCObjectGroup::VOnGroupResourceAcquire();
	
	CreatePlatforms();
}

void CObjCCGroupPlatform::VOnGroupResourceAcquire_PostObject()
{
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

}

void CObjCCGroupPlatform::VOnGroupResourceRelease()
{
	CGCObjectGroup::VOnGroupResourceRelease();

	DestroyPlatforms();
}


void CObjCCGroupPlatform::CreatePlatforms()
{
		// ground platform
	CPlatform* pcGroundPlatform = new CGroundPlatform(s_cCreationParams_Platform_Ground, cocos2d::Vec2(960.f,30.f) );

		// regular platform
	CPlatform* pcPlatform1st = new CRegularPlatform(s_cCreationParams_Platform_15x1, cocos2d::Vec2(750.f, 150.f) );

		// brick platform
	CPlatform* pcWallLeft = new CBrickPlatform(s_cCreationParams_Platform_Brick_1x16, cocos2d::Vec2(30.f, 480.f));

		// brick platform
	CPlatform* pcWallRight = new CBrickPlatform(s_cCreationParams_Platform_Brick_1x16, cocos2d::Vec2(1890.f, 480.f) );

		// brick platform
	CPlatform* pcBrickPlatform1st = new CBrickPlatform(s_cCreationParams_Platform_Brick_3x1, cocos2d::Vec2(1290.f, 210.f) );

        // crumbling platform
	CPlatform* pcCrumblingPlatform1st = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1410.f, 210.f) );
	CPlatform* pcCrumblingPlatform2nd = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1470.f, 210.f) );
	CPlatform* pcCrumblingPlatform3rd = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1530.f, 210.f) );
	CPlatform* pcCrumblingPlatform4th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1590.f, 210.f) );
	CPlatform* pcCrumblingPlatform5th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1650.f, 210.f) );

		// regular platform	
	CPlatform* pcPlatformRegular2nd = new CRegularPlatform(s_cCreationParams_Platform_3x1, cocos2d::Vec2(1770.f, 210.0f) );
	CPlatform* pcPlatformRegular3rd = new CRegularPlatform(s_cCreationParams_Platform_2x1, cocos2d::Vec2(1800.f, 330.f) );

        // moving platform
	CPlatform* pcPlatformMoving1st = new CMovingPlatform(s_cCreationParams_Platform_Moving_20x1, cocos2d::Vec2(1080.f, 390.f), EPlayerDirection::Left );

		// brick platform
	CPlatform* pcPlatformBrick2nd = new CBrickPlatform(s_cCreationParams_Platform_Brick_3x1, cocos2d::Vec2(1110.f, 450.f));

		// regular platform
	CPlatform* pcPlatformRegular4th = new CRegularPlatform(s_cCreationParams_Platform_4x1, cocos2d::Vec2(180.f, 390.f) );
	CPlatform* pcPlatformRegular5th = new CRegularPlatform(s_cCreationParams_Platform_3x1, cocos2d::Vec2(150.f, 510.f) );
	CPlatform* pcPlatformRegular6th = new CRegularPlatform(s_cCreationParams_Platform_13x1, cocos2d::Vec2(450.f, 630.f) );

        // crumbling platform
	CPlatform* pcCrumblingPlatform6th = new CCrumblingPlatform( s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2( 870.f, 630.f ) );
	CPlatform* pcCrumblingPlatform7th = new CCrumblingPlatform( s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2( 930.f, 630.f ) );
	CPlatform* pcCrumblingPlatform8th = new CCrumblingPlatform( s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2( 990.f, 630.f ) );
	CPlatform* pcCrumblingPlatform9th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1050.f, 630.f) );

	CPlatform* pcPlatformRegular7th = new CRegularPlatform(s_cCreationParams_Platform_1x1, cocos2d::Vec2(1110.f, 630.f) );

        // crumbling platform
	CPlatform* pcCrumblingPlatform10th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1170.f, 630.f));
	CPlatform* pcCrumblingPlatform11th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1230.f, 630.f));
	CPlatform* pcCrumblingPlatform12th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1290.f, 630.f));
	CPlatform* pcCrumblingPlatform13th = new CCrumblingPlatform(s_cCreationParams_Platform_Crumbling_1x1, cocos2d::Vec2(1350.f, 630.f));

		// regular platform
	CPlatform* pcPlatformRegular8th = new CRegularPlatform(s_cCreationParams_Platform_8x1, cocos2d::Vec2(1620.f, 630.f) );
}

void CObjCCGroupPlatform::DestroyPlatforms()
{
	DestroyObjectsReverseOrder([&](CGCObject* pObject)
		{
			// do nothing - DestroyObjectsReverseOrder calls delete!
			GCASSERT(GetGCTypeIDOf(CPlatform) == pObject->GetGCTypeID(), "wrong type!");

		});
}
