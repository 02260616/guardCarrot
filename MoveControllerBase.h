#pragma once
#ifndef __Carrot__MoveControllerBase_h
#define __Carrot__MoveControllerBase_h

#include "MonsterBase.h"

class MoveControllerBase : public Node
{
	friend class MonsterBase;
public:

	virtual ~MoveControllerBase();
	
	virtual void ListenerMonster(float t);//控制怪物动作


private:
	
	void SetMonster(MonsterBase* Momster); //设置移动的怪物
	
	void ErasePoint();//删除路径中的点

	virtual bool Init(MonsterBase* Momster);//初始化

	static MoveControllerBase* Create(MonsterBase* Momster);//创建 MoveControllerBase 的实例

	CC_SYNTHESIZE_PASS_BY_REF(int, myMonsterSpeed, MyMonsterSpeed);

	Animation* AnimationMaker(int BulletType, float t);//怪物动画制作

	MoveControllerBase() {}
private:

	MonsterBase* pMonster = nullptr;

	std::vector<Vec2>* pMapPonits;//存储怪物移动的路径点

};
#endif
