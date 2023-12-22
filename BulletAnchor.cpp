#include "BulletAnchor.h"
#include "VictimEntityBase.h"
//#include "MonsterManager.h"
//#include "MonsterBase.h"
//#include "BarrierManager.h"
//#include "BarrierBase.h"
//#include "GameScene.h"



bool BulletAnchor::Init(const int& Id, VictimEntityBase* VictimEntity)
{
	bool Ret = false;
	do
	{
		CC_BREAK_IF(!BulletBase::Init(Id, VictimEntity));

		Ret = true;
	} while (0);
	return Ret;
}

BulletAnchor* BulletAnchor::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	BulletAnchor* pBullet = new BulletAnchor();
	if (pBullet && pBullet->Init(Id, VictimEntity))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

