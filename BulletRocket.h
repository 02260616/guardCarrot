#pragma once
#ifndef __Carrot__BulletRocket__
#define __Carrot__BulletRocket__
#include "BulletBase.h"
class VictimEntityBase;

class BulletRocket :public BulletBase {

	friend class TowerRocket;

protected:

	void DoAction();

	void DoMove()override;

	virtual bool Init(const int& Id, VictimEntityBase* VictimEntity);

	static BulletRocket* Create(const int& Id, VictimEntityBase* VictimEntity);
};
#endif 