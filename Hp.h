#pragma once

#ifndef __HpSlot__
#define __HpSlot__


#include "PublicDefine.h"
class VictimEntityBase;
class Hp:public Node
{
public:

    virtual ~Hp();

    static Hp* Create(VictimEntityBase* _VictimEntity);//创建并初始化血条

    virtual void SetHp(const int& rIHp);//反映目标实体的当前生命值 

    virtual const Size& GetSize()const;//获取血条大小


protected:

    virtual bool Init(VictimEntityBase* _VictimEntity);//初始化血量

private:

    VictimEntityBase* pVictimEntity = nullptr;

public:

    ProgressTimer* pHp = nullptr;//用于显示血条

    int HpMax = 0;
};

#endif 