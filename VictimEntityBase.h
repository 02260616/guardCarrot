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
	virtual void Hurt(const AtkProperty HurtValue);//���ܵ�����ʱ
	void HpVisible(const bool& IsHpVisible);
	virtual void SetAtkTarget(const bool &IsAtkTarget);//������������Ŀ�����ʾ
	virtual bool GetAtkTarget()const { return IsAtkTarget; };//
protected:
	CC_SYNTHESIZE_PASS_BY_REF(int, myHp, MyHp);//Ѫ��
	
	CC_SYNTHESIZE_PASS_BY_REF(int, myState, MyState);//�ϰ�������ϰ�
	
	CC_SYNTHESIZE_PASS_BY_REF(float, mySlowTime, MySlowTime);
	CC_SYNTHESIZE_PASS_BY_REF(float, myStopTime, MyStopTime);
	CC_SYNTHESIZE_PASS_BY_REF(float, myPoisonTime, MyPoisonTime);

	CC_SYNTHESIZE_PASS_BY_REF(float, myBulletStateType, MyBulletStateType);
	const int HpCloseTime = 3;
	void CloseHp(float dt);
	virtual void DeadMove(const std::string& DeadImageFile = "")override;
	virtual void HurtMove() = 0;
	virtual bool Init(const int& _Id, const std::string& SCsvFileName);//��ʼ��
	void CreateHpSprite();
	virtual void CheckAtkTarget(float dt);
	virtual void CreateAtkSprite();//�������趨�����Ĺ���Ŀ��
	virtual void CreateTargetAnimate();//����������ʵ�����������Ŀ�궯��
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

