#ifndef _CDOORMOVING_H_
#define _CDOORMOVING_H_

#include "GamerCamp/GCCocosInterface/SGCFactoryCreationParams.h"
#include "GamerCamp/GCObject/GCObjectGroup.h"

class CGameManager;
class CBlock;

class CMovingDoor : public CGCObjectGroup
{
public:
	CMovingDoor( CGameManager& rcGameManager, const CGCFactoryCreationParams& rcFactoryCreationParams );
	CMovingDoor();
	
	~CMovingDoor();

	void MoveBlocksAside() const;
private:
	CGameManager*				m_pcGameManager;
	CBlock*						m_pcUpperBlock;
	CBlock*						m_pcLowerBlock;
	CGCFactoryCreationParams	m_cCreationParams;

	
	virtual void		DestroyItems( void );
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );
	virtual GCTypeID	VGetTypeId( void );
	virtual void		VOnGroupResourceAcquire_PostObject( void );
	virtual void		VOnGroupResourceAcquire() override;
	virtual void		VOnGroupResourceRelease( void );
	void				Init();
	
	const float m_kfPTM		= 60.0f;
	const float m_kfOffset	= 30.0;
	
};


#endif