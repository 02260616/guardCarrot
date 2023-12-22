#pragma once

#ifndef __HpSlot__
#define __HpSlot__


#include "PublicDefine.h"
class VictimEntityBase;
class Hp:public Node
{
public:

    virtual ~Hp();

    static Hp* Create(VictimEntityBase* _VictimEntity);//��������ʼ��Ѫ��

    virtual void SetHp(const int& rIHp);//��ӳĿ��ʵ��ĵ�ǰ����ֵ 

    virtual const Size& GetSize()const;//��ȡѪ����С


protected:

    virtual bool Init(VictimEntityBase* _VictimEntity);//��ʼ��Ѫ��

private:

    VictimEntityBase* pVictimEntity = nullptr;

public:

    ProgressTimer* pHp = nullptr;//������ʾѪ��

    int HpMax = 0;
};

#endif 