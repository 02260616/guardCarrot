#pragma once
#ifndef __Carrot__GameManager__
#define __Carrot__GameManager__

#include "PublicDefine.h"

class GameManager :public Ref
{
public:

    CREATE_INSTANCE_FUNC(GameManager);

    DESTROY_INSTANCE_FUNC();

protected:

    virtual bool Init();//��ʼ��

    CC_SYNTHESIZE_PASS_BY_REF(int, myCarrotHp, MyCarrotHp);// ��Ϸ�еĺ��ܲ�����ֵ

    CC_SYNTHESIZE_PASS_BY_REF(int, myMonsterBatch, MyMonsterBatch); // ��Ϸ�еĹ�������

    CC_SYNTHESIZE_PASS_BY_REF(bool, myIsPause, MyIsPause);// ��Ϸ�Ƿ�����ͣ״̬

private:

    GameManager() {}// ���캯��

    GameManager(const GameManager&) = delete;// ��ֹ�������캯��

    GameManager& operator=(const GameManager&) = delete;// ��ֹ��ֵ����������

};

#endif 
