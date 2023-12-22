#pragma once

#ifndef __CarrotFantasy__BulletBase__
#define __CarrotFantasy__BulletBase__

#include "Entity.h"
#include "DynamicPropertyInterFace.h"

class BulletBase :public Entity
{
public:
    std::set<int>& GetIdMap();//获取子弹ID的集合的引用

    virtual void DoMove();//执行子弹移动

    static BulletBase* Create(const int& Id, VictimEntityBase* VictimEntity);//创建子弹实例
   
    virtual AtkProperty GetAtkProperty();//获取子弹的攻击属性

    virtual void DeadMove(const std::string& DeadImageFile = ""); //子弹死亡

    virtual void DoAction(const float& Dt); //执行子弹动作

protected:

    virtual bool Init(const int& _Id, VictimEntityBase* VictimEntity);//初始化子弹
   
    CC_SYNTHESIZE_PASS_BY_REF(int, myAtkState, MyAtkState);//攻击状态

    CC_SYNTHESIZE_PASS_BY_REF(int, myBulletType, MyBulletType);//子弹类型

    CC_SYNTHESIZE_PASS_BY_REF(int, myDeadActCount, MyDeadActCount);//死亡动作的计数

    VictimEntityBase* pAtkTarget = nullptr;//攻击目标的指针

    std::set<int>* IdMap = nullptr;//子弹ID的集合指针
};
#endif