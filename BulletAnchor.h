#pragma once
#include "BulletBase.h"
class BulletAnchor :public BulletBase
{

	friend class TowerAnchor;

protected:
	void DoAction();//执行子弹的动作
	void DoMove()override;//处理子弹的移动操作
	virtual bool Init(const int& Id, VictimEntityBase* VictimEntity);//初始化
	static BulletAnchor* Create(const int& Id, VictimEntityBase* VictimEntity);//创建实例

};
