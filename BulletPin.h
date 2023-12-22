#pragma once
#include "BulletBase.h"
class  BulletPin :public BulletBase
{

	friend class TowerPin;

protected:
	void DoAction();
	void DoMove()override;
	virtual bool Init(const int& Id, VictimEntityBase* VictimEntity);
	static BulletPin* Create(const int& Id, VictimEntityBase* VictimEntity);

};