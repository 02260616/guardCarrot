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

    virtual bool Init();//初始化

    CC_SYNTHESIZE_PASS_BY_REF(int, myCarrotHp, MyCarrotHp);// 游戏中的胡萝卜生命值

    CC_SYNTHESIZE_PASS_BY_REF(int, myMonsterBatch, MyMonsterBatch); // 游戏中的怪物批次

    CC_SYNTHESIZE_PASS_BY_REF(bool, myIsPause, MyIsPause);// 游戏是否处于暂停状态

private:

    GameManager() {}// 构造函数

    GameManager(const GameManager&) = delete;// 禁止拷贝构造函数

    GameManager& operator=(const GameManager&) = delete;// 禁止赋值操作符重载

};

#endif 
