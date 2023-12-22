#include "BulletLittleAnchor.h"

bool BulletLittleAnchor::Init(const int& Id, VictimEntityBase* VictimEntity)
{
	bool Ret = false;
	do
	{
		CC_BREAK_IF(!BulletAnchor::Init(Id, VictimEntity));

		Ret = true;
	} while (0);
	return Ret;
}

BulletLittleAnchor* BulletLittleAnchor::Create(const int& Id, VictimEntityBase* VictimEntity)
{
	BulletLittleAnchor* pBullet = new BulletLittleAnchor();
	if (pBullet && pBullet->Init(Id, VictimEntity))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}


