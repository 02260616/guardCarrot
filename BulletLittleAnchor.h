#pragma once
#include "BulletAnchor.h"
class BulletLittleAnchor :public BulletAnchor
{

protected:

	virtual bool Init(const int& Id, VictimEntityBase* VictimEntity);
	static BulletLittleAnchor* Create(const int& Id, VictimEntityBase* VictimEntity);

};