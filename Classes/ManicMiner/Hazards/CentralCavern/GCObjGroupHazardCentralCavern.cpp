////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "ManicMiner/Hazards/GCObjGroupHazard.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "ManicMiner/Hazards/GCObjHazard.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"

// Static Creation Params lifted out of GCObjHazard to here to allow GCObjHazard to be a class which can represent differnt Hazard sprites/animations/physics.

//static CGCFactoryCreationParams s_cCreationParams_CGCObj_EBush("CGCObjHazard_EBush", "TexturePacker/Sprites/Hazards/bush/bush.plist", "bush", b2_dynamicBody, true);
//static CGCFactoryCreationParams s_cCreationParams_CGCObj_EDownwardSpike("CGCObjHazard_EDownwardSpike", "TexturePacker/Sprites/Hazards/downwardSpike/downwardSpike.plist", "downwardSpike", b2_dynamicBody, true);
//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCObjGroupHazardCentralCavern::CGCObjGroupHazardCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual function.
//////////////////////////////////////////////////////////////////////////
CGCObjGroupHazardCentralCavern::~CGCObjGroupHazardCentralCavern()
{}

//////////////////////////////////////////////////////////////////////////
// This function instantiates all required hazards for the Central Cavern level.
//////////////////////////////////////////////////////////////////////////
// virtual function.
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupHazardCentralCavern::VOnGroupResourceAcquire()
{
	//CGCObjHazard* pHazard1 = new CGCObjHazard(cocos2d::Vec2(700.0f, 210.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	//CGCObjHazard* pHazard2 = new CGCObjHazard(cocos2d::Vec2(1250.0f, 450.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	//CGCObjHazard* pHazard3 = new CGCObjHazard(cocos2d::Vec2(1450.0f, 690.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	//CGCObjHazard* pHazard4 = new CGCObjHazard(cocos2d::Vec2(1650.0f, 690.0f), HazardTypes::EHazardId::EBush, s_cCreationParams_CGCObj_EBush);
	//CGCObjHazard* pHazard5 = new CGCObjHazard(cocos2d::Vec2(740.0f, 930.0f), HazardTypes::EHazardId::EDownwardSpike, s_cCreationParams_CGCObj_EDownwardSpike);
	//CGCObjHazard* pHazard6 = new CGCObjHazard(cocos2d::Vec2(940.0f, 930.0f), HazardTypes::EHazardId::EDownwardSpike, s_cCreationParams_CGCObj_EDownwardSpike);

}

//////////////////////////////////////////////////////////////////////////
// Function to return the typeid of the GCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupHazardCentralCavern::VGetTypeId()
{
	return GetGCTypeIDOf(CGCObjGroupHazardCentralCavern);
}