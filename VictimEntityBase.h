#pragma once
#ifndef __VictimEntityBase__
#define __VictimEntityBase__


#include "PublicDefine.h"
#include "Entity.h"
#include "Hp.h"
class Hp;
class VictimEntityBase :public Entity
{
public:
	virtual ~VictimEntityBase();
	virtual void Hurt(const AtkProperty HurtValue);//当受到攻击时
	void HpVisible(const bool& IsHpVisible);
	virtual void SetAtkTarget(const bool &IsAtkTarget);//处理锁定攻击目标的显示
	virtual bool GetAtkTarget()const { return IsAtkTarget; };//
protected:
	CC_SYNTHESIZE_PASS_BY_REF(int, myHp, MyHp);//血量
	
	CC_SYNTHESIZE_PASS_BY_REF(int, myState, MyState);//障碍物、怪物障碍
	
	CC_SYNTHESIZE_PASS_BY_REF(float, mySlowTime, MySlowTime);
	CC_SYNTHESIZE_PASS_BY_REF(float, myStopTime, MyStopTime);
	CC_SYNTHESIZE_PASS_BY_REF(float, myPoisonTime, MyPoisonTime);

	CC_SYNTHESIZE_PASS_BY_REF(float, myBulletStateType, MyBulletStateType);
	const int HpCloseTime = 3;
	void CloseHp(float dt);
	virtual void DeadMove(const std::string& DeadImageFile = "")override;
	virtual void HurtMove() = 0;
	virtual bool Init(const int& _Id, const std::string& SCsvFileName);//初始化
	void CreateHpSprite();
	virtual void CheckAtkTarget(float dt);
	virtual void CreateAtkSprite();//创建并设定锁定的攻击目标
	virtual void CreateTargetAnimate();//创建并设置实体的锁定攻击目标动画
private:
	Hp* pHp=nullptr;
	bool HpExsist = false;
	Sprite* pAtkTarget = nullptr;
	Animate* pTargetAnimate = nullptr;
	bool IsAtkTarget = false;
	Vec2 Position;
	Size Size;
};

#endif

