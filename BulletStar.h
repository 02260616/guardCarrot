#ifndef __Carrot__BulletStar__
#define __Carrot__BulletStar__
#include "BulletBase.h"
class VictimEntityBase;

class BulletStar :public BulletBase {

	friend class TowerStar;

protected:

	void DoAction();

	void DoMove()override;

	virtual bool Init(const int& Id, VictimEntityBase* VictimEntity);

	static BulletStar* Create(const int& Id, VictimEntityBase* VictimEntity);
};
#endif