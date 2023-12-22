#pragma once
#ifndef __Carrot__MonsterBuilder__
#define __Carrot__MonsterBuilder__
#include "cocos2d.h"
using namespace cocos2d;
class MonsterBuilder :public Node {

protected:

	virtual bool Init();

	float DelayTime;// 延迟时间

	int MonsterCount;// 怪物数量

	int MonsterBuilderNumber;// 怪物生成器编号

	int ReadFileMonsterCount;// 读取的文件里的怪物数量

	bool Delay;// 是否延迟生成怪物

	const int ConstDelayTime = 8;// 默认延迟时间

	int Number;// 数量

	int Batch; // 批次

	virtual void OnEnter();

	void CreateMonsterUpdate(float t);// 更新生成怪物的函数

	void MonsterFactoryController();// 怪物工厂控制函数

	Animation* createMonsterAnimation; // 怪物动画

	~MonsterBuilder();

public:

	int GetBatch();// 获取批次数

	CREATE_FUNC(MonsterBuilder);

	CC_SYNTHESIZE_PASS_BY_REF(int, myBuildNumber, MyBuildNumber);
};
#endif